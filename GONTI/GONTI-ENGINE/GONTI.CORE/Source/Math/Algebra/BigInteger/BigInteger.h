#ifndef BIGINTEGER_H
#define BIGINTEGER_H

#ifdef __cplusplus
extern "C" {
#endif

    #include "../../../Defines/Defines.h"

    #include <stdarg.h>
    #include <stdio.h>

    #include "../../Maths.h"

    typedef struct {
        bool sign;
        unsigned int capacity;
        unsigned int noDigits;
        int* digits;
    }bigint;

    extern KAPI const bigint BIGINT_ZERO;
    extern KAPI const bigint BIGINT_ONE;
    extern KAPI const bigint BIGINT_NEG_ONE;

    /*BIGINT*/
    KAPI bigint gontiBigintAllocate(unsigned int capacity);
    KAPI bigint gontiBigintStrToBigint(char* str);
    KAPI bigint gontiBigintNewBigint(int i);
    KAPI bigint gontiBigintNewBigintLL(long long ll);
    KAPI bigint gontiBigintNewPositiveBigint(unsigned int i);
    KAPI bigint gontiBigintNewPositiveBigintLL(unsigned long long ll);
    KAPI bigint gontiBigintAdd(bigint bi1, bigint bi2);
    KAPI bigint gontiBigintSubtract(bigint bi1, bigint bi2);
    KAPI bigint gontiBigintMultiplyInt(unsigned int i1, unsigned int i2);
    KAPI bigint gontiBigintMultiplyLL(unsigned long long ll1, unsigned long long ll2);
    KAPI bigint gontiBigintMultiply(bigint bi1, bigint bi2);
    KAPI bigint gontiBigintCopyIntArr(int* arr, unsigned int n, bool sign);
    KAPI bigint gontiBigintLongMultiply(bigint bi1, bigint bi2);
    KAPI bigint gontiBigintKaratsubaMultiply(bigint bi1, bigint bi2);

    /* CHAR* */
    KAPI char* gontiBigintPtrToString(bigint* bi);
    KAPI char* gontiBigintToString(bigint bi);

    /*CHAR*/
    KAPI char gontiBigintCompare(bigint bi1, bigint bi2);

    /*VOID*/
    KAPI void gontiBigintFree(bigint* bi);
    KAPI void gontiBigintTrim(bigint* bi);

    /*DEFINE*/
    #define BIGINT_BASE 1000000000
    #define BIGINT_NO_BASE_DIGITS 9

#ifdef __cplusplus
}
#endif

#endif