#ifndef MEMORYSUBSYSTEM_H
#define MEMORYSUBSYSTEM_H

    #ifdef __cplusplus
        extern "C" {
    #endif

            #include "../../__GONTI_INCLUDES.h"
            #ifdef MemorySubsystem_ON

                typedef enum gontiMemoryTag {
                    MEMORY_TAG_UNKOWN,
                    MEMORY_TAG_ARRAY,
                    MEMORY_TAG_ARRAYEX,
                    MEMORY_TAG_DARRAY,
                    MEMORY_TAG_DARRAYEX,
                    MEMORY_TAG_DICT,
                    MEMORY_TAG_RING_QUEUE,
                    MEMORY_TAG_BST,
                    MEMORY_TAG_STRING,
                    MEMORY_TAG_APPLICATION,
                    MEMORY_TAG_JOB,
                    MEMORY_TAG_TEXTURE,
                    MEMORY_TAG_MATERIAL_INSTANCE,
                    MEMORY_TAG_RENDERER,
                    MEMORY_TAG_ENTRY, // GAME
                    MEMORY_TAG_TRANSFORM,
                    MEMORY_TAG_ENTITY,
                    MEMORY_TAG_ENTITY_NODE,
                    MEMORY_TAG_SCENE,
                    MEMORY_TAG_MATH,
                    MEMORY_TAG_VECTOR,
                    MEMORY_TAG_MATRIX,
                    MEMORY_TAG_BIGINT,

                    MEMORY_TAG_MAX_TAGS
                }gontiMemoryTag;

                struct gontiMemoryStats {
                    u64 totalAllocated;
                    u64 taggedAllocations[MEMORY_TAG_MAX_TAGS];
                };
                extern struct gontiMemoryStats m_stats;

                /* CHAR* */
                KAPI char* k_getMemoryUsageStr();

                /*VOID*/
                KAPI void initializeMemory();
                KAPI void shutdownMemory();
                KAPI void k_free(void* block, u64 size, gontiMemoryTag memTag);

                /* VOID* */
                KAPI void* k_allocate(u64 size, gontiMemoryTag memTag);
                KAPI void* k_zeroMemory(void* block, u64 size);
                KAPI void* k_copyMemory(void* dest, const void* source, u64 size);
                KAPI void* k_setMemory(void* dest, i32 value, u64 size);
                KAPI void* k_reallocate(void* block, u64 size);

            #endif
            
    #ifdef __cplusplus
        }
    #endif

#endif