#ifndef GT_MEM_TYPES_INL
#define GT_MEM_TYPES_INL

#ifdef __cplusplus
extern "C" {
#endif

    #include <Defines/GtDefines.inl>

    typedef enum GtMemTag {
        GT_MEM_TAG_UNKNOWN,
        GT_MEM_TAG_ARRAY,
        GT_MEM_TAG_LINEAR_ALLOC,
        GT_MEM_TAG_DARRAY,
        GT_MEM_TAG_DARRAYEX,
        GT_MEM_TAG_DICT,
        GT_MEM_TAG_RING_QUEUE,
        GT_MEM_TAG_BST,
        GT_MEM_TAG_STRING,
        GT_MEM_TAG_APPLICATION,
        GT_MEM_TAG_JOB,
        GT_MEM_TAG_TEXTURE,
        GT_MEM_TAG_MAT_INST,
        GT_MEM_TAG_WINDOW,
        GT_MEM_TAG_RENDERER,
        GT_MEM_TAG_ENTRY, // or GAME
        GT_MEM_TAG_TRANSFORM,
        GT_MEM_TAG_ENTITY,
        GT_MEM_TAG_ENTITY_NODE,
        GT_MEM_TAG_SCENE,
        GT_MEM_TAG_MATH,
        GT_MEM_TAG_VECTOR,
        GT_MEM_TAG_MATRIX,
        GT_MEM_TAG_BIGINT,

        GT_MEM_TAG_MAX_TAGS
    } GtMemTag;

    #define GT_MEM_MAGIC  0xDEADBEEFU

    typedef struct GtMemAllocHdr {
        GtU64 size;
        struct GtMemAllocHdr* next;
        struct GtMemAllocHdr* prev;
        GtU32 tag;
        GtU32 _magic;
        GtU64 _padding;
        GtU64 _alignment_pad;
    } GtMemAllocHdr;

#ifdef __cplusplus
}
#endif

#endif