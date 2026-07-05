#include <DynamicArrayEx/GtDArrayEx.h>

#include <GONTI-CORE/GONTI.CORE/Source/Memory/GtMemory.h>
#include <GONTI-CORE/GONTI.CORE/Source/Logging/GtLogger.h>
#include <GONTI-CORE/GONTI.CORE/Source/CStringTools/GtCStrTools.h>

/*
* DYNAMICARRAYEX
*/
GtDArrayEx gontiDarrayExDefaultAllocate(void) {
    return gontiDarrayExAllocate(GT_DARRAYEX_DEFAULT_SIZE);
}
GtDArrayEx gontiDarrayExAllocate(GtU32 capacity) {
    if (!capacity) {
        capacity = GT_DARRAYEX_DEFAULT_SIZE;
    }

    GtDArrayEx ret;
    ret.list = gt_allocate(capacity * sizeof(void*), GT_MEM_TAG_DARRAYEX);
    ret.size = 0;
    ret.capacity = ret.list ? capacity : 0;

    return ret;
}

/*
* DARRAYINTEGRATOREX
*/
GtDArrayExIterator gontiDarrayExIteratorNew(GtDArrayEx* list) {
    GtDArrayExIterator ret;

    ret.list = list;
    ret.cur_idx = 0;

    return ret;
}

/* 
* VOID* 
*/
void* gontiDarrayExIteratorNext(GtDArrayExIterator *it) {
    return it->cur_idx < it->list->size ? it->list->list[it->cur_idx++] : NULL;
}
void* gontiDarrayExRemoveAtIdx(GtDArrayEx* list, GtU32 idx) {
    if (idx >= list->size) return NULL;

    void *ret = list->list[idx];

    for (GtU32 i = idx + 1; i < list->size; i++) {
        list->list[i - 1] = list->list[i];
    }

    list->list[list->size - 1] = NULL;
    list->size--;

    return ret;
}
void* gontiDarrayExRemoveFirst(GtDArrayEx* list) {
    return gontiDarrayExRemoveAtIdx(list, 0);
}
void* gontiDarrayExRemoveLast(GtDArrayEx* list) {
    return gontiDarrayExRemoveAtIdx(list, list->size - 1);
}
void* gontiDarrayExGet(GtDArrayEx* list, GtU32 idx) {
    return (idx < list->size) ? list->list[idx] : NULL;
}

/*
* VOID
*/
void gontiDarrayExAddAtIdx(GtDArrayEx* list, void* element, GtU32 idx) {
    if (idx > list->size) return;

    gontiDarrayExReallocate(list, 1);

    for (GtU32 i = list->size; i > idx; i--) {
        list->list[i] = list->list[i - 1];
    }

    list->list[idx] = element;
    list->size++;
}
void gontiDarrayExAddFirst(GtDArrayEx* list, void* element) {
    gontiDarrayExAddAtIdx(list, element, 0);
}
void _gontiDarrayExAddLast(GtDArrayEx* list, void* element) {
    gontiDarrayExAddAtIdx(list, element, list->size);
}
void gontiDarrayExReallocate(GtDArrayEx* list, GtU32 additionalLength) {
    GtU32 newSize = list->size + additionalLength;

    if (newSize > list->capacity) {
        GtU32 capacity = list->capacity;
        if (!capacity) {
            capacity = 1;
        }

        while (capacity < newSize) {
            capacity <<= 1;
        }

        void **oldMem = list->list;
        list->list = gt_reallocate(list->list, capacity * sizeof(void*));

        if (!list->list) {
            list->list = gt_allocate(capacity * sizeof(void *), GT_MEM_TAG_DARRAYEX);
            gt_copyMemory(list->list, oldMem, capacity * sizeof(void*));

            gt_free(oldMem);
        }

        list->capacity = capacity;
    }
}
void gontiDarrayExSet(GtDArrayEx* list, void* element, GtU32 idx) {
    if (idx > list->size) return;

    list->list[idx] = element;
}
void gontiDarrayExIterate(GtDArrayEx* list, void(*visit)(void* element)) {
    for (GtU32 i = 0; i < list->size; i++) {
        visit(list->list[i]);
    }
}
void gontiDarrayExClear(GtDArrayEx* list) {
    gt_free(list->list);
    list->size = 0;
    list->capacity = GT_DARRAYEX_DEFAULT_SIZE;
    list->list = gt_allocate(GT_DARRAYEX_DEFAULT_SIZE * sizeof(void*), GT_MEM_TAG_DARRAYEX);
}
void gontiDarrayExFree(GtDArrayEx* list) {
    gt_free(list->list);
}
void gontiDarrayExFreeDeep(GtDArrayEx* list) {
    for (GtU32 i = 0; i < list->size; i++) {
        gt_free(list->list[i]);
    }
}