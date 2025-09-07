#include "Vectors.h"

#ifdef Vectors_ON

#include "../../Core/MemorySubsystem/MemorySubsystem.h"

const vec VEC_UNDEFINED = { 0, NULL };

/*
* VEC
*/
vec vec_allocate(unsigned int dim) {
	vec ret;

	ret.dim = dim;
	ret.elements = k_allocate(dim * sizeof(float), MEMORY_TAG_VECTOR);

	return ret;
}
vec vec_constructDefaultVectors(unsigned int dim, float val) {
	vec ret = vec_allocate(dim);

	for (unsigned int i = 0; i < dim; i++) {
		ret.elements[i] = val;
	}

	return ret;
}
vec vec_constructEmptyVector(unsigned int dim) {
	return vec_constructDefaultVectors(dim, 0);
}
vec vec_newVector(unsigned int dim, ...) {
	vec ret = vec_allocate(dim);

	va_list list;
	va_start(list, dim);

	for (int i = 0; i < dim; i++) {
		ret.elements[i] = va_arg(list, double);
	}

	va_end(list);
	return ret;
}
vec vec_copy(vec v) {
	return v;
}
vec vec_copyPtr(vec* v) {
	vec ret;
	k_copyMemory(&ret, v, sizeof(vec));
	return ret;
}
vec vec_scalarMultiplication(vec v, float k) {
	vec ret = vec_allocate(v.dim);

	for (unsigned int i = 0; i < ret.dim; i++)
	{
		ret.elements[i] = v.elements[i] * k;
	}

	return ret;
}
vec vec_scalarDivision(vec v, float k) {
	vec ret = vec_allocate(v.dim);

	for (unsigned int i = 0; i < ret.dim; i++)
	{
		ret.elements[i] = v.elements[i] / k;
	}

	return ret;
}
vec vec_add(vec v1, vec v2) {
	if (v1.dim != v2.dim) {
		return VEC_UNDEFINED;
	}

	vec ret = vec_allocate(v1.dim);

	for (unsigned int i = 0; i < ret.dim; i++) {
		ret.elements[i] = v1.elements[i] + v2.elements[i];
	}

	return ret;
}
vec vec_subtract(vec v1, vec v2) {
	if (v1.dim != v2.dim) {
		return VEC_UNDEFINED;
	}

	vec ret = vec_allocate(v1.dim);

	for (unsigned int i = 0; i < ret.dim; i++) {
		ret.elements[i] = v1.elements[i] - v2.elements[i];
	}

	return ret;
}
vec vec_multiply(vec v1, vec v2) {
	if (v1.dim != v2.dim) {
		return VEC_UNDEFINED;
	}

	vec ret = vec_allocate(v1.dim);

	for (unsigned int i = 0; i < ret.dim; i++) {
		ret.elements[i] = v1.elements[i] * v2.elements[i];
	}

	return ret;
}
vec vec_divide(vec v1, vec v2) {
	if (v1.dim != v2.dim) {
		return VEC_UNDEFINED;
	}

	vec ret = vec_allocate(v1.dim);

	for (unsigned int i = 0; i < ret.dim; i++) {
		ret.elements[i] = v1.elements[i] / v2.elements[i];
	}

	return ret;
}
vec vec_cross(vec v1, vec v2) {
	if (v1.dim != 3 || v2.dim != 3) {
		return VEC_UNDEFINED;
	}

	vec ret = vec_allocate(3);

	ret.elements[0] = (v1.elements[1] * v2.elements[2]) - (v1.elements[2] * v2.elements[1]);
	ret.elements[1] = -1 * ((v1.elements[0] * v2.elements[2]) - (v1.elements[2] * v2.elements[0]));
	ret.elements[2] = (v1.elements[0] * v2.elements[1]) - (v1.elements[1] * v2.elements[0]);

	return ret;
}
vec vec_normalized(vec v) {
	vec ret = vec_allocate(v.dim);
	float mag = vec_magnitude(v);

	for (unsigned int i = 0; i < ret.dim; i++) {
		ret.elements[i] = v.elements[i] / mag;
	}

	return ret;
}
vec vec_power(vec v, float k) {
	vec ret = vec_allocate(v.dim);

	for (unsigned int i = 0; i < ret.dim; i++)
	{
		ret.elements[i] = powf(v.elements[i], k);
	}

	return ret;
}

/*
* VOID
*/
void vec_print(vec v) {
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
void vec_scalarMultiplicationBy(vec* v, float k) {
	for (unsigned int i = 0; i < v->dim; i++) {
		v->elements[i] *= k;
	}
}
void vec_scalarDivisionBy(vec* v, float k) {
	for (unsigned int i = 0; i < v->dim; i++) {
		v->elements[i] /= k;
	}
}
void vec_normalize(vec* v) {
	float mag = vec_magnitude(*v);

	for (unsigned int i = 0; i < v->dim; i++) {
		v->elements[i] /= mag;
	}
}
void vec_powerOf(vec* v, float k) {
	for (unsigned int i = 0; i < v->dim; i++) {
		v->elements[i] = powf(v->elements[i], k);
	}
}
void vec_free(vec* v) {
	k_free(v->elements, v->dim * sizeof(float), MEMORY_TAG_VECTOR);
	v->elements = NULL;
	v->dim = 0;
}

/*
* BOOL
*/
bool vec_equals(vec v1, vec v2) {
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
bool vec_addTo(vec* v1, vec v2) {
	if (v1->dim != v2.dim) {
		return false;
	}

	for (unsigned int i = 0; i < v1->dim; i++) {
		v1->elements[i] += v2.elements[i];
	}

	return true;
}
bool vec_subtractFrom(vec* v1, vec v2) {
	if (v1->dim != v2.dim) {
		return false;
	}

	for (unsigned int i = 0; i < v1->dim; i++) {
		v1->elements[i] -= v2.elements[i];
	}

	return true;
}
bool vec_multiplyBy(vec* v1, vec v2) {
	if (v1->dim != v2.dim) {
		return false;
	}

	for (unsigned int i = 0; i < v1->dim; i++) {
		v1->elements[i] *= v2.elements[i];
	}

	return true;
}
bool vec_divideBy(vec* v1, vec v2) {
	if (v1->dim != v2.dim) {
		return false;
	}

	for (unsigned int i = 0; i < v1->dim; i++) {
		v1->elements[i] /= v2.elements[i];
	}

	return true;
}
bool vec_orthogonal(vec v1, vec v2) {
	return v1.dim == v2.dim ?
		vec_dot(v1, v2) == 0.0f : false;
}

/*
* FLOAT
*/
float vec_dot(vec v1, vec v2) {
	float ret = 0.0f;

	if (v1.dim == v2.dim) {
		for (unsigned int i = 0; i < v1.dim; i++) {
			ret += v1.elements[i] * v2.elements[i];
		}
	}

	return ret;
}
float vec_magnitude(vec v) {
	return sqrt(vec_magnitudeSquared(v));
}
float vec_magnitudeSquared(vec v) {
	float ret = 0.0f;

	for (unsigned int i = 0; i < v.dim; i++) {
		ret += v.elements[i] * v.elements[i];
	}

	return ret;
}

/*
* m_vector
*/
Vectors m_vector = {
	/*VEC*/
	.allocate = vec_allocate,
	.constructDefaultVectors = vec_constructDefaultVectors,
	.constructEmptyVector = vec_constructEmptyVector,
	.newVector = vec_newVector,
	.copy = vec_copy,
	.copyPtr = vec_copyPtr,
	.scalarMultiplication = vec_scalarMultiplication,
	.scalarDivision = vec_scalarDivision,
	.add = vec_add,
	.subtract = vec_subtract,
	.multiply = vec_multiply,
	.divide = vec_divide,
	.cross = vec_cross,
	.normalized = vec_normalized,
	.power = vec_power,

	/*VOID*/
	.print = vec_print,
	.scalarMultiplicationBy = vec_scalarMultiplicationBy,
	.scalarDivisionBy = vec_scalarDivisionBy,
	.normalize = vec_normalize,
	.powerOf = vec_powerOf,
	.free = vec_free,

	/*BOOL*/
	.equals = vec_equals,
	.addTo = vec_addTo,
	.subtractFrom = vec_subtractFrom,
	.multiplyBy = vec_multiplyBy,
	.divideBy = vec_divideBy,
	.orthogonal = vec_orthogonal,

	/*FLOAT*/
	.dot = vec_dot,
	.magnitude = vec_magnitude,
	.magnitudeSquared = vec_magnitudeSquared
};

#endif
