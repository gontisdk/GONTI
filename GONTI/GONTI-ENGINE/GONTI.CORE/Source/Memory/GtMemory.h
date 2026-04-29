#ifndef GT_MEMORY_H
#define GT_MEMORY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <Defines/GtDefines.inl>
#include <Logging/GtLogger.h>
#include <Memory/GtMemTypes.inl>

GTAPI GtB8 gontiMemoryCheckLeaks(void);
GTAPI char* gontiGetMemoryUsageStr(void);
GTAPI GtU64 gontiMemoryGetAllocCount(void);
GTAPI void gontiMemoryInitialize(GtU64* memoryRequirement, void* state);
GTAPI void gontiMemoryShutdown(void* state);
GTAPI void* __gontiMemoryAllocate(GtU64 size, GtMemTag tag);
GTAPI void* __gontiMemoryReallocate(void* block, GtU64 newSize);
GTAPI void __gontiMemoryFree(void* block);
GTAPI void* __gontiMemoryZero(void* block, GtU64 size);
GTAPI void* __gontiMemoryCopy(void* dest, const void* source, GtU64 size);
GTAPI void* __gontiMemorySet(void* dest, GtI32 value, GtU64 size);
GTAPI const char** __gontiGetMemoryTagStringsArr(void);

#define gt_allocate(size, memTag)                                               \
    ({                                                                          \
        void* _blk = __gontiMemoryAllocate((size), (memTag));                   \
        if (_blk) {                                                             \
            const char* _tag = ((memTag) < GT_MEM_TAG_MAX_TAGS)                 \
                ? __gontiGetMemoryTagStringsArr()[(memTag)] : "INVALID";        \
            GTDEBUG("[MEM] ALLOC  | %p | Size: %llu | Tag: %s | %s:%d",         \
                    _blk, (GtU64)(size), _tag, __FILE__, __LINE__);             \
        } else {                                                                \
            GTERROR("[MEM] ALLOC FAILED | Size: %llu | Tag: %d | %s:%d",        \
                    (GtU64)(size), (memTag), __FILE__, __LINE__);               \
        }                                                                       \
        _blk;                                                                   \
    })

#define gt_reallocate(block, newSize)                                           \
    ({                                                                          \
        GTDEBUG("[MEM] REALLOC | %p | NewSize: %llu | %s:%d",                   \
                (block), (GtU64)(newSize), __FILE__, __LINE__);                 \
        void* _newBlk = __gontiMemoryReallocate((block), (newSize));            \
        GTDEBUG("[MEM] REALLOC | Old: %p -> New: %p", (block), _newBlk);        \
        _newBlk;                                                                \
    })

#define gt_free(block)                                                          \
    do {                                                                        \
        if (!(block)) {                                                         \
            GTERROR("[MEM] FREE | NULL PTR | %s:%d", __FILE__, __LINE__);       \
        } else {                                                                \
            GtMemAllocHdr* _hdr = ((GtMemAllocHdr*)(block)) - 1;      \
            if (_hdr->_magic != GT_MEM_MAGIC) {                                 \
                GTERROR("[MEM] FREE | CORRUPTION/DOUBLE-FREE | %p | %s:%d",     \
                        (block), __FILE__, __LINE__);                           \
            } else {                                                            \
                const char* _tag = (_hdr->tag < GT_MEM_TAG_MAX_TAGS)            \
                    ? __gontiGetMemoryTagStringsArr()[_hdr->tag] : "INVALID";   \
                GTDEBUG("[MEM] FREE   | %p | Size: %llu | Tag: %s | %s:%d",     \
                        (block), _hdr->size, _tag, __FILE__, __LINE__);         \
                __gontiMemoryFree(block);                                       \
                (block) = 0;                                                    \
            }                                                                   \
        }                                                                       \
    } while (0)

#define gt_zeroMemory(block, size)          __gontiMemoryZero((block), (size))
#define gt_copyMemory(dest, source, size)   __gontiMemoryCopy((dest), (source), (size))
#define gt_setMemory(dest, value, size)     __gontiMemorySet((dest), (value), (size))

#ifdef __cplusplus
}
#endif

#endif