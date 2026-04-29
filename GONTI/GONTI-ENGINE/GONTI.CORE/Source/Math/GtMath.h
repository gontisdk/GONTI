#ifndef GtMathsH
#define GtMathsH

#ifdef __cplusplus
	extern "C" {
#endif

		#include <Defines/GtDefines.inl>

		/* DEFINES */
		#define GT_NUMARGS(type, ...) (sizeof((type[]){(type)0, ##__VA_ARGS__}) / sizeof(type) - 1)

		#define GT_MAX(i1, i2) ((i1) > (i2) ? (i1) : (i2))
		#define GT_MIN(i1, i2) ((i1) < (i2) ? (i1) : (i2))

		#define GT_PI 3.14159265358979323846f
		#define GT_PI_2 2.0f * GT_PI
		#define GT_HALF_PI 0.5f * GT_PI
		#define GT_QUARTER_PI 0.25f * GT_PI
		#define GT_ONE_OVER_PI 1.0f / GT_PI
		#define GT_ONE_OVER_TWO_PI 1.0f / GT_PI_2

		#define GT_SQRT_TWO 1.41421356237309504880f
		#define GT_SQRT_THREE 1.73205080756887729352f
		#define GT_SQRT_ONE_OVER_TWO 0.70710678118654752440f
		#define GT_SQRT_ONE_OVER_THREE 0.57735026918962576450f

		#define GT_DEG2RAD_MULTIPLIER GT_PI / 180.0f
		#define GT_RAD2DEG_MULTIPLIER 180.0f / GT_PI

		#define GT_SEC_TO_MS_MULTIPLIER 1000.0f
		#define GT_MS_TO_SEC_MULTIPLIER 0.001f

		#define GT_INFINITY 1e30f
		#define GT_FLOAT_EPSILON 1.192092896e-07f

		/* INLINE BOOL / B8 */
		GTINLINE GtB8 gontiMathIsPowerOf2(GtU64 value) {
			return (value != 0) && ((value & (value - 1)) == 0);
		}

		/*VOID*/
		GTAPI void gontiMathSwap(GtB32* a, GtB32* b);
		GTAPI void gontiMathSwapFloat(GtF32* a, GtF32* b);
		GTAPI void gontiMathPrintUintArray(GtU32* arr, GtU32 noElements);
		GTAPI void gontiMathDivMod(GtU32 num, GtU32 divisor, GtU32* quotient, GtU32* remainder);
		GTAPI void gontiMathDivModLL(GtU64 num, GtU64 divisor, GtU64* quotient, GtU64* remainder);

		/*BOOL*/
		GTAPI GtB8 gontiMathContainsUint(GtU32* arr, GtU32 noElements, GtU32 target);

		/*UNSIGNED INT*/
		GTAPI GtU32 gontiMathNumDigits(GtU32 val, GtU32 base);

		/*UNSIGNED LONG LONG*/
		GTAPI GtU64 gontiMathNumDigitsLL(GtU64 val, GtU64 base);

		/* F32 / FLOAT */
		GTAPI GtF32 gontiMathSin(GtF32 x);
		GTAPI GtF32 gontiMathCos(GtF32 x);
		GTAPI GtF32 gontiMathTan(GtF32 x);
		GTAPI GtF32 gontiMathAcos(GtF32 x);
		GTAPI GtF32 gontiMathSqrt(GtF32 x);
		GTAPI GtF32 gontiMathAbs(GtF32 x);
		GTAPI GtF32 gontiMathRandomFloat(void);
		GTAPI GtF32 gontiMathRandomFloatInRange(GtF32 min, GtF32 max);

		/* I32 / INT */
		GTAPI GtI32 gontiMathRandom(void);
		GTAPI GtI32 gontiMathRandomInRange(GtI32 min, GtI32 max);

#ifdef __cplusplus
	}
#endif

#endif