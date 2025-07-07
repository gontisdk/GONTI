#include "BigInteger.h"

#ifdef BigInteger_ON

#include <stdlib.h>
#include <string.h>

#include "../../../Core/MemorySubsystem/MemorySubsystem.h"
#include "../../../Libs/StringLib/StringLib.h"

const bigint BIGINT_ZERO = { true, 0, 0, NULL };

/*
* BIGINT
*/
bigint bigint_allocate(unsigned int capacity) {
    bigint ret;

    ret.capacity = capacity;
    ret.noDigits = 0;
    ret.sign = true;
    ret.digits = k_allocate(capacity * sizeof(unsigned int), MEMORY_TAG_BIGINT);

    return ret;
}
bigint bigint_strToBigint(char* str) {
    unsigned int len = stringLength(str);
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

    m_math.divMod(effectiveLen, BIGINT_NO_BASE_DIGITS, &noDigits, &extra);

    if (extra) noDigits++;

    bigint ret = bigint_allocate(noDigits);
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
            bigint_free(&ret);
            return BIGINT_ZERO;
        }
    }

    return ret;
}
bigint bigint_newBigint(int i) {
    bool sign = true;

    if (i < 0) {
        sign = false;
        i *= -1;
    }

    bigint ret = bigint_newPositiveBigint(i);
    ret.sign = sign;

    return ret;
}
bigint bigint_newBigintLL(long long ll) {
    bool sign = true;

    if (ll < 0LL) {
        sign = false;
        ll *= -1LL;
    }

    bigint ret = bigint_newPositiveBigintLL(ll);
    ret.sign = sign;

    return ret;
}
bigint bigint_newPositiveBigint(unsigned int i) {
    unsigned int noDigits = m_math.numDigits(1, BIGINT_BASE);

    bigint ret = bigint_allocate(noDigits);
    ret.noDigits = noDigits;

    for (unsigned int idx = 0; idx < noDigits; idx++) {
        m_math.divMod(i, BIGINT_BASE, &i, (unsigned int*)&ret.digits[idx]);
    }

    return ret;
}
bigint bigint_newPositiveBigintLL(unsigned long long ll) {
    unsigned long long noDigits = m_math.numDigitsLL(1, BIGINT_BASE);

    bigint ret = bigint_allocate(noDigits);
    ret.noDigits = noDigits;

    for (unsigned int i = 0; i < noDigits; i++) {
        m_math.divModLL(ll, BIGINT_BASE, &ll, (unsigned long long*)&ret.digits[i]);
    }

    return ret;
}
bigint bigint_add(bigint bi1, bigint bi2) {
    if (bi1.sign && !bi2.sign) {
        bi2.sign = true;
        return bigint_subtract(bi1, bi2);
    } else if (!bi1.sign && bi2.sign) {
        bi1.sign = true;
        return bigint_subtract(bi2, bi1);
    }

    unsigned int noDigits = MAX(bi1.noDigits, bi2.noDigits) + 1;

    bigint ret = bigint_allocate(noDigits);
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

    bigint_trim(&ret);

    if (!bi1.sign && !bi2.sign) ret.sign = false;

    return ret;
}
bigint bigint_subtract(bigint bi1, bigint bi2) {
    char comparsion = bigint_compare(bi1, bi2);

    if (!comparsion) return BIGINT_ZERO;

    if (bi1.sign && !bi2.sign) {
        bi2.sign = true;

        return bigint_add(bi1, bi2);
    } else if (!bi1.sign && bi2.sign) {
        bi2.sign = false;

        return bigint_add(bi1, bi2);
    } else if (!bi1.sign && !bi2.sign) {
        bi1.sign = true;
        bi2.sign = true;

        return bigint_subtract(bi2, bi1);
    } else {
        if (comparsion < 0) {
            bigint res = bigint_subtract(bi2, bi1);
            res.sign = !res.sign;

            return res;
        }
    }

    unsigned int noDigits = bi1.noDigits;

    bigint ret = bigint_allocate(noDigits);
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

    bigint_trim(&ret);

    return ret;
}
bigint bigint_multiplyInt(unsigned int i1, unsigned int i2) {
    return bigint_newPositiveBigint(i1 * i2);
}
bigint bigint_multiplyLL(unsigned long long ll1, unsigned long long ll2) {
    return bigint_newPositiveBigintLL(ll1 * ll2);
}
bigint bigint_multiply(bigint bi1, bigint bi2) {
    if (!bigint_compare(bi1, BIGINT_ZERO) || !bigint_compare(bi2, BIGINT_ZERO)) return BIGINT_ZERO;

    bool sign = !(bi1.sign ^ bi2.sign);
    unsigned int noDigits = bi1.noDigits + bi2.noDigits;

    bigint ret = bigint_allocate(noDigits);
    ret.noDigits = noDigits;
    ret.sign = sign;

    for (unsigned int i = 0; i < ret.noDigits; i++) {
        ret.digits[i] = 0;
    }

    for (unsigned int i = 0; i < bi1.noDigits; i++) {
        int carry = 0;

        for (unsigned int j = 0; j < bi2.noDigits; j++) {
            bigint prod = bigint_multiplyLL((unsigned long long)bi1.digits[i], (unsigned long long)bi2.digits[j]);
            ret.digits[i + j] += carry + prod.digits[0];
            
            if (ret.digits[i + j] >= BIGINT_BASE) {
                carry = 1;
                ret.digits[i + j] -= BIGINT_BASE;
            } else {
                carry = 0;
            }

            if (prod.noDigits > 1) {
                carry += prod.digits[1];
            }
        }

        if (carry) {
            ret.digits[i + bi2.noDigits] = carry;
        }
    }

    bigint_trim(&ret);

    return ret;
}

/*
* CHAR* 
*/
char* bigint_ptrToString(bigint* bi) {
    if (!bi->noDigits) return "0";

    unsigned int noChars = (bi->noDigits - 1) * BIGINT_NO_BASE_DIGITS;
    unsigned int noDigitsLast = m_math.numDigits(bi->digits[bi->noDigits - 1], 10);

    noChars += noDigitsLast;

    if (!bi->sign) {
        noChars++;
    }

    char* ret = k_allocate((noChars + 1) * sizeof(char), MEMORY_TAG_MATH);

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
            m_math.divMod(digit, 10, &digit, &strDigit);
            ret [strIdx + offset] = '0' + strDigit;
        }

        offset = BIGINT_NO_BASE_DIGITS;

        strIdx += intcrement;
    }

    ret[noChars] = '\0';

    return ret;
}
char* bigint_toString(bigint bi) {
    return bigint_ptrToString(&bi);
}

/*
* CHAR
*/
char bigint_compare(bigint bi1, bigint bi2) {
    if (bi1.sign && !bi2.sign) return 1;
    else if (!bi1.sign && bi2.sign) return -1;
    else if (!bi1.sign && !bi2.sign) {
        bi1.sign = true;
        bi2.sign = true;

        char comparsion = bigint_compare(bi1, bi2);
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
void bigint_free(bigint* bi) {
    k_free(bi->digits, bi->capacity * sizeof(unsigned int), MEMORY_TAG_BIGINT);
    
    bi->capacity = 0;
    bi->noDigits = 0;
    bi->sign = 1;
}
void bigint_trim(bigint* bi) {
    while (!bi->digits[bi->noDigits - 1]) {
        bi->noDigits--;
    }

    // int* newDigits = k_allocate(bi->noDigits * sizeof(unsigned int), MEMORY_TAG_MATH);

    // for (unsigned int i = 0; i < bi->noDigits; i++) {
    //     newDigits[i] = bi->digits[i];
    // }

    // int* tmp = bi->digits;
    // bi->digits = newDigits;
    // newDigits = tmp;

    // k_free(newDigits, *tmp * sizeof(unsigned int), MEMORY_TAG_MATH);
    // free(tmp);
}

#endif