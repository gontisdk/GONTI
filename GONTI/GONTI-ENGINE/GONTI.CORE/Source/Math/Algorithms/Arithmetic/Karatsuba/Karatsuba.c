#include "Karatsuba.h"

#include "../../../../Memory/Memory.h"
#include "../../../Algebra/BigInteger/BigInteger.h"

/* 
* INT* 
*/
int* gontiKaratsubaMultiplyLongIntArr(
    int* arr1, unsigned int arr1I, unsigned int arr1F,
    int* arr2, unsigned int arr2I, unsigned int arr2F,
    unsigned int* outSize
) {
    unsigned int arr1Range = arr1F - arr1I;
    unsigned int arr2Range = arr2F - arr2I;
    unsigned int noDigits = arr1Range + arr2Range;

    int* ret = k_allocate(noDigits * sizeof(int), GONTI_MEMORY_TAG_MATH);

    if (!ret) {
        *outSize = 0;
        return NULL;
    }

    k_setMemory(ret, 0, noDigits * sizeof(int) / sizeof(char));

    for (unsigned int i = 0; i < arr1Range; i++) {
        int carry = 0;

        for (unsigned int j = 0; j < arr2Range; j++) {
            bigint prod = gontiBigintMultiplyInt(arr1[arr1I + i], arr2[arr2I + j]);

            unsigned int addition = carry;

            if (prod.noDigits > 0) {
                addition += prod.digits[0];
            }

            if (addition >= BIGINT_BASE) {
                carry = 1;
                addition -= BIGINT_BASE;
            } else {
                carry = 0;
            }

            ret[i + j] += addition;

            if (ret[i + j] >= BIGINT_BASE) {
                carry++;
                ret[i + j] -= BIGINT_BASE;
            }

            if (prod.noDigits > 1) {
                carry += prod.digits[1];
            }
        }

        if (carry) {
            ret[i + arr2Range] += carry;
        }
    }

    *outSize = noDigits;
    return ret;
}
int* gontiKaratsubaMultiplyIntArr(
    int* arr1, unsigned int arr1Size,
    int* arr2, unsigned int arr2Size,
    unsigned int idxI, unsigned int idxF,
    unsigned int* outSize
) {
    if (
        idxI >= arr1Size ||
        idxI >= arr2Size ||
        idxI >= idxF
    ) {
        *outSize = 0;
        return NULL;
    }

    unsigned int range = idxF - idxI;
    unsigned int noDigits = range << 1;
    unsigned int half = ((idxF + idxI) >> 1) + (range & 1);
    unsigned int sumTermSize = half - idxI + 1;

    if (range <= KARATSUBA_THRESHOLD) {
        return gontiKaratsubaMultiplyLongIntArr(
            arr1, idxI, MIN(arr1Size, idxF),
            arr2, idxI, MIN(arr2Size, idxF),
            outSize
        );
    }

    int* ret = k_allocate(noDigits * sizeof(int), GONTI_MEMORY_TAG_MATH);
    int* arr1Sum = k_allocate(sumTermSize * sizeof(int), GONTI_MEMORY_TAG_MATH);
    int* arr2Sum = k_allocate(sumTermSize * sizeof(int), GONTI_MEMORY_TAG_MATH);

    unsigned int leftIdx = idxI;
    unsigned int rightIdx = half;

    bool c1 = false;
    bool c2 = false;

    for (int i = 0; i < sumTermSize; i++, leftIdx++, rightIdx++) {
        arr1Sum[i] = c1;
        c1 = false;

        if (leftIdx < arr1Size && leftIdx < half) arr1Sum[i] += arr1[leftIdx];
        if (rightIdx < arr1Size && rightIdx < idxF) arr1Sum[i] += arr1[rightIdx];

        if (arr1Sum[i] >= BIGINT_BASE) {
            c1 = true;
            arr1Sum[i] -= BIGINT_BASE;
        }

        arr2Sum[i] = c2;
        c2 = false;

        if (leftIdx < arr2Size && leftIdx < half) arr2Sum[i] += arr2[leftIdx];
        if (rightIdx < arr2Size && rightIdx < idxF) arr2Sum[i] += arr2[rightIdx];

        if (arr2Sum[i] >= BIGINT_BASE) {
            c2 = true;
            arr2Sum[i] -= BIGINT_BASE;
        }
    }

    unsigned int hiSize;
    unsigned int mdSize;
    unsigned int loSize;

    int* hi = gontiKaratsubaMultiplyIntArr(
        arr1, arr1Size,
        arr2, arr2Size,
        half, idxF,
        &hiSize
    );
    int* lo = gontiKaratsubaMultiplyIntArr(
        arr1, arr1Size,
        arr2, arr2Size,
        idxI, half,
        &loSize
    );
    int* md = gontiKaratsubaMultiplyIntArr(
        arr1Sum, sumTermSize,
        arr2Sum, sumTermSize,
        0, sumTermSize,
        &mdSize
    );

    int carry = 0;

    for (unsigned int i = 0; i < mdSize; i++) {
        md[i] -= ((i < hiSize) ? hi[i] : 0) + ((i < loSize) ? lo[i] : 0);

        if (carry) md[i] += carry;

        carry = 0;

        while (md[i] < 0) {
            carry--;
            md[i] += BIGINT_BASE;
        }
    }

    int loHalf = (half - idxI) << 1;
    int i = 0;

    for (; i < loHalf; i++) {
        ret[i] = (i < loSize) ? lo[i] : 0;
    }

    int hiIdx = 0;

    for (; i < noDigits; i++, hiIdx++) {
        ret[i] = (hiIdx < hiSize) ? hi[hiIdx] : 0;
    }

    carry = 0;
    
    unsigned int retIdx = half - idxI;

    for (i = 0; i < mdSize && retIdx < noDigits; i++, retIdx++) {
        ret[retIdx] += md[i] + carry;
        carry = 0;

        if (ret[retIdx] >= BIGINT_BASE) {
            carry = 1;
            ret[retIdx] -= BIGINT_BASE;
        }
    }

    if (carry && retIdx < noDigits) {
        ret[retIdx]++;
    }

    k_free(arr1Sum, sumTermSize * sizeof(int), GONTI_MEMORY_TAG_MATH);
    k_free(arr2Sum, sumTermSize * sizeof(int), GONTI_MEMORY_TAG_MATH);
    gontiKaratsubaFreeIntArr(half, idxF, &hi);
    gontiKaratsubaFreeIntArr(idxI, half, &lo);
    gontiKaratsubaFreeIntArr(0, sumTermSize, &md);

    *outSize = noDigits;
    return ret;
}

/*
* VOID
*/
void gontiKaratsubaFreeLongIntArr(
    unsigned int i1, unsigned int f1,
    unsigned int i2, unsigned int f2,
    int** outArr
) {
    unsigned int range1 = f1 - i1;
    unsigned int range2 = f2 - i2;
    unsigned int noDigits = range1 + range2;

    if (*outArr) {
        k_free(*outArr, noDigits * sizeof(int), GONTI_MEMORY_TAG_MATH);
        *outArr = 0;
    }
}
void gontiKaratsubaFreeIntArr(
    unsigned int i, unsigned int f,
    int** outArr
) {
    unsigned int range = f - i;
    unsigned int noDigits = range << 1;

    if (*outArr) {
        k_free(*outArr, noDigits * sizeof(int), GONTI_MEMORY_TAG_MATH);
        *outArr = 0;
    }
}