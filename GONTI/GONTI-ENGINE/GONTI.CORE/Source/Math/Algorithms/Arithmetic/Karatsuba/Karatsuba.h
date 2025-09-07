#ifndef KARATSUBA_H
#define KARATSUBA_H

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../../../Maths.h"

        /* INT* */
        KAPI int* gontiKaratsubaMultiplyLongIntArr(
            int* arr1, unsigned int arr1I, unsigned int arr1F,
            int* arr2, unsigned int arr2I, unsigned int arr2F,
            unsigned int* outSize
        );
        KAPI int* gontiKaratsubaMultiplyIntArr(
            int* arr1, unsigned int arr1Size,
            int* arr2, unsigned int arr2Size,
            unsigned int idxI, unsigned int idxF,
            unsigned int* outSize
        );

        /* VOID */
        KAPI void gontiKaratsubaFreeLongIntArr(
            unsigned int i1, unsigned int f1,
            unsigned int i2, unsigned int f2,
            int** outArr
        );
        KAPI void gontiKaratsubaFreeIntArr(
            unsigned int i, unsigned int f,
            int** outArr
        );

        /*DEFINE*/
        #define KARATSUBA_THRESHOLD 4

#ifdef __cplusplus
    }
#endif

#endif