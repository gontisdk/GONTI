#include "BigInteger.h"

#include <stdlib.h>
#include <math.h>

#include "../../../Memory/Memory.h"
#include "../../../String/String.h"
#include "../../Algorithms/Arithmetic/Karatsuba/Karatsuba.h"

static int arrOne[1] = {1};

const bigint BIGINT_ZERO = { true, 0, 0, NULL };
const bigint BIGINT_ONE = { true, 1, 1, arrOne };
const bigint BIGINT_NEG_ONE = { false, 1, 1, arrOne };

/*
* BIGINT
*/
bigint gontiBigintAllocate(unsigned int capacity) {
    bigint ret;

    ret.capacity = capacity;
    ret.noDigits = 0;
    ret.sign = true;
    ret.digits = k_allocate(capacity * sizeof(unsigned int), GONTI_MEMORY_TAG_BIGINT);

    return ret;
}
bigint gontiBigintStrToBigint(char* str) {
    unsigned int len = gontiStringLength(str);
    unsigned int effectiveLen = len;
    unsigned int firstDigit = 0;

    bool sign = true;
    if (str[firstDigit] == '-') {
        sign = false;
        effectiveLen--;
        firstDigit++;
    }

    while (str[firstDigit] == '0') {
        effectiveLen--;
        firstDigit++;
    }

    if (effectiveLen == 0) return BIGINT_ZERO;

    unsigned int noDigits;
    unsigned int extra;

    gonti_math.divMod(effectiveLen, BIGINT_NO_BASE_DIGITS, &noDigits, &extra);

    if (extra) noDigits++;

    bigint ret = gontiBigintAllocate(noDigits);
    ret.noDigits = noDigits;
    ret.sign = sign;

    unsigned int currentDigit = 0;
    char currentDigitCharIdx = extra ? extra : BIGINT_NO_BASE_DIGITS;
    unsigned int digitsArrIdx = noDigits - 1;

    for (unsigned int i = firstDigit; i < len; i++) {
        char c = str[i];

        if (c >= '0' && c <= '9') {
            currentDigit *= 10;
            currentDigit += c - '0';
            currentDigitCharIdx--;

            if (!currentDigitCharIdx) {
                ret.digits[digitsArrIdx--] = currentDigit;

                currentDigit = 0;
                currentDigitCharIdx = BIGINT_NO_BASE_DIGITS;
            }
        } else {
            gontiBigintFree(&ret);
            return BIGINT_ZERO;
        }
    }

    return ret;
}
bigint gontiBigintNewBigint(int i) {
    bool sign = true;

    if (i < 0) {
        sign = false;
        i *= -1;
    }

    bigint ret = gontiBigintNewPositiveBigint(i);
    ret.sign = sign;

    return ret;
}
bigint gontiBigintNewBigintLL(long long ll) {
    bool sign = true;

    if (ll < 0LL) {
        sign = false;
        ll *= -1LL;
    }

    bigint ret = gontiBigintNewPositiveBigintLL(ll);
    ret.sign = sign;

    return ret;
}
bigint gontiBigintNewPositiveBigint(unsigned int i) {
    unsigned int noDigits = gonti_math.numDigits(1, BIGINT_BASE);

    bigint ret = gontiBigintAllocate(noDigits);
    ret.noDigits = noDigits;

    for (unsigned int idx = 0; idx < noDigits; idx++) {
        gonti_math.divMod(i, BIGINT_BASE, &i, (unsigned int*)&ret.digits[idx]);
    }

    return ret;
}
bigint gontiBigintNewPositiveBigintLL(unsigned long long ll) {
    unsigned long long noDigits = gonti_math.numDigitsLL(1, BIGINT_BASE);

    bigint ret = gontiBigintAllocate(noDigits);
    ret.noDigits = noDigits;

    for (unsigned int i = 0; i < noDigits; i++) {
        gonti_math.divModLL(ll, BIGINT_BASE, &ll, (unsigned long long*)&ret.digits[i]);
    }

    return ret;
}
bigint gontiBigintAdd(bigint bi1, bigint bi2) {
    if (bi1.sign && !bi2.sign) {
        bi2.sign = true;
        return gontiBigintSubtract(bi1, bi2);
    } else if (!bi1.sign && bi2.sign) {
        bi1.sign = true;
        return gontiBigintSubtract(bi2, bi1);
    }

    unsigned int noDigits = MAX(bi1.noDigits, bi2.noDigits) + 1;

    bigint ret = gontiBigintAllocate(noDigits);
    ret.noDigits = noDigits;

    bool carry = false;

    for (unsigned int i = 0; i < ret.noDigits; i++) {
        int res = carry ? 1 : 0;

        if (i < bi1.noDigits) res += bi1.digits[i];
        if (i < bi2.noDigits) res += bi2.digits[i];

        if (res >= BIGINT_BASE) {
            carry = true;
            res -= BIGINT_BASE;
        } else {
            carry = false;
        }

        ret.digits[i] = res;
    }

    gontiBigintTrim(&ret);

    if (!bi1.sign && !bi2.sign) ret.sign = false;

    return ret;
}
bigint gontiBigintSubtract(bigint bi1, bigint bi2) {
    char comparsion = gontiBigintCompare(bi1, bi2);

    if (!comparsion) return BIGINT_ZERO;

    if (bi1.sign && !bi2.sign) {
        bi2.sign = true;

        return gontiBigintAdd(bi1, bi2);
    } else if (!bi1.sign && bi2.sign) {
        bi2.sign = false;

        return gontiBigintAdd(bi1, bi2);
    } else if (!bi1.sign && !bi2.sign) {
        bi1.sign = true;
        bi2.sign = true;

        return gontiBigintSubtract(bi2, bi1);
    } else {
        if (comparsion < 0) {
            bigint res = gontiBigintSubtract(bi2, bi1);
            res.sign = !res.sign;

            return res;
        }
    }

    unsigned int noDigits = bi1.noDigits;

    bigint ret = gontiBigintAllocate(noDigits);
    ret.noDigits = noDigits;

    bool carry = false;

    for (unsigned int i = 0; i < ret.noDigits; i++) {
        int res = carry ? -1 : 0;

        if (i < bi1.noDigits) res += bi1.digits[i];
        if (i < bi2.noDigits) res -= bi2.digits[i];

        if (res < 0) {
            carry = true;
            res += BIGINT_BASE;
        } else {
            carry = false;
        }

        ret.digits[i] = res;
    }

    gontiBigintTrim(&ret);

    return ret;
}
bigint gontiBigintMultiplyInt(unsigned int i1, unsigned int i2) {
    return gontiBigintNewPositiveBigint(i1 * i2);
}
bigint gontiBigintMultiplyLL(unsigned long long ll1, unsigned long long ll2) {
    return gontiBigintNewPositiveBigintLL(ll1 * ll2);
}
bigint gontiBigintMultiply(bigint bi1, bigint bi2) {
    if (!(gontiBigintCompare(bi1, BIGINT_ZERO) && gontiBigintCompare(bi2, BIGINT_ZERO))) return BIGINT_ZERO;

    char bi1ComparisionP1 = gontiBigintCompare(bi1, BIGINT_ONE);
    char bi1ComparisionN1 = gontiBigintCompare(bi1, BIGINT_NEG_ONE);

    if (!(bi1ComparisionP1 && bi1ComparisionN1)) {
        return gontiBigintCopyIntArr(
            bi2.digits, bi2.noDigits,
            !bi1ComparisionP1 ? !bi2.sign : bi2.sign
        );
    }

    char bi2ComparisionP1 = gontiBigintCompare(bi2, BIGINT_ONE);
    char bi2ComparisionN1 = gontiBigintCompare(bi2, BIGINT_NEG_ONE);

    if (!(bi2ComparisionP1 && bi2ComparisionN1)) {
        return gontiBigintCopyIntArr(
            bi1.digits, bi1.noDigits,
            !bi2ComparisionP1 ? !bi1.sign : bi1.sign
        );
    }

    unsigned int minNoDigits = MIN(bi1.noDigits, bi2.noDigits); //bi1.noDigits;
    unsigned int maxNoDigits = MAX(bi1.noDigits, bi2.noDigits); //bi2.noDigits;

    //if (bi1.noDigits > bi2.noDigits) {
    //    minNoDigits = bi2.noDigits;
    //    maxNoDigits = bi1.noDigits;
    //}

    if (maxNoDigits < KARATSUBA_THRESHOLD || minNoDigits <= sqrtl(maxNoDigits)) {
        return gontiBigintLongMultiply(bi1, bi2);
    } else {
        return gontiBigintKaratsubaMultiply(bi1, bi2);
    }
}
bigint gontiBigintCopyIntArr(int* arr, unsigned int n, bool sign) {
    bigint ret = gontiBigintAllocate(n);
    ret.noDigits = n;
    ret.sign = sign;

    k_copyMemory(ret.digits, arr, n * sizeof(int));

    return ret;
}
bigint gontiBigintLongMultiply(bigint bi1, bigint bi2) {
    bool sign = !(bi1.sign ^ bi2.sign);
    
    unsigned int noDigits;
    int* retArr = gontiKaratsubaMultiplyLongIntArr(
        bi1.digits, 0, bi1.noDigits,
        bi2.digits, 0, bi2.noDigits,
        &noDigits
    );

    bigint ret = gontiBigintCopyIntArr(retArr, noDigits, sign);

    gontiBigintTrim(&ret);
    if (retArr) gontiKaratsubaFreeLongIntArr(0, bi1.noDigits, 0, bi2.noDigits, &retArr);

    return ret;
}
bigint gontiBigintKaratsubaMultiply(bigint bi1, bigint bi2) {
    bool sign = !(bi1.sign ^ bi2.sign);

    unsigned int maxNoDigits = MAX(bi1.noDigits, bi2.noDigits);
    unsigned int noDigits;
    int* retArr = gontiKaratsubaMultiplyIntArr(
        bi1.digits, bi1.noDigits,
        bi2.digits, bi2.noDigits,
        0, maxNoDigits,
        &noDigits
    );

    bigint ret = gontiBigintCopyIntArr(retArr, noDigits, sign);

    gontiBigintTrim(&ret);
    if (retArr) gontiKaratsubaFreeIntArr(0, maxNoDigits, &retArr);

    return ret;
}

/*
* CHAR* 
*/
char* gontiBigintPtrToString(bigint* bi) {
    if (!bi->noDigits) return "0";

    unsigned int noChars = (bi->noDigits - 1) * BIGINT_NO_BASE_DIGITS;
    unsigned int noDigitsLast = gonti_math.numDigits(bi->digits[bi->noDigits - 1], 10);

    noChars += noDigitsLast;

    if (!bi->sign) {
        noChars++;
    }

    char* ret = k_allocate((noChars + 1) * sizeof(char), GONTI_MEMORY_TAG_MATH);

    if (!ret) return NULL;

    unsigned int strIdx = 0;

    if (!bi->sign) {
        ret[strIdx++] = '-';
    }

    char offset = noDigitsLast;
    unsigned int strDigit;

    for (int digitInx = bi->noDigits; digitInx; digitInx--) {
        char intcrement = offset;
        unsigned int digit = bi->digits[digitInx - 1];

        while (offset--) {
            gonti_math.divMod(digit, 10, &digit, &strDigit);
            ret [strIdx + offset] = '0' + strDigit;
        }

        offset = BIGINT_NO_BASE_DIGITS;

        strIdx += intcrement;
    }

    ret[noChars] = '\0';

    return ret;
}
char* gontiBigintToString(bigint bi) {
    return gontiBigintPtrToString(&bi);
}

/*
* CHAR
*/
char gontiBigintCompare(bigint bi1, bigint bi2) {
    if (bi1.sign && !bi2.sign) return 1;
    else if (!bi1.sign && bi2.sign) return -1;
    else if (!bi1.sign && !bi2.sign) {
        bi1.sign = true;
        bi2.sign = true;

        char comparsion = gontiBigintCompare(bi1, bi2);
        return -comparsion;
    }

    if (bi1.noDigits > bi2.noDigits) return 1;
    else if (bi1.noDigits < bi2.noDigits) return -1;

    unsigned int idx = bi1.noDigits;

    while (idx--) {
        if (bi1.digits[idx] > bi2.digits[idx]) return 1;
        else if (bi1.digits[idx] < bi2.digits[idx]) return -1;
    }

    return 0;
}

/*
* VOID
*/
void gontiBigintFree(bigint* bi) {
    k_free(bi->digits, bi->capacity * sizeof(unsigned int), GONTI_MEMORY_TAG_BIGINT);
    
    bi->capacity = 0;
    bi->noDigits = 0;
    bi->sign = 1;
}
void gontiBigintTrim(bigint* bi) {
    while (!bi->digits[bi->noDigits - 1]) {
        bi->noDigits--;
    }

    // int* newDigits = k_allocate(bi->noDigits * sizeof(unsigned int), GONTI_MEMORY_TAG_MATH);

    // for (unsigned int i = 0; i < bi->noDigits; i++) {
    //     newDigits[i] = bi->digits[i];
    // }

    // int* tmp = bi->digits;
    // bi->digits = newDigits;
    // newDigits = tmp;

    // k_free(newDigits, *tmp * sizeof(unsigned int), GONTI_MEMORY_TAG_MATH);
    // free(tmp);
}