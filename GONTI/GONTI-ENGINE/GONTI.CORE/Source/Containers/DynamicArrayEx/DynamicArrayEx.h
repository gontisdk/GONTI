#ifndef DYNAMICARRAYEX_H
#define DYNAMICARRAYEX_H

    #ifdef __cplusplus
        extern "C" {
    #endif

            #include "../../Defines/Defines.h"

            #define DARRAYEX_DEFAULT_SIZE 4

            #define gontiDarrayExCreateSpecificIterator(type) void gontiDarrayEx##type##_iterate(GontiDynamicArrayEx *list, void (*visit)(type * element)) {                                                                               \
                for (unsigned int i = 0; i < list->size; i++)                               \
                {                                                                           \
                    visit((type *)list->list[i]);                                           \
                }                                                                           \
            }

            typedef struct {
                unsigned int size;
                unsigned int capacity;
                void** list;
            }GontiDynamicArrayEx;

            typedef struct GontiDArrayIteratorEx {
                GontiDynamicArrayEx* list;
                unsigned int cur_idx;
            }GontiDArrayIteratorEx;

            /*DYNAMICARRAYEX*/
            KAPI GontiDynamicArrayEx gontiDarrayExDefaultAllocate();
            KAPI GontiDynamicArrayEx gontiDarrayExAllocate(unsigned int capacity);

            /*DARRAYINTEGRATOREX*/
            KAPI GontiDArrayIteratorEx gontiDarrayExIteratorNew(GontiDynamicArrayEx* list);

            /* VOID* */
            KAPI void* gontiDarrayExIteratorNext(GontiDArrayIteratorEx *it);
            KAPI void* gontiDarrayExRemoveAtIdx(GontiDynamicArrayEx* list, unsigned int idx);
            KAPI void* gontiDarrayExRemoveFirst(GontiDynamicArrayEx* list);
            KAPI void* gontiDarrayExRemoveLast(GontiDynamicArrayEx* list);
            KAPI void* gontiDarrayExGet(GontiDynamicArrayEx* list, unsigned int idx);

            /*VOID*/
            KAPI void gontiDarrayExAddAtIdx(GontiDynamicArrayEx* list, void* element, unsigned int idx);
            KAPI void gontiDarrayExAddFirst(GontiDynamicArrayEx* list, void* element);
            KAPI void gontiDarrayExAddLast(GontiDynamicArrayEx* list, void* element);
            KAPI void gontiDarrayExReallocate(GontiDynamicArrayEx* list, unsigned int additionalLength);
            KAPI void gontiDarrayExSet(GontiDynamicArrayEx* list, void* element, unsigned int idx);
            KAPI void gontiDarrayExIterate(GontiDynamicArrayEx* list, void(*visit)(void* element));
            KAPI void gontiDarrayExClear(GontiDynamicArrayEx* list);
            KAPI void gontiDarrayExFree(GontiDynamicArrayEx* list);
            KAPI void gontiDarrayExFreeDeep(GontiDynamicArrayEx* list);

    #ifdef __cplusplus
        }
    #endif

#endif