#include "GtLinearAlloc.h"

#include "../Memory/GtMemory.h"
#include "../Logging/GtLogger.h"

/* 
* VOID 
*/
void gontiLinearAllocatorCreate(GtU64 totalSize, void* memory, GtLinearAlloc* outAllocator) {
    if (outAllocator) {
        outAllocator->totalSize = totalSize;
        outAllocator->allocated = 0;
        outAllocator->ownsMemory = memory == 0;

        if (memory) outAllocator->memory = memory;
        else outAllocator->memory = gt_allocate(totalSize, GT_MEM_TAG_LINEAR_ALLOCATOR);
    }
}
void gontiLinearAllocatorDestroy(GtLinearAlloc* allocator) {
    if (allocator) {
        allocator->allocated = 0;
        
        if (allocator->ownsMemory && allocator->memory) gt_free(allocator->memory);
        
        allocator->memory = 0;
        allocator->totalSize = 0;
        allocator->ownsMemory = GtFalse;
    }
}
void gontiLinearAllocatorFreeAll(GtLinearAlloc* allocator) {
    if (allocator && allocator->memory) {
        allocator->allocated = 0;
        gt_zeroMemory(allocator->memory, allocator->totalSize);
    }
}

/* 
* VOID* 
*/
void* gontiLinearAllocatorAllocate(GtLinearAlloc* allocator, GtU64 size) {
    if (allocator && allocator->memory) {
        if (allocator->allocated + size > allocator->totalSize) {
            GtU64 remaining = allocator->totalSize - allocator->allocated;
            GTERROR("gontiLinearAllocatorAllocate - Tried to allocate %lluB, only %lluB remaining.", size, remaining);
            return 0;
        }

        void* block = allocator->memory + allocator->allocated;
        allocator->allocated += size;

        return block;
    }

    GTERROR("gontiLinearAllocatorAllocate - Provided allocator not initialized.");
    return 0;
}