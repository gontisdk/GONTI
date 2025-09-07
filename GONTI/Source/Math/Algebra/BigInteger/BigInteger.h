#ifndef BIGINTEGER_H
#define BIGINTEGER_H

#ifdef __cplusplus
extern "C" {
#endif

    #include "../../../__GONTI_INCLUDES.h"

    #ifdef BigInteger_ON

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

        /*BIGINT*/
        KAPI bigint bigint_allocate(unsigned int capacity);
        KAPI bigint bigint_strToBigint(char* str);
        KAPI bigint bigint_newBigint(int i);
        KAPI bigint bigint_newBigintLL(long long ll);
        KAPI bigint bigint_newPositiveBigint(unsigned int i);
        KAPI bigint bigint_newPositiveBigintLL(unsigned long long ll);
        KAPI bigint bigint_add(bigint bi1, bigint bi2);
        KAPI bigint bigint_subtract(bigint bi1, bigint bi2);
        KAPI bigint bigint_multiplyInt(unsigned int i1, unsigned int i2);
        KAPI bigint bigint_multiplyLL(unsigned long long ll1, unsigned long long ll2);
        KAPI bigint bigint_multiply(bigint bi1, bigint bi2);

        /* CHAR* */
        KAPI char* bigint_ptrToString(bigint* bi);
        KAPI char* bigint_toString(bigint bi);

        /*CHAR*/
        KAPI char bigint_compare(bigint bi1, bigint bi2);

        /*VOID*/
        KAPI void bigint_free(bigint* bi);
        KAPI void bigint_trim(bigint* bi);

        /*DEFINE*/
        #define BIGINT_BASE 1000000000
        #define BIGINT_NO_BASE_DIGITS 9

    #endif

#ifdef __cplusplus
}
#endif

#endif