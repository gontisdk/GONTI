#ifndef GtMemoryH
#define GtMemoryH

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../Defines/GtDefines.h"
        #include "../Logging/GtLogger.h"
        #include "GtMemTypes.inl"

        #define gt_zeroMemory(block, size) __gontiMemoryZero(block, size)
        #define gt_copyMemory(dest, source, size) __gontiMemoryCopy(dest, source, size)
        #define gt_setMemory(dest, value, size) __gontiMemorySet(dest, value, size)

        #define gt_allocate(size, memTag) ({ \
            void* _gt_allocate_block = __gontiMemoryAllocate(size, memTag); \
            if (_gt_allocate_block) { \
                const char* _gt_allocate_tagName = (memTag < GT_MEM_TAG_MAX_TAGS) ? __gontiGetMemoryTagStringsArr()[memTag] : "INVALID"; \
                GTDEBUG("[MEM] ALLOC | %p | Size: %llu | Tag: %s | File: %s:%d", _gt_allocate_block, (GtU64)(size), _gt_allocate_tagName, __FILE__, __LINE__); \
            } else { \
                GTERROR("[MEM] ALLOC FAILED | Size: %llu | Tag: %d | File: %s:%d", (GtU64)(size), memTag, __FILE__, __LINE__); \
            } \
            _gt_allocate_block; \
        })
        #define gt_reallocate(block, newSize) ({ \
            GTDEBUG("[MEM] REALLOC_REQ | %p | NewSize: %llu | File: %s:%d", block, newSize, __FILE__, __LINE__);\
            void* _gt_reallocate_newBlock = __gontiMemoryReallocate(block, newSize); \
            GTDEBUG("[MEM] REALLOC_RES | Old: %p -> New: %p", block, _gt_reallocate_newBlock); \
            _gt_reallocate_newBlock; \
        })
        #define gt_free(block) do { \
            void* _block_ptr = (block); \
            if (_block_ptr == 0) { \
                GTERROR("[MEM] FREE | NULL PTR SKIPPED | File: %s:%d", __FILE__, __LINE__); \
            } else { \
                GtAllocationHeader* _gt_free_header = ((GtAllocationHeader*)block) - 1; \
                if (_gt_free_header->_magic != 0xDEADBEEF) { \
                    GTERROR("Memory corruption/double free at %p! (Called from %s:%d)", block, __FILE__, __LINE__); \
                } else {\
                    GtU64 _gt_free_size = _gt_free_header->size; \
                    GtMemTag _gt_free_tag = _gt_free_header->tag; \
                    const char* _gt_free_tagName = (_gt_free_tag < GT_MEM_TAG_MAX_TAGS) ? __gontiGetMemoryTagStringsArr()[_gt_free_tag] : "INVALID"; \
                    GTDEBUG("[MEM] FREE  | %p | Size: %llu | Tag: %s | File: %s:%d", block, _gt_free_size, _gt_free_tagName, __FILE__, __LINE__); \
                    __gontiMemoryFree(_block_ptr); \
                    (block) = 0; \
                } \
            } \
        } while(0)

        /* B8 */
        GTAPI GtB8 gontiMemoryCheckLeaks();
        
        /* CHAR* */
        GTAPI char* gontiGetMemoryUsageStr();

        /* U64 */
        GTAPI GtU64 gontiMemoryGetAllocCount();
        
        /*VOID*/
        GTAPI void gontiMemoryInitialize(GtU64* memoryRequirement, void* state);
        GTAPI void gontiMemoryShutdown(void* state);
        GTAPI void __gontiMemoryFree(void* block);
        
        /* CONST CHAR** */
        GTAPI const char** __gontiGetMemoryTagStringsArr();

        /* VOID* */
        GTAPI void* __gontiMemoryAllocate(GtU64 size, GtMemTag memTag);
        GTAPI void* __gontiMemoryZero(void* block, GtU64 size);
        GTAPI void* __gontiMemoryCopy(void* dest, const void* source, GtU64 size);
        GTAPI void* __gontiMemorySet(void* dest, GtI32 value, GtU64 size);
        GTAPI void* __gontiMemoryReallocate(void* block, GtU64 newSize);

#ifdef __cplusplus
    }
#endif

#endif