#ifndef VECTORS_H
#define VECTORS_H

#ifdef __cplusplus
extern "C" {
#endif

	/*=============================================={[ CODE ]}==============================================*/

	#include "../../Maths.h"

	#include "stdarg.h"
	#include "stdio.h"
	#include "stdlib.h"
	#include "string.h"
	#include "math.h"
	
	/*type vec*/
	typedef struct {
		unsigned int dim;
		float* elements;
	}vec;

	extern KAPI const vec VEC_UNDEFINED;

	/*VEC*/
	KAPI vec gontiVecAllocate(unsigned int dim);
	KAPI vec gontiVecConstructDefaultgontiVectors(unsigned int dim, float val);
	KAPI vec gontiVecConstructEmptyVector(unsigned int dim);
	KAPI vec gontiVecNewVector(unsigned int dim, ...);
	KAPI vec gontiVecCopy(vec v);
	KAPI vec gontiVecCopyPtr(vec* v);
	KAPI vec gontiVecScalarMultiplication(vec v, float k);
	KAPI vec gontiVecScalarDivision(vec v, float k);
	KAPI vec gontiVecAdd(vec v1, vec v2);
	KAPI vec gontiVecSubtract(vec v1, vec v2);
	KAPI vec gontiVecMultiply(vec v1, vec v2);
	KAPI vec gontiVecDivide(vec v1, vec v2);
	KAPI vec gontiVecCross(vec v1, vec v2);
	KAPI vec gontiVecNormalized(vec v);
	KAPI vec gontiVecPower(vec v, float k);

	/*VOID*/
	KAPI void gontiVecPrint(vec v);
	KAPI void gontiVecScalarMultiplicationBy(vec* v, float k);
	KAPI void gontiVecScalarDivisionBy(vec* v, float k);
	KAPI void gontiVecNormalize(vec* v);
	KAPI void gontiVecPowerOf(vec* v, float k);
	KAPI void gontiVecFree(vec* v);

	/*BOOL*/
	KAPI bool gontiVecEquals(vec v1, vec v2);
	KAPI bool gontiVecAddTo(vec* v1, vec v2);
	KAPI bool gontiVecSubtractFrom(vec* v1, vec v2);
	KAPI bool gontiVecMultiplyBy(vec* v1, vec v2);
	KAPI bool gontiVecDivideBy(vec* v1, vec v2);
	KAPI bool gontiVecOrthogonal(vec v1, vec v2);

	/*FLOAT*/
	KAPI float gontiVecDot(vec v1, vec v2);
	KAPI float gontiVecMagnitude(vec v);
	KAPI float gontiVecMagnitudeSquared(vec v);

	/*gonti_vector*/

	typedef struct {
		/*VEC*/
		vec(*allocate)(unsigned int dim);
		vec(*constructDefaultgontiVectors)(unsigned int dim, float val);
		vec(*constructEmptyVector)(unsigned int dim);
		vec(*newVector)(unsigned int dim, ...);
		vec(*copy)(vec v);
		vec(*copyPtr)(vec* v);
		vec(*scalarMultiplication)(vec v, float k);
		vec(*scalarDivision)(vec v, float k);
		vec(*add)(vec v1, vec v2);
		vec(*subtract)(vec v1, vec v2);
		vec(*multiply)(vec v1, vec v2);
		vec(*divide)(vec v1, vec v2);
		vec(*cross)(vec v1, vec v2);
		vec(*normalized)(vec v);
		vec(*power)(vec v, float k);

		/*VOID*/
		void (*print)(vec v);
		void (*scalarMultiplicationBy)(vec* v, float k);
		void (*scalarDivisionBy)(vec* v, float k);
		void (*normalize)(vec* v);
		void (*powerOf)(vec* v, float k);
		void (*free)(vec* v);

		/*BOOL*/
		bool (*equals)(vec v1, vec v2);
		bool (*addTo)(vec* v1, vec v2);
		bool (*subtractFrom)(vec* v1, vec v2);
		bool (*multiplyBy)(vec* v1, vec v2);
		bool (*divideBy)(vec* v1, vec v2);
		bool (*orthogonal)(vec v1, vec v2);

		/*FLOAT*/
		float (*dot)(vec v1, vec v2);
		float (*magnitude)(vec v);
		float (*magnitudeSquared)(vec v);
	} gontiVectors;
	extern KAPI gontiVectors gonti_vector;

	/*DEFINES*/

	#define vector(...) gontiVecNewVector(NUMARGS(float, __VA_ARGS__), ##__VA_ARGS__)

	/*=============================================={[ !CODE ]}==============================================*/

#ifdef __cplusplus
}
#endif

#endif // !VECTORS_H