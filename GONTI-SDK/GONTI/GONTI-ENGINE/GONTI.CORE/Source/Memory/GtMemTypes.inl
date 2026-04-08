#ifndef GtMemTypesINL
#define GtMemTypesINL

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../Defines/GtDefines.h"

        typedef enum GtMemTag {
            GT_MEM_TAG_UNKOWN,
            GT_MEM_TAG_ARRAY,
            GT_MEM_TAG_LINEAR_ALLOCATOR,
            GT_MEM_TAG_DARRAY,
            GT_MEM_TAG_DARRAYEX,
            GT_MEM_TAG_DICT,
            GT_MEM_TAG_RING_QUEUE,
            GT_MEM_TAG_BST,
            GT_MEM_TAG_STRING,
            GT_MEM_TAG_APPLICATION,
            GT_MEM_TAG_JOB,
            GT_MEM_TAG_TEXTURE,
            GT_MEM_TAG_MATERIAL_INSTANCE,
            GT_MEM_TAG_WINDOW,
            GT_MEM_TAG_RENDERER,
            GT_MEM_TAG_ENTRY, // GAME
            GT_MEM_TAG_TRANSFORM,
            GT_MEM_TAG_ENTITY,
            GT_MEM_TAG_ENTITY_NODE,
            GT_MEM_TAG_SCENE,
            GT_MEM_TAG_MATH,
            GT_MEM_TAG_VECTORS,
            GT_MEM_TAG_MATRICES,
            GT_MEM_TAG_BIGINT,

            GT_MEM_TAG_MAX_TAGS
        } GtMemTag;

        typedef struct GtAllocationHeader {
            GtU64 size;
            struct GtAllocationHeader* next;
            struct GtAllocationHeader* prev;
            GtMemTag tag;
            GtU32 _magic;
        } GtAllocationHeader;

#ifdef __cplusplus
    }
#endif

#endif