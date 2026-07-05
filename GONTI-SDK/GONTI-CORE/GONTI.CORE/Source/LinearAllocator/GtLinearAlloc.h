#ifndef GtLinearAllocH
#define GtLinearAllocH

#ifdef __cplusplus
    extern "C" {
#endif

        #include <Defines/GtDefines.inl>

        typedef struct GtLinearAlloc {
            GtU64 totalSize;
            GtU64 allocated;
            void* memory;
            GtB8 ownsMemory;
        } GtLinearAlloc;

        /* VOID */
        GTAPI void gontiLinearAllocatorCreate(GtU64 totalSize, void* memory, GtLinearAlloc* outAllocator);
        GTAPI void gontiLinearAllocatorDestroy(GtLinearAlloc* allocator);
        GTAPI void gontiLinearAllocatorFreeAll(GtLinearAlloc* allocator);

        /* VOID* */
        GTAPI void* gontiLinearAllocatorAllocate(GtLinearAlloc* allocator, GtU64 size);

#ifdef __cplusplus
    }
#endif

#endif