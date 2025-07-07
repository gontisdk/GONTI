#ifndef DYNAMICARRAYEX_H
#define DYNAMICARRAYEX_H

    #ifdef __cplusplus
        extern "C" {
    #endif

            #include "../../__GONTI_INCLUDES.h"
            #ifdef DynamicArrayEx_ON

                #define DARRAYEX_DEFAULT_SIZE 4

                #define darrayExCreateSpecificIterator(type) void darrayEx##type##_iterate(DynamicArrayEx *list, void (*visit)(type * element)) {                                                                               \
                    for (unsigned int i = 0; i < list->size; i++)                               \
                    {                                                                           \
                        visit((type *)list->list[i]);                                           \
                    }                                                                           \
                }

                typedef struct {
                    unsigned int size;
                    unsigned int capacity;
                    void** list;
                }DynamicArrayEx;

                typedef struct DArrayIteratorEx {
                    DynamicArrayEx* list;
                    unsigned int cur_idx;
                }DArrayIteratorEx;

                /*DYNAMICARRAYEX*/
                KAPI DynamicArrayEx darrayExDefaultAllocate();
                KAPI DynamicArrayEx darrayExAllocate(unsigned int capacity);

                /*DARRAYINTEGRATOREX*/
                KAPI DArrayIteratorEx darrayExIteratorNew(DynamicArrayEx* list);

                /* VOID* */
                KAPI void* darrayExIteratorNext(DArrayIteratorEx *it);
                KAPI void* darrayExRemoveAtIdx(DynamicArrayEx* list, unsigned int idx);
                KAPI void* darrayExRemoveFirst(DynamicArrayEx* list);
                KAPI void* darrayExRemoveLast(DynamicArrayEx* list);
                KAPI void* darrayExGet(DynamicArrayEx* list, unsigned int idx);

                /*VOID*/
                KAPI void darrayExAddAtIdx(DynamicArrayEx* list, void* element, unsigned int idx);
                KAPI void darrayExAddFirst(DynamicArrayEx* list, void* element);
                KAPI void darrayExAddLast(DynamicArrayEx* list, void* element);
                KAPI void darrayExReallocate(DynamicArrayEx* list, unsigned int additionalLength);
                KAPI void darrayExSet(DynamicArrayEx* list, void* element, unsigned int idx);
                KAPI void darrayExIterate(DynamicArrayEx* list, void(*visit)(void* element));
                KAPI void darrayExClear(DynamicArrayEx* list);
                KAPI void darrayExFree(DynamicArrayEx* list);
                KAPI void darrayExFreeDeep(DynamicArrayEx* list);

            #endif

    #ifdef __cplusplus
        }
    #endif

#endif