#ifndef VECTORS_H
#define VECTORS_H

#ifdef __cplusplus
extern "C" {
#endif

	/*=============================================={[ CODE ]}==============================================*/

	#include "../Maths.h"

	#ifdef Vectors_ON

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
		KAPI vec vec_allocate(unsigned int dim);
		KAPI vec vec_constructDefaultVectors(unsigned int dim, float val);
		KAPI vec vec_constructEmptyVector(unsigned int dim);
		KAPI vec vec_newVector(unsigned int dim, ...);
		KAPI vec vec_copy(vec v);
		KAPI vec vec_copyPtr(vec* v);
		KAPI vec vec_scalarMultiplication(vec v, float k);
		KAPI vec vec_scalarDivision(vec v, float k);
		KAPI vec vec_add(vec v1, vec v2);
		KAPI vec vec_subtract(vec v1, vec v2);
		KAPI vec vec_multiply(vec v1, vec v2);
		KAPI vec vec_divide(vec v1, vec v2);
		KAPI vec vec_cross(vec v1, vec v2);
		KAPI vec vec_normalized(vec v);
		KAPI vec vec_power(vec v, float k);

		/*VOID*/
		KAPI void vec_print(vec v);
		KAPI void vec_scalarMultiplicationBy(vec* v, float k);
		KAPI void vec_scalarDivisionBy(vec* v, float k);
		KAPI void vec_normalize(vec* v);
		KAPI void vec_powerOf(vec* v, float k);
		KAPI void vec_free(vec* v);

		/*BOOL*/
		KAPI bool vec_equals(vec v1, vec v2);
		KAPI bool vec_addTo(vec* v1, vec v2);
		KAPI bool vec_subtractFrom(vec* v1, vec v2);
		KAPI bool vec_multiplyBy(vec* v1, vec v2);
		KAPI bool vec_divideBy(vec* v1, vec v2);
		KAPI bool vec_orthogonal(vec v1, vec v2);

		/*FLOAT*/
		KAPI float vec_dot(vec v1, vec v2);
		KAPI float vec_magnitude(vec v);
		KAPI float vec_magnitudeSquared(vec v);

		/*m_vector*/

		typedef struct {
			/*VEC*/
			vec(*allocate)(unsigned int dim);
			vec(*constructDefaultVectors)(unsigned int dim, float val);
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
		} Vectors;
		extern KAPI Vectors m_vector;

		/*DEFINES*/

		#define vector(...) vec_newVector(NUMARGS(float, __VA_ARGS__), ##__VA_ARGS__)

	#endif // !Vectors_ON

	/*=============================================={[ !CODE ]}==============================================*/

#ifdef __cplusplus
}
#endif

#endif // !VECTORS_H