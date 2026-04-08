#ifndef GtVectorH
#define GtVectorH

#ifdef __cplusplus
	extern "C" {
#endif

		#include "../../../GtMaths.h"
		#include "../GtVecTypes.inl"

		#include "stdarg.h"
		#include "stdio.h"
		#include "stdlib.h"
		#include "string.h"
		#include "math.h"

		extern GTAPI const GtVec VEC_UNDEFINED;

		/*VEC*/
		GTAPI GtVec gontiVecAllocate(GtU32 dim);
		GTAPI GtVec gontiVecConstructDefaultgontiVectors(GtU32 dim, GtF32 val);
		GTAPI GtVec gontiVecConstructEmptyVector(GtU32 dim);
		GTAPI GtVec gontiVecNewVector(GtU32 dim, ...);
		GTAPI GtVec gontiVecCopy(GtVec v);
		GTAPI GtVec gontiVecCopyPtr(GtVec* v);
		GTAPI GtVec gontiVecScalarMultiplication(GtVec v, GtF32 k);
		GTAPI GtVec gontiVecScalarDivision(GtVec v, GtF32 k);
		GTAPI GtVec gontiVecAdd(GtVec v1, GtVec v2);
		GTAPI GtVec gontiVecSubtract(GtVec v1, GtVec v2);
		GTAPI GtVec gontiVecMultiply(GtVec v1, GtVec v2);
		GTAPI GtVec gontiVecDivide(GtVec v1, GtVec v2);
		GTAPI GtVec gontiVecCross(GtVec v1, GtVec v2);
		GTAPI GtVec gontiVecNormalized(GtVec v);
		GTAPI GtVec gontiVecPower(GtVec v, GtF32 k);

		/*VOID*/
		GTAPI void gontiVecPrint(GtVec v);
		GTAPI void gontiVecScalarMultiplicationBy(GtVec* v, GtF32 k);
		GTAPI void gontiVecScalarDivisionBy(GtVec* v, GtF32 k);
		GTAPI void gontiVecNormalize(GtVec* v);
		GTAPI void gontiVecPowerOf(GtVec* v, GtF32 k);
		GTAPI void gontiVecFree(GtVec* v);

		/*BOOL*/
		GTAPI GtB8 gontiVecEquals(GtVec v1, GtVec v2);
		GTAPI GtB8 gontiVecAddTo(GtVec* v1, GtVec v2);
		GTAPI GtB8 gontiVecSubtractFrom(GtVec* v1, GtVec v2);
		GTAPI GtB8 gontiVecMultiplyBy(GtVec* v1, GtVec v2);
		GTAPI GtB8 gontiVecDivideBy(GtVec* v1, GtVec v2);
		GTAPI GtB8 gontiVecOrthogonal(GtVec v1, GtVec v2);

		/*FLOAT*/
		GTAPI GtF32 gontiVecDot(GtVec v1, GtVec v2);
		GTAPI GtF32 gontiVecMagnitude(GtVec v);
		GTAPI GtF32 gontiVecMagnitudeSquared(GtVec v);

		/*DEFINES*/

		#define GtVector(...) gontiVecNewVector(GT_NUMARGS(GtF32, __VA_ARGS__), ##__VA_ARGS__)

#ifdef __cplusplus
	}
#endif

#endif