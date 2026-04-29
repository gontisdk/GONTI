#ifndef GtBigintegerH
#define GtBigintegerH

#ifdef __cplusplus
    extern "C" {
#endif

        #include <Defines/GtDefines.inl>
        #include <stdarg.h>
        #include <stdio.h>
        #include <Math/GtMath.h>

        typedef struct {
            GtB8 sign;
            GtU32 capacity;
            GtU32 noDigits;
            GtB32* digits;
            GtB8 heapAllocated;
        }GtBigint;

        /*BIGINT*/
        GTAPI GtBigint gontiBigintAllocate(GtU32 capacity);
        GTAPI GtBigint gontiBigintZero(void);
        GTAPI GtBigint gontiBigintOne(void);
        GTAPI GtBigint gontiBigintNegOne(void);
        GTAPI GtBigint gontiBigintFromStr(char* str);
        GTAPI GtBigint gontiBigintFromInt(GtB32 i);
        GTAPI GtBigint gontiBigintFromLL(long long ll);
        GTAPI GtBigint gontiBigintFromUnsignedInt(GtU32 ui);
        GTAPI GtBigint gontiBigintFromUnsignedLL(GtU64 ull);
        GTAPI GtBigint gontiBigintAdd(GtBigint bi1, GtBigint bi2);
        GTAPI GtBigint gontiBigintSubtract(GtBigint bi1, GtBigint bi2);
        GTAPI GtBigint gontiBigintMultiplyUnsignedInt(GtU32 i1, GtU32 i2);
        GTAPI GtBigint gontiBigintMultiplyUnsignedLL(GtU64 ll1, GtU64 ll2);
        GTAPI GtBigint gontiBigintMultiply(GtBigint bi1, GtBigint bi2);
        GTAPI GtBigint gontiBigintCopyIntArr(GtB32* arr, GtU32 n, GtB8 sign);
        GTAPI GtBigint gontiBigintLongMultiply(GtBigint bi1, GtBigint bi2);
        GTAPI GtBigint gontiBigintKaratsubaMultiply(GtBigint bi1, GtBigint bi2);
        GTAPI GtBigint gontiBigintBaseDivide(GtBigint bi, GtB32 d);
        GTAPI GtBigint gontiBigintBaseMult(GtBigint bi, GtB32 d);

        /* CHAR* */
        GTAPI char* gontiBigintPtrToString(GtBigint* bi);
        GTAPI char* gontiBigintToString(GtBigint bi);

        /*CHAR*/
        GTAPI char gontiBigintCompare(GtBigint bi1, GtBigint bi2);

        /*VOID*/
        GTAPI void gontiBigintFree(GtBigint* bi);
        GTAPI void gontiBigintFreeString(char* str);
        GTAPI void gontiBigintTrim(GtBigint* bi);
        GTAPI void gontiBigintDebugPrint(const char* name, GtBigint* bi);

        /* INT* */
        GTAPI GtB32* gontiBigintRightShiftArr(GtB32* i, GtB32 n, GtB32 d, GtB32* newSize);
        GTAPI GtB32* gontiBigintLeftShiftArr(GtB32* i, GtB32 n, GtB32 d, GtB32* newSize);

        /*DEFINE*/
        #define BIGINT_BASE 1000000000
        #define BIGINT_NO_BASE_DIGITS 9

#ifdef __cplusplus
    }
#endif

#endif