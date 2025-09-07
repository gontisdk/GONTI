#ifndef MEMORYSUBSYSTEM_H
#define MEMORYSUBSYSTEM_H

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../Defines/Defines.h"

        typedef enum GontiMemoryTag {
            GONTI_MEMORY_TAG_UNKOWN,
            GONTI_MEMORY_TAG_ARRAY,
            GONTI_MEMORY_TAG_ARRAYEX,
            GONTI_MEMORY_TAG_DARRAY,
            GONTI_MEMORY_TAG_DARRAYEX,
            GONTI_MEMORY_TAG_DICT,
            GONTI_MEMORY_TAG_RING_QUEUE,
            GONTI_MEMORY_TAG_BST,
            GONTI_MEMORY_TAG_STRING,
            GONTI_MEMORY_TAG_APPLICATION,
            GONTI_MEMORY_TAG_JOB,
            GONTI_MEMORY_TAG_TEXTURE,
            GONTI_MEMORY_TAG_MATERIAL_INSTANCE,
            GONTI_MEMORY_TAG_WINDOW,
            GONTI_MEMORY_TAG_RENDERER,
            GONTI_MEMORY_TAG_ENTRY, // GAME
            GONTI_MEMORY_TAG_TRANSFORM,
            GONTI_MEMORY_TAG_ENTITY,
            GONTI_MEMORY_TAG_ENTITY_NODE,
            GONTI_MEMORY_TAG_SCENE,
            GONTI_MEMORY_TAG_MATH,
            GONTI_MEMORY_TAG_VECTOR,
            GONTI_MEMORY_TAG_MATRIX,
            GONTI_MEMORY_TAG_BIGINT,

            GONTI_MEMORY_TAG_MAX_TAGS
        } GontiMemoryTag;

        struct GontiMemoryStats {
            u64 totalAllocated;
            u64 taggedAllocations[GONTI_MEMORY_TAG_MAX_TAGS];
        };
        extern struct GontiMemoryStats m_stats;

        /* CHAR* */
        KAPI char *k_getMemoryUsageStr();

        /*VOID*/
        KAPI void gontiInitializeMemory();
        KAPI void gontiShutdownMemory();
        KAPI void k_free(void *block, u64 size, GontiMemoryTag memTag);

        /* VOID* */
        KAPI void *k_allocate(u64 size, GontiMemoryTag memTag);
        KAPI void *k_zeroMemory(void *block, u64 size);
        KAPI void *k_copyMemory(void *dest, const void *source, u64 size);
        KAPI void *k_setMemory(void *dest, i32 value, u64 size);
        KAPI void *k_reallocate(void *block, u64 size);

#ifdef __cplusplus
    }
#endif

#endif