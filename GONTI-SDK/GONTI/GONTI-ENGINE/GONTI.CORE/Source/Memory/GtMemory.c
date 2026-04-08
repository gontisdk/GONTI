#include "GtMemory.h"

#include <stdio.h>
#include "../Platform/GtPlatform.h"
#include "../CStringTools/GtCStrTools.h"

static const char* memoryTagStrings[GT_MEM_TAG_MAX_TAGS] = {
    "UNKNOWN:.................................",
    "ARRAY:...................................",
    "LINEAR_ALLOC:............................",
    "DARRAY:..................................",
    "DARRAYEX:................................",
    "DICT:....................................",
    "RING_QUEUE:..............................",
    "BST:.....................................",
    "STRING:..................................",
    "APPLICATION:.............................",
    "JOB:.....................................",
    "TEXTURE:.................................",
    "MAT_INST:................................",
    "WINDOW:..................................",
    "RENDERER:................................",
    "ENTRY:...................................",
    "TRANSFORM:...............................",
    "ENTITY:..................................",
    "ENTITY_MODE:.............................",
    "SCENE:...................................",
    "MATH:....................................",
    "VECTOR:..................................",
    "MATRIX:..................................",
    "BIGINTEGER..............................."
};

static const char* memTagClearArr[GT_MEM_TAG_MAX_TAGS] = {
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
    "ENTITY_MODE",
    "SCENE",
    "MATH",
    "VECTOR",
    "MATRIX",
    "BIGINTEGER"
};

struct GtMemStats {
    GtU64 totalAllocatedCount;
    GtU64 taggedAllocations[GT_MEM_TAG_MAX_TAGS];
};

typedef struct GtMemSysState {
    struct GtMemStats stats;
    GtAllocationHeader* listHead;
    GtU64 allocCount;
} GtMemSysState;
static GtMemSysState* statePtr;

/* B8 */
GtB8 gontiMemoryCheckLeaks() {
    //if (statePtr) {
        if (!statePtr->listHead) {
            GTINFO("Memory shutdown: All clear. No leaks.");
            return GtFalse;
        }

        printf("\n");
        GTERROR("***************************************************");
        GTERROR("              MEMORY LEAKS DETECTED                ");
        GTERROR("***************************************************");

        GtAllocationHeader* curr = statePtr->listHead;
        GtU64 totalLeaked = 0;

        while (curr) {
            GTERROR(
                "  -> Leak: %p | Size: %llu B | Tag: %s",
                (void*)(curr + 1),
                curr->size,
                memTagClearArr[curr->tag]
            );

            totalLeaked += curr->size;
            curr = curr->next;
        }

        GTERROR("  TOTAL LEAKED: %.2f KiB", (GtF32)totalLeaked / 1024.0f);
        GTERROR("***************************************************");
        printf("\n");
    //}

    return GtTrue;
}

/* CHAR* */
char* gontiGetMemoryUsageStr() {
    const GtU64 gib = 1024 * 1024 * 1024;
    const GtU64 mib = 1024 * 1024;
    const GtU64 kib = 1024;

    char buffer[8000] = "System memory use (tagged):\n";
    GtU64 offset = gontiStringLength(buffer);
    const GtU64 bufferSize = 8000;

    for (GtU32 i = 0; i < GT_MEM_TAG_MAX_TAGS; i++) {
        char unit[4] = "XiB";
        GtF32 amount = 1.0f;

        if (statePtr->stats.taggedAllocations[i] >= gib) {
            unit[0] = 'G';
            amount = statePtr->stats.taggedAllocations[i] / (GtF32)gib;
        } else if (statePtr->stats.taggedAllocations[i] >= mib) {
            unit[0] = 'M';
            amount = statePtr->stats.taggedAllocations[i] / (GtF32)mib;
        } else if (statePtr->stats.taggedAllocations[i] >= kib) {
            unit[0] = 'K';
            amount = statePtr->stats.taggedAllocations[i] / (GtF32)kib;
        } else {
            unit[0] = 'B';
            unit[1] = 0;
            amount = (GtF32)statePtr->stats.taggedAllocations[i];
        }

        if (offset >= bufferSize) break;

        GtI32 length = snprintf(buffer + offset, 8000, "  %s-> %.2f%s\n", memoryTagStrings[i], amount, unit);
        if (length < 0) break;
        offset += length;
    }

    char* outString = gontiStringDuplicate(buffer);
    return outString;
}

/* U64 */
GtU64 gontiMemoryGetAllocCount() {
    return statePtr->allocCount;
}

/*VOID*/
void gontiMemoryInitialize(GtU64* memoryRequirement, void* state) {
    *memoryRequirement = sizeof(GtMemSysState);
    if (!state) return;

    statePtr = state;
    statePtr->listHead = 0;
    statePtr->allocCount = 0;
    gontiPlatformZeroMemory(&statePtr->stats, sizeof(statePtr->stats));
}
void gontiMemoryShutdown(void* state) {
    gontiMemoryCheckLeaks();
    gontiPlatformZeroMemory(&statePtr->stats, sizeof(statePtr->stats));
    statePtr = 0;
}
void __gontiMemoryFree(void* block) {
    if (block == 0) return;

    GtAllocationHeader* header = ((GtAllocationHeader*)block) - 1;
    if (header->_magic != 0xDEADBEEF) {
        GTERROR("__gontiMemoryFree: Memory corruption or double free at %p!", block);
        return;
    }

    if (statePtr) {
    if (header->prev) {
        header->prev->next = header->next;
    } else {
        if (statePtr->listHead == header) {
            statePtr->listHead = header->next;
        }
    }

    if (header->next) {
        header->next->prev = header->prev;
    }

    if (header->tag < GT_MEM_TAG_MAX_TAGS) {
        statePtr->stats.totalAllocatedCount -= 1;
        statePtr->stats.taggedAllocations[header->tag] -= header->size;
    } else {
        GTWARN("__gontiMemoryFree: Invalid tag on block %p. Stats might be incorrect.", block);
    }

    statePtr->allocCount--;
    }

    header->_magic = 0;
    header->next = 0;
    header->prev = 0;

    gontiPlatformFree(header, GtFalse);
}

/* CHAR** */
const char** __gontiGetMemoryTagStringsArr() {
    return (const char**)memTagClearArr;
}

/* VOID* */
void* __gontiMemoryAllocate(GtU64 size, GtMemTag memTag) {
    if (memTag == GT_MEM_TAG_UNKOWN)
        GTWARN("__gontiMemoryAllocate() called with UNKNOWN tag");

    GtU64 total = size + sizeof(GtAllocationHeader);
    GtAllocationHeader* header = gontiPlatformAllocate(total, GtFalse);

    if (header == 0) {
        GTWARN("__gontiMemoryAllocate: Out of memory!");
        return 0; 
    }

    header->size = size;
    header->tag = memTag;
    header->_magic = 0xDEADBEEF;

    header->prev = 0;

    if (statePtr) {
    header->next = statePtr->listHead;

    if (statePtr->listHead) {
        statePtr->listHead->prev = header;
    }
    statePtr->listHead = header;

    statePtr->stats.totalAllocatedCount += 1;
    statePtr->stats.taggedAllocations[memTag] += size;

    statePtr->allocCount++;
    }

    void* block = (void*)(header + 1);
    gontiPlatformZeroMemory(block, size);
    return block;
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
void* __gontiMemoryReallocate(void* block, GtU64 newSize) {
    if (!block) return __gontiMemoryAllocate(newSize, GT_MEM_TAG_UNKOWN);
    
    if (newSize == 0) {
        __gontiMemoryFree(block);
        return 0;
    }

    GtAllocationHeader* oldHeader = ((GtAllocationHeader*)block) - 1;
    if (oldHeader->_magic != 0xDEADBEEF) {
        GTERROR("__gontiMemoryReallocate: Memory corruption detected!");
        return 0;
    }

    GtMemTag tag = oldHeader->tag;
    GtU64 oldSize = oldHeader->size;

    GtU64 total = newSize + sizeof(GtAllocationHeader);
    GtAllocationHeader* newHeader = gontiPlatformReallocate(oldHeader, total);
    
    if (!newHeader) {
        GTWARN("__gontiMemoryReallocate: Failed!");
        return 0;
    }

    if (statePtr) {
    statePtr->stats.taggedAllocations[tag] -= oldSize;
    statePtr->stats.taggedAllocations[tag] += newSize;
    newHeader->size = newSize;

    if (newHeader != oldHeader) {
        if (newHeader->prev) newHeader->prev->next = newHeader;
        else statePtr->listHead = newHeader;
        if (newHeader->next) newHeader->next->prev = newHeader;
        
    }
    }

    return (void*)(newHeader + 1);
}
