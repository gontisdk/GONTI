#include <Algorithms/Arithmetic/Karatsuba/GtKaratsuba.h>

#include <stdlib.h>
#include <GONTI-CORE/GONTI.CORE/Source/Memory/GtMemory.h>
#include <Algebra/BigInteger/GtBigInteger.h>

/* 
* INT* 
*/
GtB32* gontiKaratsubaMultiplyLongIntArr(
    GtB32* arr1, GtU32 arr1I, GtU32 arr1F,
    GtB32* arr2, GtU32 arr2I, GtU32 arr2F,
    GtU32* outSize
) {
    GtU32 i1range = arr1F - arr1I;
    GtU32 i2range = arr2F - arr2I;
    GtU32 noDigits = i1range + i2range;

    GtB32* ret = gt_allocate(noDigits * sizeof(GtB32), GT_MEM_TAG_ARRAY);
    if (!ret) {
        *outSize = 0;
        return NULL;
    }

    for (GtU32 i = 0; i < noDigits; i++) ret[i] = 0;
    for (GtU32 i = 0; i < i1range; i++) {
        GtB32 carry = 0;

        for (GtU32 j = 0; j < i2range; j++) {
            GtBigint prod = gontiBigintMultiplyUnsignedLL((long long)arr1[arr1I + i], (long long)arr2[arr2I + j]);

            GtU32 addition = 0;
            if (prod.noDigits > 0) addition = carry + prod.digits[0];
            else addition = carry;

            if (addition >= BIGINT_BASE) {
                carry = 1;
                addition -= BIGINT_BASE;
            } else carry = 0;

            ret[i + j] += addition;
            if (ret[i + j] >= BIGINT_BASE) {
                carry++;
                ret[i + j] -= BIGINT_BASE;
            }

            GtU32 factor = 1;
            for (GtB32 k = 1; k < prod.noDigits; k++) {
                carry += prod.digits[k] * factor;
                factor *= BIGINT_BASE;
            }
        }

        if (carry) ret[i + i2range] += carry;
    }

    *outSize = noDigits;
    return ret;
}

GtB32* gontiKaratsubaMultiplyIntArr(
    GtB32* arr1, GtU32 arr1Size,
    GtB32* arr2, GtU32 arr2Size,
    GtU32 idxI, GtU32 idxF,
    GtU32* outSize
) {
    if (idxI >= arr1Size || idxI >= arr2Size || idxI >= idxF) {
        *outSize = 0;
        return NULL;
    }

    GtU32 range = idxF - idxI;
    GtU32 noDigits = range << 1;
    GtU32 half = ((idxF + idxI) >> 1) + (range & 1);
    GtU32 sumTermSize = half - idxI + 1;

    if (range <= GT_KARATSUBA_THRESHOLD) 
    return gontiKaratsubaMultiplyLongIntArr(
        arr1, idxI, GT_MIN(arr1Size, idxF), 
        arr2, idxI, GT_MIN(arr2Size, idxF),
        outSize
    );

    GtB32* ret = gt_allocate(noDigits * sizeof(GtB32), GT_MEM_TAG_ARRAY);
    GtB32* i1sum = gt_allocate(sumTermSize * sizeof(GtB32), GT_MEM_TAG_ARRAY);
    GtB32* i2sum = gt_allocate(sumTermSize * sizeof(GtB32), GT_MEM_TAG_ARRAY);

    GtU32 leftIdx = idxI;
    GtU32 rightIdx = half;

    GtB8 c1 = GtFalse;
    GtB8 c2 = GtFalse;

    for (GtB32 i = 0; i < sumTermSize; i++, leftIdx++, rightIdx++) {
        i1sum[i] = c1;
        c1 = GtFalse;

        if (leftIdx < arr1Size && leftIdx < half) i1sum[i] += arr1[leftIdx];
        if (rightIdx < arr1Size && rightIdx < idxF) i1sum[i] += arr1[rightIdx];

        i2sum[i] = c2;
        c2 = GtFalse;

        if (leftIdx < arr2Size && leftIdx < half) i2sum[i] += arr2[leftIdx];
        if (rightIdx < arr2Size && rightIdx < idxF) i2sum[i] += arr2[rightIdx];

        if (i1sum[i] >= BIGINT_BASE) {
            c1 = GtTrue;
            i1sum[i] -= BIGINT_BASE;
        }

        if (i2sum[i] >= BIGINT_BASE) {
            c2 = GtTrue;
            i2sum[i] -= BIGINT_BASE;
        }
    }

    GtU32 hiSize, loSize, mdSize;
    GtB32* hi = gontiKaratsubaMultiplyIntArr(arr1, arr1Size, arr2, arr2Size, half, idxF, &hiSize);
    GtB32* lo = gontiKaratsubaMultiplyIntArr(arr1, arr1Size, arr2, arr2Size, idxI, half, &loSize);
    GtB32* md = gontiKaratsubaMultiplyIntArr(i1sum, sumTermSize, i2sum, sumTermSize, 0, sumTermSize, &mdSize);

    GtB32 carry = 0;
    for (GtU32 i = 0; i < mdSize; i++) {
        md[i] -= ((i < hiSize) ? hi[i] : 0) + ((i < loSize) ? lo[i] : 0);
        if (carry) md[i] += carry;

        carry = 0;
        while (md[i] < 0) {
            carry--;
            md[i] += BIGINT_BASE;
        }
    }

    GtB32 loHalf = (half - idxI) << 1;
    GtB32 i = 0;

    for (; i < loHalf; i++) ret[i] = (i < loSize) ? lo[i] : 0;

    GtB32 hiIdx = 0;
    for (; i < noDigits; i++, hiIdx++) ret[i] = (hiIdx < hiSize) ? hi[hiIdx] : 0;

    carry = GtFalse;
    GtU32 retIdx = half - idxI;

    for (i = 0; i < mdSize && retIdx < noDigits; i++, retIdx++) {
        ret[retIdx] += md[i] + carry;

        carry = GtFalse;
        if (ret[retIdx] >= BIGINT_BASE) {
            carry = GtTrue;
            ret[retIdx] -= BIGINT_BASE;
        }
    }

    if (carry && retIdx < noDigits) ret[retIdx]++;

    gt_free(lo);
    gt_free(md);
    gt_free(hi);
    gt_free(i1sum);
    gt_free(i2sum);

    *outSize = noDigits;
    return ret;
}