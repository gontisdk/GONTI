#include "DynamicArrayEx.h"

#ifdef DynamicArrayEx_ON

#include "../../Core/MemorySubsystem/MemorySubsystem.h"
#include "../../Utilities/Logging/Scripts/Logger.h"
#include "../../Libs/StringLib/StringLib.h"

/*
* DYNAMICARRAYEX
*/
DynamicArrayEx darrayExDefaultAllocate() {
    return darrayExAllocate(DARRAYEX_DEFAULT_SIZE);
}
DynamicArrayEx darrayExAllocate(unsigned int capacity) {
    if (!capacity) {
        capacity = DARRAYEX_DEFAULT_SIZE;
    }

    DynamicArrayEx ret;
    ret.list = k_allocate(capacity * sizeof(void*), MEMORY_TAG_DARRAYEX);
    ret.size = 0;
    ret.capacity = ret.list ? capacity : 0;

    return ret;
}

/*
* DARRAYINTEGRATOREX
*/
DArrayIteratorEx darrayExIteratorNew(DynamicArrayEx* list) {
    DArrayIteratorEx ret;

    ret.list = list;
    ret.cur_idx = 0;

    return ret;
}

/* 
* VOID* 
*/
void* darrayExIteratorNext(DArrayIteratorEx *it) {
    return it->cur_idx < it->list->size ? it->list->list[it->cur_idx++] : NULL;
}
void* darrayExRemoveAtIdx(DynamicArrayEx* list, unsigned int idx) {
    if (idx >= list->size) return NULL;

    void *ret = list->list[idx];

    for (unsigned int i = idx + 1; i < list->size; i++) {
        list->list[i - 1] = list->list[i];
    }

    list->list[list->size - 1] = NULL;
    list->size--;

    return ret;
}
void* darrayExRemoveFirst(DynamicArrayEx* list) {
    return darrayExRemoveAtIdx(list, 0);
}
void* darrayExRemoveLast(DynamicArrayEx* list) {
    return darrayExRemoveAtIdx(list, list->size - 1);
}
void* darrayExGet(DynamicArrayEx* list, unsigned int idx) {
    return (idx < list->size) ? list->list[idx] : NULL;
}

/*
* VOID
*/
void darrayExAddAtIdx(DynamicArrayEx* list, void* element, unsigned int idx) {
    if (idx > list->size) return;

    darrayExReallocate(list, 1);

    for (unsigned int i = list->size; i > idx; i--) {
        list->list[i] = list->list[i - 1];
    }

    list->list[idx] = element;
    list->size++;
}
void darrayExAddFirst(DynamicArrayEx* list, void* element) {
    darrayExAddAtIdx(list, element, 0);
}
void _darrayExAddLast(DynamicArrayEx* list, void* element) {
    darrayExAddAtIdx(list, element, list->size);
}
void darrayExReallocate(DynamicArrayEx* list, unsigned int additionalLength) {
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
            list->list = k_allocate(capacity * sizeof(void *), MEMORY_TAG_DARRAYEX);
            k_copyMemory(list->list, oldMem, capacity * sizeof(void*));

            // update pointers
            k_free(oldMem, capacity * sizeof(void*) ,MEMORY_TAG_DARRAYEX);
        }

        list->capacity = capacity;
    }
}
void darrayExSet(DynamicArrayEx* list, void* element, unsigned int idx) {
    if (idx > list->size) return;

    list->list[idx] = element;
}
void darrayExIterate(DynamicArrayEx* list, void(*visit)(void* element)) {
    for (unsigned int i = 0; i < list->size; i++) {
        visit(list->list[i]);
    }
}
void darrayExClear(DynamicArrayEx* list) {
    k_free(list->list, list->capacity * sizeof(void*), MEMORY_TAG_DARRAYEX);
    list->size = 0;
    list->capacity = DARRAYEX_DEFAULT_SIZE;
    list->list = k_allocate(DARRAYEX_DEFAULT_SIZE * sizeof(void*), MEMORY_TAG_DARRAYEX);
}
void darrayExFree(DynamicArrayEx* list) {
    k_free(list->list, list->capacity * sizeof(void*), MEMORY_TAG_DARRAYEX);
}
void darrayExFreeDeep(DynamicArrayEx* list) {
    for (unsigned int i = 0; i < list->size; i++) {
        k_free(list->list[i], sizeof(void*), MEMORY_TAG_DARRAYEX);
    }
}

#endif