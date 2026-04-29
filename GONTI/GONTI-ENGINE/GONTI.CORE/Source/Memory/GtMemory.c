#include <Memory/GtMemory.h>

#include <stdio.h>
#include <Platform/GtPlatform.h>
#include <CStringTools/GtCStrTools.h>

static const char* const memTagNames[GT_MEM_TAG_MAX_TAGS] = {
    "UNKNOWN",
    "ARRAY",
    "LINEAR_ALLOC",
    "DARRAY",
    "DARRAYEX",
    "DICT",
    "RING_QUEUE",
    "BST",
    "STRING",
    "APPLICATION",
    "JOB",
    "TEXTURE",
    "MAT_INST",
    "WINDOW",
    "RENDERER",
    "ENTRY",
    "TRANSFORM",
    "ENTITY",
    "ENTITY_NODE",
    "SCENE",
    "MATH",
    "VECTOR",
    "MATRIX",
    "BIGINT"
};

typedef char _gt_tag_count_check[
    (sizeof(memTagNames) / sizeof(memTagNames[0]) == GT_MEM_TAG_MAX_TAGS) ? 1 : -1
];

typedef struct GtMemStats {
    GtU64 totalAllocatedCount;
    GtU64 taggedAllocations[GT_MEM_TAG_MAX_TAGS];
} GtMemStats;

typedef struct GtMemSysState {
    GtMemStats stats;
    GtMemAllocHdr* listHead;
    GtU64 allocCount;
} GtMemSysState;

static GtMemSysState* s_state = NULL;

static void listRemove(GtMemAllocHdr* hdr) {
    if (hdr->prev) {
        hdr->prev->next = hdr->next;
    } else {
        if (s_state->listHead != hdr) {
            GTERROR("listRemove: LIST CORRUPT — node %p has no prev "
                    "but is not the list head (head=%p). "
                    "Possible cause: double-free or header corruption.",
                    (void*)hdr, (void*)s_state->listHead);
            return;
        }
        s_state->listHead = hdr->next;
    }

    if (hdr->next) {
        hdr->next->prev = hdr->prev;
    }
}

static void listPrepend(GtMemAllocHdr* hdr) {
    hdr->prev = NULL;
    hdr->next = s_state->listHead;

    if (s_state->listHead) {
        s_state->listHead->prev = hdr;
    }
    s_state->listHead = hdr;
}

static void formatBytes(GtU64 bytes, GtF32* outAmount, char outUnit[4]) {
    static const GtU64 GiB = 1024ULL * 1024 * 1024;
    static const GtU64 MiB = 1024ULL * 1024;
    static const GtU64 KiB = 1024ULL;

    if (bytes >= GiB) {
        outUnit[0] = 'G'; outUnit[1] = 'i'; outUnit[2] = 'B'; outUnit[3] = '\0';
        *outAmount = (GtF32)bytes / (GtF32)GiB;
    } else if (bytes >= MiB) {
        outUnit[0] = 'M'; outUnit[1] = 'i'; outUnit[2] = 'B'; outUnit[3] = '\0';
        *outAmount = (GtF32)bytes / (GtF32)MiB;
    } else if (bytes >= KiB) {
        outUnit[0] = 'K'; outUnit[1] = 'i'; outUnit[2] = 'B'; outUnit[3] = '\0';
        *outAmount = (GtF32)bytes / (GtF32)KiB;
    } else {
        outUnit[0] = 'B'; outUnit[1] = '\0'; outUnit[2] = '\0'; outUnit[3] = '\0';
        *outAmount = (GtF32)bytes;
    }
}

void gontiMemoryInitialize(GtU64* memoryRequirement, void* state) {
    *memoryRequirement = sizeof(GtMemSysState);
    if (!state) return;

    s_state = (GtMemSysState*)state;
    s_state->listHead  = NULL;
    s_state->allocCount = 0;
    gontiPlatformZeroMemory(&s_state->stats, sizeof(s_state->stats));
}

void gontiMemoryShutdown(void* state) {
    (void)state; 
    gontiMemoryCheckLeaks();
    gontiPlatformZeroMemory(&s_state->stats, sizeof(s_state->stats));
    s_state = NULL;
}

GtU64 gontiMemoryGetAllocCount(void) {
    return s_state->allocCount;
}

#define GT_MEM_USAGE_BUF_SIZE 8192
char* gontiGetMemoryUsageStr(void) {
    char buffer[GT_MEM_USAGE_BUF_SIZE];

    GtU64 offset = (GtU64)snprintf(buffer, GT_MEM_USAGE_BUF_SIZE,
                                "System memory use (tagged):\n");

    for (GtU32 i = 0; i < GT_MEM_TAG_MAX_TAGS; ++i) {
        if (offset >= GT_MEM_USAGE_BUF_SIZE - 1) break;

        GtF32 amount;
        char  unit[4];
        formatBytes(s_state->stats.taggedAllocations[i], &amount, unit);

        GtI32 written = snprintf(buffer + offset, GT_MEM_USAGE_BUF_SIZE - offset,
                                "  %-20s %.2f %s\n",
                                memTagNames[i], amount, unit);
        if (written < 0) break;
        offset += (GtU64)written;
    }

    return gontiStringDuplicate(buffer);
} 
#undef GT_MEM_USAGE_BUF_SIZE

GtB8 gontiMemoryCheckLeaks(void) {
    if (!s_state || !s_state->listHead) {
        GTINFO("Memory shutdown: All clear. No leaks detected.");
        return GtFalse;
    }

    GTERROR("***************************************************");
    GTERROR("          MEMORY LEAKS DETECTED                    ");
    GTERROR("***************************************************");

    GtMemAllocHdr* curr = s_state->listHead;
    GtU64 totalLeaked = 0;

    while (curr) {
        const char* tagName = (curr->tag < GT_MEM_TAG_MAX_TAGS)
            ? memTagNames[curr->tag] : "INVALID";

        GTERROR("  -> Leak: %p | Size: %llu B | Tag: %s",
                (void*)(curr + 1), curr->size, tagName);

        totalLeaked += curr->size;
        curr = curr->next;
    }

    GTERROR("  TOTAL LEAKED: %.2f KiB", (GtF32)totalLeaked / 1024.0f);
    GTERROR("***************************************************");

    return GtTrue;
}

void* __gontiMemoryAllocate(GtU64 size, GtMemTag tag) {
    if (tag == GT_MEM_TAG_UNKNOWN) {
        GTWARN("__gontiMemoryAllocate: called with GT_MEM_TAG_UNKNOWN");
    }

    GtU64 total = size + sizeof(GtMemAllocHdr);
    GtMemAllocHdr* hdr = (GtMemAllocHdr*)gontiPlatformAllocate(total, GtFalse);

    if (!hdr) {
        GTERROR("__gontiMemoryAllocate: platform allocation failed (requested %llu B)", total);
        return NULL;
    }

    hdr->size   = size;
    hdr->tag    = tag;
    hdr->_magic = GT_MEM_MAGIC;

    if (s_state) {
        listPrepend(hdr);
        s_state->stats.totalAllocatedCount++;
        s_state->stats.taggedAllocations[tag] += size;
        s_state->allocCount++;
    }

    void* block = (void*)(hdr + 1);
    gontiPlatformZeroMemory(block, size);
    return block;
}

void __gontiMemoryFree(void* block) {
    if (!block) return;

    GtMemAllocHdr* hdr = ((GtMemAllocHdr*)block) - 1;

    if (hdr->_magic != GT_MEM_MAGIC) {
        GTERROR("__gontiMemoryFree: corruption or double-free at %p", block);
        return;
    }

    if (s_state) {
        listRemove(hdr);

        if (hdr->tag < GT_MEM_TAG_MAX_TAGS) {
            s_state->stats.totalAllocatedCount--;
            s_state->stats.taggedAllocations[hdr->tag] -= hdr->size;
        } else {
            GTWARN("__gontiMemoryFree: invalid tag on block %p — stats may be inaccurate", block);
        }
        s_state->allocCount--;
    }

    hdr->_magic = 0;
    hdr->next   = NULL;
    hdr->prev   = NULL;

    gontiPlatformFree(hdr, GtFalse);
}

void* __gontiMemoryReallocate(void* block, GtU64 newSize) {
    if (!block)   return __gontiMemoryAllocate(newSize, GT_MEM_TAG_UNKNOWN);
    if (!newSize) { __gontiMemoryFree(block); return NULL; }

    GtMemAllocHdr* oldHdr = ((GtMemAllocHdr*)block) - 1;
    if (oldHdr->_magic != GT_MEM_MAGIC) {
        GTERROR("__gontiMemoryReallocate: corruption at %p", block);
        return NULL;
    }

    GtMemTag oldTag  = oldHdr->tag;
    GtU64    oldSize = oldHdr->size;
    GtMemAllocHdr* prevNode = oldHdr->prev;
    GtMemAllocHdr* nextNode = oldHdr->next;

    if (s_state) {
        listRemove(oldHdr);
        s_state->stats.taggedAllocations[oldTag] -= oldSize;
        s_state->allocCount--;
    }

    oldHdr->_magic = 0;
    
    GtMemAllocHdr* newHdr = (GtMemAllocHdr*)gontiPlatformReallocate(oldHdr, newSize + sizeof(GtMemAllocHdr));
    if (!newHdr) {
        GTERROR("__gontiMemoryReallocate: platform realloc failed");
        oldHdr->_magic = GT_MEM_MAGIC;
        if (s_state) {
            listPrepend(oldHdr);
            s_state->stats.taggedAllocations[oldTag] += oldSize;
            s_state->allocCount++;
        }
        return NULL;
    }

    newHdr->size   = newSize;
    newHdr->tag    = oldTag;
    newHdr->_magic = GT_MEM_MAGIC;
    newHdr->prev   = prevNode;
    newHdr->next   = nextNode;

    if (s_state) {
        if (newHdr->prev) newHdr->prev->next = newHdr;
        else s_state->listHead  = newHdr;
        if (newHdr->next) newHdr->next->prev = newHdr;

        s_state->stats.taggedAllocations[oldTag] += newSize;
        s_state->allocCount++;
        // s_state->stats.totalAllocatedCount++; // NOTE: Optional, normally realloc != new alloc
    }

    if (newSize > oldSize) {
        GtU8* newBytes = (GtU8*)(newHdr + 1) + oldSize;
        gontiPlatformZeroMemory(newBytes, newSize - oldSize);
    }

    return (void*)(newHdr + 1);
}

void* __gontiMemoryZero(void* block, GtU64 size) {
    return gontiPlatformZeroMemory(block, size);
}

void* __gontiMemoryCopy(void* dest, const void* source, GtU64 size) {
    return gontiPlatformCopyMemory(dest, source, size);
}

void* __gontiMemorySet(void* dest, GtI32 value, GtU64 size) {
    return gontiPlatformSetMemory(dest, value, size);
}

const char** __gontiGetMemoryTagStringsArr(void) {
    return (const char**)memTagNames;
}