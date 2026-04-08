#ifndef GtKaratsubaH
#define GtKaratsubaH

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../../../GtMaths.h"

        /* INT* */
        GTAPI GtB32* gontiKaratsubaMultiplyLongIntArr(
            GtB32* arr1, GtU32 arr1I, GtU32 arr1F,
            GtB32* arr2, GtU32 arr2I, GtU32 arr2F,
            GtU32* outSize
        );
        GTAPI GtB32* gontiKaratsubaMultiplyIntArr(
            GtB32* arr1, GtU32 arr1Size,
            GtB32* arr2, GtU32 arr2Size,
            GtU32 idxI, GtU32 idxF,
            GtU32* outSize
        );

        /*DEFINE*/
        #define GT_KARATSUBA_THRESHOLD 4

#ifdef __cplusplus
    }
#endif

#endif