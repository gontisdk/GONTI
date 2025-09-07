#include "Memory.h"

#include <stdio.h>
#include "../Platform/Platform.h"
#include "../Logging/Logger.h"
#include "../String/String.h"

static const char* memoryTagStrings[GONTI_MEMORY_TAG_MAX_TAGS] = {
    "UNKNOWN:.................................",
    "ARRAY:...................................",
    "ARRAYEX:.................................",
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

struct GontiMemoryStats m_stats;

/* CHAR* */
char* k_getMemoryUsageStr() {
    const u64 gib = 1024 * 1024 * 1024;
    const u64 mib = 1024 * 1024;
    const u64 kib = 1024;

    char buffer[8000] = "System memory use (tagged):\n";
    u64 offset = gontiStringLength(buffer);

    for (u32 i = 0; i < GONTI_MEMORY_TAG_MAX_TAGS; i++) {
        char unit[4] = "XiB";
        float amount = 1.0f;

        if (m_stats.taggedAllocations[i] >= gib) {
            unit[0] = 'G';
            amount = m_stats.taggedAllocations[i] / (float)gib;
        } else if (m_stats.taggedAllocations[i] >= mib) {
            unit[0] = 'M';
            amount = m_stats.taggedAllocations[i] / (float)mib;
        } else if (m_stats.taggedAllocations[i] >= kib) {
            unit[0] = 'K';
            amount = m_stats.taggedAllocations[i] / (float)kib;
        } else {
            unit[0] = 'B';
            unit[1] = 0;
            amount = (float)m_stats.taggedAllocations[i];
        }

        i32 length = snprintf(buffer + offset, 8000, "  %s-> %.2f%s\n", memoryTagStrings[i], amount, unit);
        offset += length;
    }

    char* outString = gontiStringDuplicate(buffer);
    return outString;
}

/*VOID*/
void gontiInitializeMemory() {
    gontiPlatformZeroMemory(&m_stats, sizeof(struct GontiMemoryStats));
}
void gontiShutdownMemory() {
    gontiPlatformFree(&m_stats, false);
}
void k_free(void* block, u64 size, GontiMemoryTag memTag) {
    if (memTag == GONTI_MEMORY_TAG_UNKOWN) {
        KWARN("k_free() called using GONTI_MEMORY_TAG_UNKOWN. Re-class this allocation");
    }

    m_stats.totalAllocated -= 1;
    m_stats.taggedAllocations[memTag] -= size;

    // TODO: Memory aligment
    gontiPlatformFree(block, false);
}

/* VOID* */
void* k_allocate(u64 size, GontiMemoryTag memTag) {
    if (memTag == GONTI_MEMORY_TAG_UNKOWN) KWARN("k_allocate() called using GONTI_MEMORY_TAG_UNKOWN. Re-class this allocation");

    m_stats.totalAllocated += 1;
    m_stats.taggedAllocations[memTag] += size;

    // TODO: Memory aligment
    void* block = gontiPlatformAllocate(size, false);
    gontiPlatformZeroMemory(block, size);

    return block;
}
void* k_zeroMemory(void* block, u64 size) {
    return gontiPlatformZeroMemory(block, size);
}
void* k_copyMemory(void* dest, const void* source, u64 size) {
    return gontiPlatformCopyMemory(dest, source, size);
}
void* k_setMemory(void* dest, i32 value, u64 size) {
    return gontiPlatformSetMemory(dest, value, size);
}
void* k_reallocate(void* block, u64 size) {
    return gontiPlatformReallocate(block, size);
}