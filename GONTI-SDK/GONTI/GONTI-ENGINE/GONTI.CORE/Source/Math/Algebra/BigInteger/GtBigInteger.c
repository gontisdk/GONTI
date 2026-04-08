#include "GtBigInteger.h"

#include <stdlib.h>
#include <math.h>

#include "../../../Memory/GtMemory.h"
#include "../../../CStringTools/GtCStrTools.h"
#include "../../Algorithms/Arithmetic/Karatsuba/GtKaratsuba.h"
#include "../../../Logging/GtLogger.h"

static GtB32 DIG1[1] = {1};
static GtB32 DIG0[1] = {0};
static const GtBigint BIGINT_ZERO = {GtTrue, 1, 1, DIG0, GtFalse};
static const GtBigint BIGINT_ONE = {GtTrue, 1, 1, DIG1, GtFalse};
static const GtBigint BIGINT_NEG_ONE = {GtFalse, 1, 1, DIG1, GtFalse};

/*
* BIGINT
*/
GtBigint gontiBigintAllocate(GtU32 capacity) {
    GtBigint ret;
    ret.capacity = capacity;
    ret.noDigits = 0;
    ret.sign = GtTrue;
    ret.heapAllocated = GtTrue;
    ret.digits = gt_allocate(capacity * sizeof(GtB32), GT_MEM_TAG_BIGINT);
    return ret;
}
GtBigint gontiBigintZero() {
    GtBigint ret = gontiBigintAllocate(BIGINT_ZERO.capacity);

    ret.sign = BIGINT_ZERO.sign;
    ret.noDigits = BIGINT_ZERO.noDigits;
    gt_copyMemory(ret.digits, BIGINT_ZERO.digits, BIGINT_ZERO.noDigits * sizeof(GtB32));

    return ret;
}
GtBigint gontiBigintOne() {
    GtBigint ret = gontiBigintAllocate(BIGINT_ONE.capacity);

    ret.sign = BIGINT_ONE.sign;
    ret.noDigits = BIGINT_ONE.noDigits;
    gt_copyMemory(ret.digits, BIGINT_ONE.digits, BIGINT_ONE.noDigits * sizeof(GtB32));

    return ret;
}
GtBigint gontiBigintNegOne() {
    GtBigint ret = gontiBigintAllocate(BIGINT_NEG_ONE.capacity);

    ret.sign = BIGINT_NEG_ONE.sign;
    ret.noDigits = BIGINT_NEG_ONE.noDigits;
    gt_copyMemory(ret.digits, BIGINT_NEG_ONE.digits, BIGINT_NEG_ONE.noDigits * sizeof(GtB32));

    return ret;
}
GtBigint gontiBigintFromStr(char *str) {
    GtU32 len = gontiStringLength(str);
    GtU32 effectiveLen = len;
    GtU32 firstDigit = 0;

    GtB8 sign = GtTrue;
    if (str[firstDigit] == '-') {
        sign = GtFalse;
        effectiveLen--;
        firstDigit++;
    }

    while (str[firstDigit] == '0') {
        effectiveLen--;
        firstDigit++;
    }

    if (effectiveLen == 0) return gontiBigintZero();

    GtU32 numDigits, extra;
    gontiMathDivMod(effectiveLen, BIGINT_NO_BASE_DIGITS, &numDigits, &extra);

    if (extra) numDigits++;

    GtBigint ret = gontiBigintAllocate(numDigits);
    ret.noDigits = numDigits;
    ret.sign = sign;

    GtU32 currentDigit = 0;
    GtU32 digitArrIdx = numDigits - 1;
    char currentDigitCharIdx = extra ? extra : BIGINT_NO_BASE_DIGITS;

    for (GtU32 idx = firstDigit; idx < len; idx++) {
        char c = str[idx];

        if (c >= '0' && c <= '9') {
            currentDigit *= 10;
            currentDigit += c - '0';
            currentDigitCharIdx--;

            if (!currentDigitCharIdx) {
                ret.digits[digitArrIdx--] = currentDigit;

                currentDigit = 0;
                currentDigitCharIdx = BIGINT_NO_BASE_DIGITS;
            }
        } else {
            gontiBigintFree(&ret);
            return gontiBigintZero();
        }
    }

    return ret;
}
GtBigint gontiBigintFromInt(GtB32 i) {
    GtB8 sign = GtTrue;
    if (i < 0) {
        sign = GtFalse;
        i *= -1;
    }

    GtBigint ret = gontiBigintFromUnsignedInt(i);
    ret.sign = sign;

    return ret;
}
GtBigint gontiBigintFromLL(long long ll)
{
    GtB8 sign = GtTrue;
    if (ll < 0LL) {
        sign = GtFalse;
        ll *= -1LL;
    }

    GtBigint ret = gontiBigintFromUnsignedLL(ll);
    ret.sign = sign;

    return ret;
}
GtBigint gontiBigintFromUnsignedInt(GtU32 ui) {
    GtU32 noDigits = gontiMathNumDigits(ui, BIGINT_BASE);

    GtBigint ret = gontiBigintAllocate(noDigits);
    ret.noDigits = noDigits;
    for (GtU32 idx = 0; idx < noDigits; idx++) {
        GtU32 digit;
        gontiMathDivMod(ui, BIGINT_BASE, &ui, &digit);
        ret.digits[idx] = (GtB32)digit;
    }

    return ret;
}
GtBigint gontiBigintFromUnsignedLL(GtU64 ull) {
    GtU64 noDigits = gontiMathNumDigitsLL(ull, BIGINT_BASE);

    GtBigint ret = gontiBigintAllocate(noDigits);
    ret.noDigits = noDigits;
    for (GtU32 idx = 0; idx < noDigits; idx++) {
        GtU64 digit;

        gontiMathDivModLL(ull, BIGINT_BASE, &ull, (GtU64 *)&digit);
        ret.digits[idx] = digit;
    }

    return ret;
}
GtBigint gontiBigintAdd(GtBigint bi1, GtBigint bi2) {
    if (bi1.sign && !bi2.sign) {
        bi2.sign = GtTrue;
        return gontiBigintSubtract(bi1, bi2);
    } else if (!bi1.sign && bi2.sign) {
        bi1.sign = GtTrue;
        return gontiBigintSubtract(bi2, bi1);
    }

    GtU32 noDigits = GT_MAX(bi1.noDigits, bi2.noDigits) + 1;

    GtBigint ret = gontiBigintAllocate(noDigits);
    ret.noDigits = noDigits;

    GtB8 carry = GtFalse;
    for (GtU32 idx = 0; idx < ret.noDigits; idx++) {
        GtB32 res = carry ? 1 : 0;
        if (idx < bi1.noDigits) res += bi1.digits[idx];
        if (idx < bi2.noDigits) res += bi2.digits[idx];

        if (res >= BIGINT_BASE) {
            carry = GtTrue;
            res -= BIGINT_BASE;
        } else carry = GtFalse;

        ret.digits[idx] = res;
    }

    gontiBigintTrim(&ret);

    if (!bi1.sign && !bi2.sign) ret.sign = GtFalse;

    return ret;
}
GtBigint gontiBigintSubtract(GtBigint bi1, GtBigint bi2) {
    char comparison = gontiBigintCompare(bi1, bi2);
    if (!comparison) return BIGINT_ZERO;

    if (bi1.sign && !bi2.sign) {
        bi2.sign = GtTrue;
        return gontiBigintAdd(bi1, bi2);
    } else if (!bi1.sign && bi2.sign) {
        bi2.sign = GtFalse;
        return gontiBigintAdd(bi1, bi2);
    } else if (!bi1.sign && !bi2.sign) {
        bi1.sign = GtTrue;
        bi2.sign = GtTrue;
        return gontiBigintSubtract(bi2, bi1);
    } else {
        if (comparison < 0) {
            GtBigint res = gontiBigintSubtract(bi2, bi1);
            res.sign = !res.sign;
            return res;
        }
    }

    GtU32 noDigits = bi1.noDigits;

    GtBigint ret = gontiBigintAllocate(noDigits);
    ret.noDigits = noDigits;

    GtB8 carry = GtFalse;
    for (GtU32 idx = 0; idx < ret.noDigits; idx++) {
        GtB32 res = carry ? -1 : 0;

        if (idx < bi1.noDigits) res += bi1.digits[idx];
        if (idx < bi2.noDigits) res -= bi2.digits[idx];

        if (res < 0) {
            carry = GtTrue;
            res += BIGINT_BASE;
        } else carry = GtFalse;

        ret.digits[idx] = res;
    }

    gontiBigintTrim(&ret);
    return ret;
}
GtBigint gontiBigintMultiplyUnsignedInt(GtU32 i1, GtU32 i2) {
    return gontiBigintFromUnsignedInt(i1 * i2);
}
GtBigint gontiBigintMultiplyUnsignedLL(GtU64 ll1, GtU64 ll2) {
    return gontiBigintFromUnsignedLL(ll1 * ll2);
}
GtBigint gontiBigintMultiply(GtBigint i1, GtBigint i2) {
    GtB8 sign = !(i1.sign ^ i2.sign);

    if (!(gontiBigintCompare(i1, BIGINT_ZERO) && gontiBigintCompare(i2, BIGINT_ZERO))) return BIGINT_ZERO;

    if (i1.noDigits == 1 && i1.digits[0] == 1) return gontiBigintCopyIntArr(i2.digits, i2.noDigits, sign);
    else if (i2.noDigits == 1 && i2.digits[0] == 1) return gontiBigintCopyIntArr(i1.digits, i1.noDigits, sign);

    GtU32 minNoDigits, maxNoDigits;
    if (i1.noDigits > i2.noDigits) {
        minNoDigits = i2.noDigits;
        maxNoDigits = i1.noDigits;
    } else {
        minNoDigits = i1.noDigits;
        maxNoDigits = i2.noDigits;
    }

    GtU32 noDigits;
    GtB32* retArr;
    if (maxNoDigits < GT_KARATSUBA_THRESHOLD || minNoDigits <= sqrt(maxNoDigits)) {
        retArr = gontiKaratsubaMultiplyLongIntArr(
            i1.digits, 0, i1.noDigits,
            i2.digits, 0, i2.noDigits,
            &noDigits
        );
    } else {
        retArr = gontiKaratsubaMultiplyIntArr(
            i1.digits, i1.noDigits,
            i2.digits, i2.noDigits,
            0, maxNoDigits,
            &noDigits
        );
    }

    GtBigint ret = gontiBigintCopyIntArr(retArr, noDigits, sign);
    gontiBigintTrim(&ret);
    if (retArr) gt_free(retArr);

    return ret;
}
GtBigint gontiBigintCopyIntArr(GtB32 *arr, GtU32 n, GtB8 sign) {
    GtBigint ret = gontiBigintAllocate(n);
    ret.noDigits = n;
    ret.sign = sign;

    gt_copyMemory(ret.digits, arr, n * sizeof(GtB32));

    return ret;
}
GtBigint gontiBigintLongMultiply(GtBigint bi1, GtBigint bi2) {
    GtB8 sign = !(bi1.sign ^ bi2.sign);
    
    GtU32 noDigits;
    GtB32* retArr = gontiKaratsubaMultiplyLongIntArr(
        bi1.digits, 0, bi1.noDigits,
        bi2.digits, 0, bi2.noDigits,
        &noDigits
    );

    if (!retArr) {
        return gontiBigintZero();
    }

    GtBigint ret = gontiBigintCopyIntArr(retArr, noDigits, sign);

    gt_free(retArr);
    gontiBigintTrim(&ret);

    return ret;
}
GtBigint gontiBigintKaratsubaMultiply(GtBigint bi1, GtBigint bi2) {
    GtB8 sign = !(bi1.sign ^ bi2.sign);

    GtU32 maxNoDigits = GT_MAX(bi1.noDigits, bi2.noDigits);
    GtU32 noDigits;
    GtB32* retArr = gontiKaratsubaMultiplyIntArr(
        bi1.digits, bi1.noDigits,
        bi2.digits, bi2.noDigits,
        0, maxNoDigits,
        &noDigits
    );

    if (!retArr) return gontiBigintZero();

    GtBigint ret = gontiBigintCopyIntArr(retArr, noDigits, sign);
    gt_free(retArr);
    gontiBigintTrim(&ret);

    return ret;
}
GtBigint gontiBigintBaseDivide(GtBigint bi, GtB32 d) {
    GtB32 newSize;
    bi.digits = gontiBigintRightShiftArr(bi.digits, bi.noDigits, d, &newSize);
    bi.noDigits = (GtU32)newSize;
    return bi;
}

GtBigint gontiBigintBaseMult(GtBigint bi, GtB32 d) {
    GtB32 newSize;
    bi.digits = gontiBigintLeftShiftArr(bi.digits, bi.noDigits, d, &newSize);
    bi.noDigits = (GtU32)newSize;
    return bi;
}

/*
* CHAR*
*/
char* gontiBigintPtrToString(GtBigint* bi) {
    if (!bi->noDigits) return "0";

    GtU32 noChars = (bi->noDigits - 1) * BIGINT_NO_BASE_DIGITS;
    GtU32 numDigitsLast = gontiMathNumDigits(bi->digits[bi->noDigits - 1], 10);
    noChars += numDigitsLast;

    if (!bi->sign) noChars++;

    char *ret = gt_allocate((noChars + 1) * sizeof(char), GT_MEM_TAG_STRING);

    if (!ret) return NULL;

    GtU32 strIdx = 0;
    if (!bi->sign) ret[strIdx++] = '-';

    char offset = numDigitsLast;
    GtU32 strDigit;
    for (GtB32 digitIdx = bi->noDigits; digitIdx; digitIdx--) {
        char increment = offset;
        GtU32 digit = (GtU32)bi->digits[digitIdx - 1];

        while (offset--) {
            gontiMathDivMod(digit, 10, &digit, &strDigit);
            ret[strIdx + offset] = '0' + (char)strDigit;
        }

        offset = BIGINT_NO_BASE_DIGITS;
        strIdx += increment;
    }

    ret[noChars] = '\0';
    return ret;
}
char* gontiBigintToString(GtBigint bi) {
    return gontiBigintPtrToString(&bi);
}

/*
* CHAR
*/
char gontiBigintCompare(GtBigint bi1, GtBigint bi2) {
    if (bi1.sign && !bi2.sign) return 1;
    else if (!bi1.sign && bi2.sign) return -1;
    else if (!bi1.sign && !bi2.sign) {
        bi1.sign = GtTrue;
        bi2.sign = GtTrue;

        char comparison = gontiBigintCompare(bi1, bi2);
        return -comparison;
    }

    if (bi1.noDigits > bi2.noDigits) return 1;
    else if (bi1.noDigits < bi2.noDigits) return -1;

    GtU32 idx = bi1.noDigits;
    while (idx--) {
        if (bi1.digits[idx] > bi2.digits[idx]) return 1;
        else if (bi1.digits[idx] < bi2.digits[idx]) return -1;
    }

    return 0;
}

/*
* VOID
*/
void gontiBigintFree(GtBigint* bi) {
    if (!bi->heapAllocated) return;

    gt_free(bi->digits);
    bi->capacity = 0;
    bi->noDigits = 0;
    bi->sign = 1;
    bi->heapAllocated = GtFalse;
}
void gontiBigintFreeString(char* str) {
    if (!str) return;
    
    //GtU32 len = gontiStringLength(str);
    gt_free(str);
}
void gontiBigintTrim(GtBigint* bi) {
    while (!bi->digits[bi->noDigits - 1]) bi->noDigits--;
    GtB32* oldDigits = bi->digits;

    bi->digits = gt_allocate(bi->noDigits * sizeof(GtB32), GT_MEM_TAG_BIGINT);
    gt_copyMemory(bi->digits, oldDigits, bi->noDigits * sizeof(GtB32));
    gt_free(oldDigits);
}
void gontiBigintDebugPrint(const char* name, GtBigint* bi) {
    printf("%s: sign=%d, noDigits=%u, capacity=%u\n", 
            name, bi->sign, bi->noDigits, bi->capacity);
    printf("  Digits (base %d): ", BIGINT_BASE);
    for (GtU32 i = 0; i < bi->noDigits; i++) {
        printf("[%u]=%d ", i, bi->digits[i]);
    }
    printf("\n");
    
    GtB8 hasNegative = GtFalse;
    for (GtU32 i = 0; i < bi->noDigits; i++) {
        if (bi->digits[i] < 0) {
            GTWARN("Negative digit at index %u: %d\n", i, bi->digits[i]);
            hasNegative = GtTrue;
        }
        if (bi->digits[i] >= BIGINT_BASE) {
            GTWARN("Digit >= BASE at index %u: %d\n", i, bi->digits[i]);
        }
    }
    if (!hasNegative) {
        char* str = gontiBigintToString(*bi);
        printf("  String: %s\n", str);
        gt_free(str);
    }
}

/*
* INT*
*/
GtB32* gontiBigintRightShiftArr(GtB32* i, GtB32 n, GtB32 d, GtB32* newSize) {
    if (d < 0) return gontiBigintLeftShiftArr(i, n, -d, newSize);
    if (!d) {
        *newSize = n;
        return i;
    }
    if (d >= n) {
        gt_free(i);
        *newSize = 0;
        return NULL;
    }

    GtB32 noLeadingZeros = 0;
    GtB32 idx = n - 1;
    while (idx >= 0 && !i[idx--]) noLeadingZeros++;
    if (noLeadingZeros == n) {
        gt_free(i);
        *newSize = 0;
        return NULL;
    }

    *newSize = n - noLeadingZeros - d;
    GtB32* ret = gt_allocate(*newSize * sizeof(GtB32), GT_MEM_TAG_ARRAY);
    gt_copyMemory(ret, i + d, (n - noLeadingZeros - d) * sizeof(GtB32));
    gt_free(i);

    return ret;
}
GtB32* gontiBigintLeftShiftArr(GtB32* i, GtB32 n, GtB32 d, GtB32* newSize) {
    if (d < 0) return gontiBigintRightShiftArr(i, n, -d, newSize);
    if (!d) {
        *newSize = n;
        return i;
    }

    GtB32 noLeadingZeros = 0;
    GtB32 idx = n - 1;
    while (idx >= 0 && !i[idx--]) noLeadingZeros++;

    if (noLeadingZeros == n) {
        gt_free(i);
        *newSize = 0;
        return NULL;
    }

    *newSize = n - noLeadingZeros + d;
    GtB32* ret = gt_allocate(*newSize * sizeof(GtB32), GT_MEM_TAG_ARRAY);
    gt_copyMemory(ret + d, i, (n - noLeadingZeros) * sizeof(GtB32));
    gt_setMemory(ret, 0, d * sizeof(GtB32));
    gt_free(i);

    return ret;
}