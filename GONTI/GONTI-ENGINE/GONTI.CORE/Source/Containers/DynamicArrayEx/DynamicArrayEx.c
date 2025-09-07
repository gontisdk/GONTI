#include "DynamicArrayEx.h"

#include "../../Memory/Memory.h"
#include "../../Logging/Logger.h"
#include "../../String/String.h"

/*
* DYNAMICARRAYEX
*/
GontiDynamicArrayEx gontiDarrayExDefaultAllocate() {
    return gontiDarrayExAllocate(DARRAYEX_DEFAULT_SIZE);
}
GontiDynamicArrayEx gontiDarrayExAllocate(unsigned int capacity) {
    if (!capacity) {
        capacity = DARRAYEX_DEFAULT_SIZE;
    }

    GontiDynamicArrayEx ret;
    ret.list = k_allocate(capacity * sizeof(void*), GONTI_MEMORY_TAG_DARRAYEX);
    ret.size = 0;
    ret.capacity = ret.list ? capacity : 0;

    return ret;
}

/*
* DARRAYINTEGRATOREX
*/
GontiDArrayIteratorEx gontiDarrayExIteratorNew(GontiDynamicArrayEx* list) {
    GontiDArrayIteratorEx ret;

    ret.list = list;
    ret.cur_idx = 0;

    return ret;
}

/* 
* VOID* 
*/
void* gontiDarrayExIteratorNext(GontiDArrayIteratorEx *it) {
    return it->cur_idx < it->list->size ? it->list->list[it->cur_idx++] : NULL;
}
void* gontiDarrayExRemoveAtIdx(GontiDynamicArrayEx* list, unsigned int idx) {
    if (idx >= list->size) return NULL;

    void *ret = list->list[idx];

    for (unsigned int i = idx + 1; i < list->size; i++) {
        list->list[i - 1] = list->list[i];
    }

    list->list[list->size - 1] = NULL;
    list->size--;

    return ret;
}
void* gontiDarrayExRemoveFirst(GontiDynamicArrayEx* list) {
    return gontiDarrayExRemoveAtIdx(list, 0);
}
void* gontiDarrayExRemoveLast(GontiDynamicArrayEx* list) {
    return gontiDarrayExRemoveAtIdx(list, list->size - 1);
}
void* gontiDarrayExGet(GontiDynamicArrayEx* list, unsigned int idx) {
    return (idx < list->size) ? list->list[idx] : NULL;
}

/*
* VOID
*/
void gontiDarrayExAddAtIdx(GontiDynamicArrayEx* list, void* element, unsigned int idx) {
    if (idx > list->size) return;

    gontiDarrayExReallocate(list, 1);

    for (unsigned int i = list->size; i > idx; i--) {
        list->list[i] = list->list[i - 1];
    }

    list->list[idx] = element;
    list->size++;
}
void gontiDarrayExAddFirst(GontiDynamicArrayEx* list, void* element) {
    gontiDarrayExAddAtIdx(list, element, 0);
}
void _gontiDarrayExAddLast(GontiDynamicArrayEx* list, void* element) {
    gontiDarrayExAddAtIdx(list, element, list->size);
}
void gontiDarrayExReallocate(GontiDynamicArrayEx* list, unsigned int additionalLength) {
    unsigned int newSize = list->size + additionalLength;

    if (newSize > list->capacity) {
        unsigned int capacity = list->capacity;
        if (!capacity) {
            capacity = 1;
        }

        while (capacity < newSize) {
            capacity <<= 1;
        }

        void **oldMem = list->list;
        list->list = k_reallocate(list->list, capacity * sizeof(void*));

        if (!list->list) {
            // allocate in new location
            list->list = k_allocate(capacity * sizeof(void *), GONTI_MEMORY_TAG_DARRAYEX);
            k_copyMemory(list->list, oldMem, capacity * sizeof(void*));

            // update pointers
            k_free(oldMem, capacity * sizeof(void*) ,GONTI_MEMORY_TAG_DARRAYEX);
        }

        list->capacity = capacity;
    }
}
void gontiDarrayExSet(GontiDynamicArrayEx* list, void* element, unsigned int idx) {
    if (idx > list->size) return;

    list->list[idx] = element;
}
void gontiDarrayExIterate(GontiDynamicArrayEx* list, void(*visit)(void* element)) {
    for (unsigned int i = 0; i < list->size; i++) {
        visit(list->list[i]);
    }
}
void gontiDarrayExClear(GontiDynamicArrayEx* list) {
    k_free(list->list, list->capacity * sizeof(void*), GONTI_MEMORY_TAG_DARRAYEX);
    list->size = 0;
    list->capacity = DARRAYEX_DEFAULT_SIZE;
    list->list = k_allocate(DARRAYEX_DEFAULT_SIZE * sizeof(void*), GONTI_MEMORY_TAG_DARRAYEX);
}
void gontiDarrayExFree(GontiDynamicArrayEx* list) {
    k_free(list->list, list->capacity * sizeof(void*), GONTI_MEMORY_TAG_DARRAYEX);
}
void gontiDarrayExFreeDeep(GontiDynamicArrayEx* list) {
    for (unsigned int i = 0; i < list->size; i++) {
        k_free(list->list[i], sizeof(void*), GONTI_MEMORY_TAG_DARRAYEX);
    }
}