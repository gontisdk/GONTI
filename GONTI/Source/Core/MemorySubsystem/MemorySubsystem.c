#include "MemorySubsystem.h"

#ifdef MemorySubsystem_ON

#include <stdio.h>
#include "../Platform/Platform.h"
#include "../../Utilities/Logging/Scripts/Logger.h"
#include "../../Libs/StringLib/StringLib.h"

static const char* memoryTagStrings[MEMORY_TAG_MAX_TAGS] = {
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

struct gontiMemoryStats m_stats;

/* CHAR* */
char* k_getMemoryUsageStr() {
    const u64 gib = 1024 * 1024 * 1024;
    const u64 mib = 1024 * 1024;
    const u64 kib = 1024;

    char buffer[8000] = "System memory use (tagged):\n";
    u64 offset = stringLength(buffer);

    for (u32 i = 0; i < MEMORY_TAG_MAX_TAGS; i++) {
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

    char* outString = stringDuplicate(buffer);
    return outString;
}

/*VOID*/
void initializeMemory() {
    platformZeroMemory(&m_stats, sizeof(m_stats));
}
void shutdownMemory() {
    
}
void k_free(void* block, u64 size, gontiMemoryTag memTag) {
    if (memTag == MEMORY_TAG_UNKOWN) {
        KWARN("k_free() called using MEMORY_TAG_UNKOWN. Re-class this allocation");
    }

    m_stats.totalAllocated -= 1;
    m_stats.taggedAllocations[memTag] -= size;

    // TODO: Memory aligment
    platformFree(block, false);
}

/* VOID* */
void* k_allocate(u64 size, gontiMemoryTag memTag) {
    if (memTag == MEMORY_TAG_UNKOWN) KWARN("k_allocate() called using MEMORY_TAG_UNKOWN. Re-class this allocation");

    m_stats.totalAllocated += 1;
    m_stats.taggedAllocations[memTag] += size;

    // TODO: Memory aligment
    void* block = platformAllocate(size, false);
    platformZeroMemory(block, size);

    return block;
}
void* k_zeroMemory(void* block, u64 size) {
    return platformZeroMemory(block, size);
}
void* k_copyMemory(void* dest, const void* source, u64 size) {
    return platformCopyMemory(dest, source, size);
}
void* k_setMemory(void* dest, i32 value, u64 size) {
    return platformSetMemory(dest, value, size);
}
void* k_reallocate(void* block, u64 size) {
    return platformReallocate(block, size);
}

#endif