#include "Vectors.h"

#include "../../../Memory/Memory.h"

const vec VEC_UNDEFINED = { 0, NULL };

/*
* VEC
*/
vec gontiVecAllocate(unsigned int dim) {
	vec ret;

	ret.dim = dim;
	ret.elements = k_allocate(dim * sizeof(float), GONTI_MEMORY_TAG_VECTOR);

	return ret;
}
vec gontiVecConstructDefaultgontiVectors(unsigned int dim, float val) {
	vec ret = gontiVecAllocate(dim);

	for (unsigned int i = 0; i < dim; i++) {
		ret.elements[i] = val;
	}

	return ret;
}
vec gontiVecConstructEmptyVector(unsigned int dim) {
	return gontiVecConstructDefaultgontiVectors(dim, 0);
}
vec gontiVecNewVector(unsigned int dim, ...) {
	vec ret = gontiVecAllocate(dim);

	va_list list;
	va_start(list, dim);

	for (int i = 0; i < dim; i++) {
		ret.elements[i] = va_arg(list, double);
	}

	va_end(list);
	return ret;
}
vec gontiVecCopy(vec v) {
	return v;
}
vec gontiVecCopyPtr(vec* v) {
	vec ret;
	k_copyMemory(&ret, v, sizeof(vec));
	return ret;
}
vec gontiVecScalarMultiplication(vec v, float k) {
	vec ret = gontiVecAllocate(v.dim);

	for (unsigned int i = 0; i < ret.dim; i++)
	{
		ret.elements[i] = v.elements[i] * k;
	}

	return ret;
}
vec gontiVecScalarDivision(vec v, float k) {
	vec ret = gontiVecAllocate(v.dim);

	for (unsigned int i = 0; i < ret.dim; i++)
	{
		ret.elements[i] = v.elements[i] / k;
	}

	return ret;
}
vec gontiVecAdd(vec v1, vec v2) {
	if (v1.dim != v2.dim) {
		return VEC_UNDEFINED;
	}

	vec ret = gontiVecAllocate(v1.dim);

	for (unsigned int i = 0; i < ret.dim; i++) {
		ret.elements[i] = v1.elements[i] + v2.elements[i];
	}

	return ret;
}
vec gontiVecSubtract(vec v1, vec v2) {
	if (v1.dim != v2.dim) {
		return VEC_UNDEFINED;
	}

	vec ret = gontiVecAllocate(v1.dim);

	for (unsigned int i = 0; i < ret.dim; i++) {
		ret.elements[i] = v1.elements[i] - v2.elements[i];
	}

	return ret;
}
vec gontiVecMultiply(vec v1, vec v2) {
	if (v1.dim != v2.dim) {
		return VEC_UNDEFINED;
	}

	vec ret = gontiVecAllocate(v1.dim);

	for (unsigned int i = 0; i < ret.dim; i++) {
		ret.elements[i] = v1.elements[i] * v2.elements[i];
	}

	return ret;
}
vec gontiVecDivide(vec v1, vec v2) {
	if (v1.dim != v2.dim) {
		return VEC_UNDEFINED;
	}

	vec ret = gontiVecAllocate(v1.dim);

	for (unsigned int i = 0; i < ret.dim; i++) {
		ret.elements[i] = v1.elements[i] / v2.elements[i];
	}

	return ret;
}
vec gontiVecCross(vec v1, vec v2) {
	if (v1.dim != 3 || v2.dim != 3) {
		return VEC_UNDEFINED;
	}

	vec ret = gontiVecAllocate(3);

	ret.elements[0] = (v1.elements[1] * v2.elements[2]) - (v1.elements[2] * v2.elements[1]);
	ret.elements[1] = -1 * ((v1.elements[0] * v2.elements[2]) - (v1.elements[2] * v2.elements[0]));
	ret.elements[2] = (v1.elements[0] * v2.elements[1]) - (v1.elements[1] * v2.elements[0]);

	return ret;
}
vec gontiVecNormalized(vec v) {
	vec ret = gontiVecAllocate(v.dim);
	float mag = gontiVecMagnitude(v);

	for (unsigned int i = 0; i < ret.dim; i++) {
		ret.elements[i] = v.elements[i] / mag;
	}

	return ret;
}
vec gontiVecPower(vec v, float k) {
	vec ret = gontiVecAllocate(v.dim);

	for (unsigned int i = 0; i < ret.dim; i++)
	{
		ret.elements[i] = powf(v.elements[i], k);
	}

	return ret;
}

/*
* VOID
*/
void gontiVecPrint(vec v) {
	printf("\n[ ");

	for (int i = 0; i < v.dim; i++) {
		printf("%f", v.elements[i]);

		if (i < v.dim - 1) {
			printf(", ");
		}
		else {
			printf(" ");
		}
	}

	printf("]\n");
}
void gontiVecScalarMultiplicationBy(vec* v, float k) {
	for (unsigned int i = 0; i < v->dim; i++) {
		v->elements[i] *= k;
	}
}
void gontiVecScalarDivisionBy(vec* v, float k) {
	for (unsigned int i = 0; i < v->dim; i++) {
		v->elements[i] /= k;
	}
}
void gontiVecNormalize(vec* v) {
	float mag = gontiVecMagnitude(*v);

	for (unsigned int i = 0; i < v->dim; i++) {
		v->elements[i] /= mag;
	}
}
void gontiVecPowerOf(vec* v, float k) {
	for (unsigned int i = 0; i < v->dim; i++) {
		v->elements[i] = powf(v->elements[i], k);
	}
}
void gontiVecFree(vec* v) {
	k_free(v->elements, v->dim * sizeof(float), GONTI_MEMORY_TAG_VECTOR);
	v->elements = NULL;
	v->dim = 0;
}

/*
* BOOL
*/
bool gontiVecEquals(vec v1, vec v2) {
	if (v1.dim == v2.dim) {
		for (unsigned int i = 0; i < v1.dim; i++)
		{
			if (v1.elements[i] != v2.elements[i]) {
				return false;
			}
		}

		return true;
	}

	return false;
}
bool gontiVecAddTo(vec* v1, vec v2) {
	if (v1->dim != v2.dim) {
		return false;
	}

	for (unsigned int i = 0; i < v1->dim; i++) {
		v1->elements[i] += v2.elements[i];
	}

	return true;
}
bool gontiVecSubtractFrom(vec* v1, vec v2) {
	if (v1->dim != v2.dim) {
		return false;
	}

	for (unsigned int i = 0; i < v1->dim; i++) {
		v1->elements[i] -= v2.elements[i];
	}

	return true;
}
bool gontiVecMultiplyBy(vec* v1, vec v2) {
	if (v1->dim != v2.dim) {
		return false;
	}

	for (unsigned int i = 0; i < v1->dim; i++) {
		v1->elements[i] *= v2.elements[i];
	}

	return true;
}
bool gontiVecDivideBy(vec* v1, vec v2) {
	if (v1->dim != v2.dim) {
		return false;
	}

	for (unsigned int i = 0; i < v1->dim; i++) {
		v1->elements[i] /= v2.elements[i];
	}

	return true;
}
bool gontiVecOrthogonal(vec v1, vec v2) {
	return v1.dim == v2.dim ?
		gontiVecDot(v1, v2) == 0.0f : false;
}

/*
* FLOAT
*/
float gontiVecDot(vec v1, vec v2) {
	float ret = 0.0f;

	if (v1.dim == v2.dim) {
		for (unsigned int i = 0; i < v1.dim; i++) {
			ret += v1.elements[i] * v2.elements[i];
		}
	}

	return ret;
}
float gontiVecMagnitude(vec v) {
	return sqrt(gontiVecMagnitudeSquared(v));
}
float gontiVecMagnitudeSquared(vec v) {
	float ret = 0.0f;

	for (unsigned int i = 0; i < v.dim; i++) {
		ret += v.elements[i] * v.elements[i];
	}

	return ret;
}

/*
* gonti_vector
*/
gontiVectors gonti_vector = {
	/*VEC*/
	.allocate = gontiVecAllocate,
	.constructDefaultgontiVectors = gontiVecConstructDefaultgontiVectors,
	.constructEmptyVector = gontiVecConstructEmptyVector,
	.newVector = gontiVecNewVector,
	.copy = gontiVecCopy,
	.copyPtr = gontiVecCopyPtr,
	.scalarMultiplication = gontiVecScalarMultiplication,
	.scalarDivision = gontiVecScalarDivision,
	.add = gontiVecAdd,
	.subtract = gontiVecSubtract,
	.multiply = gontiVecMultiply,
	.divide = gontiVecDivide,
	.cross = gontiVecCross,
	.normalized = gontiVecNormalized,
	.power = gontiVecPower,

	/*VOID*/
	.print = gontiVecPrint,
	.scalarMultiplicationBy = gontiVecScalarMultiplicationBy,
	.scalarDivisionBy = gontiVecScalarDivisionBy,
	.normalize = gontiVecNormalize,
	.powerOf = gontiVecPowerOf,
	.free = gontiVecFree,

	/*BOOL*/
	.equals = gontiVecEquals,
	.addTo = gontiVecAddTo,
	.subtractFrom = gontiVecSubtractFrom,
	.multiplyBy = gontiVecMultiplyBy,
	.divideBy = gontiVecDivideBy,
	.orthogonal = gontiVecOrthogonal,

	/*FLOAT*/
	.dot = gontiVecDot,
	.magnitude = gontiVecMagnitude,
	.magnitudeSquared = gontiVecMagnitudeSquared
};
