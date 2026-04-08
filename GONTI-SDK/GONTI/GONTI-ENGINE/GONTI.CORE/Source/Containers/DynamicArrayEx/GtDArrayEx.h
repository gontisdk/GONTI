#ifndef GtDArrayExH
#define GtDArrayExH

    #ifdef __cplusplus
        extern "C" {
    #endif

            #include "../../Defines/GtDefines.h"

            #define GT_DARRAYEX_DEFAULT_SIZE 4

            #define gontiDarrayExCreateSpecificIterator(type) void gontiDarrayEx##type##_iterate(GtDArrayEx *list, void (*visit)(type * element)) {                                                                               \
                for (GtU32 i = 0; i < list->size; i++)                               \
                {                                                                           \
                    visit((type *)list->list[i]);                                           \
                }                                                                           \
            }

            typedef struct {
                GtU32 size;
                GtU32 capacity;
                void** list;
            }GtDArrayEx;

            typedef struct GtDArrayExIterator {
                GtDArrayEx* list;
                GtU32 cur_idx;
            }GtDArrayExIterator;

            /*DYNAMICARRAYEX*/
            GTAPI GtDArrayEx gontiDarrayExDefaultAllocate();
            GTAPI GtDArrayEx gontiDarrayExAllocate(GtU32 capacity);

            /*DARRAYINTEGRATOREX*/
            GTAPI GtDArrayExIterator gontiDarrayExIteratorNew(GtDArrayEx* list);

            /* VOID* */
            GTAPI void* gontiDarrayExIteratorNext(GtDArrayExIterator *it);
            GTAPI void* gontiDarrayExRemoveAtIdx(GtDArrayEx* list, GtU32 idx);
            GTAPI void* gontiDarrayExRemoveFirst(GtDArrayEx* list);
            GTAPI void* gontiDarrayExRemoveLast(GtDArrayEx* list);
            GTAPI void* gontiDarrayExGet(GtDArrayEx* list, GtU32 idx);

            /*VOID*/
            GTAPI void gontiDarrayExAddAtIdx(GtDArrayEx* list, void* element, GtU32 idx);
            GTAPI void gontiDarrayExAddFirst(GtDArrayEx* list, void* element);
            GTAPI void gontiDarrayExAddLast(GtDArrayEx* list, void* element);
            GTAPI void gontiDarrayExReallocate(GtDArrayEx* list, GtU32 additionalLength);
            GTAPI void gontiDarrayExSet(GtDArrayEx* list, void* element, GtU32 idx);
            GTAPI void gontiDarrayExIterate(GtDArrayEx* list, void(*visit)(void* element));
            GTAPI void gontiDarrayExClear(GtDArrayEx* list);
            GTAPI void gontiDarrayExFree(GtDArrayEx* list);
            GTAPI void gontiDarrayExFreeDeep(GtDArrayEx* list);

    #ifdef __cplusplus
        }
    #endif

#endif