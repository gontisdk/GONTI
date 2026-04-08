#include "GtVector.h"

#include "../../../../Memory/GtMemory.h"

const GtVec VEC_UNDEFINED = { 0, NULL };

/*
* VEC
*/
GtVec gontiVecAllocate(GtU32 dim) {
	GtVec ret;

	ret.dim = dim;
	ret.elements = gt_allocate(dim * sizeof(GtF32), GT_MEM_TAG_VECTORS);

	return ret;
}
GtVec gontiVecConstructDefaultgontiVectors(GtU32 dim, GtF32 val) {
	GtVec ret = gontiVecAllocate(dim);

	for (GtU32 i = 0; i < dim; i++) {
		ret.elements[i] = val;
	}

	return ret;
}
GtVec gontiVecConstructEmptyVector(GtU32 dim) {
	return gontiVecConstructDefaultgontiVectors(dim, 0);
}
GtVec gontiVecNewVector(GtU32 dim, ...) {
	GtVec ret = gontiVecAllocate(dim);

	va_list list;
	va_start(list, dim);

	for (GtB32 i = 0; i < dim; i++) {
		ret.elements[i] = va_arg(list, GtF64);
	}

	va_end(list);
	return ret;
}
GtVec gontiVecCopy(GtVec v) {
	return v;
}
GtVec gontiVecCopyPtr(GtVec* v) {
	GtVec ret;
	gt_copyMemory(&ret, v, sizeof(GtVec));
	return ret;
}
GtVec gontiVecScalarMultiplication(GtVec v, GtF32 k) {
	GtVec ret = gontiVecAllocate(v.dim);

	for (GtU32 i = 0; i < ret.dim; i++)
	{
		ret.elements[i] = v.elements[i] * k;
	}

	return ret;
}
GtVec gontiVecScalarDivision(GtVec v, GtF32 k) {
	GtVec ret = gontiVecAllocate(v.dim);

	for (GtU32 i = 0; i < ret.dim; i++)
	{
		ret.elements[i] = v.elements[i] / k;
	}

	return ret;
}
GtVec gontiVecAdd(GtVec v1, GtVec v2) {
	if (v1.dim != v2.dim) {
		return VEC_UNDEFINED;
	}

	GtVec ret = gontiVecAllocate(v1.dim);

	for (GtU32 i = 0; i < ret.dim; i++) {
		ret.elements[i] = v1.elements[i] + v2.elements[i];
	}

	return ret;
}
GtVec gontiVecSubtract(GtVec v1, GtVec v2) {
	if (v1.dim != v2.dim) {
		return VEC_UNDEFINED;
	}

	GtVec ret = gontiVecAllocate(v1.dim);

	for (GtU32 i = 0; i < ret.dim; i++) {
		ret.elements[i] = v1.elements[i] - v2.elements[i];
	}

	return ret;
}
GtVec gontiVecMultiply(GtVec v1, GtVec v2) {
	if (v1.dim != v2.dim) {
		return VEC_UNDEFINED;
	}

	GtVec ret = gontiVecAllocate(v1.dim);

	for (GtU32 i = 0; i < ret.dim; i++) {
		ret.elements[i] = v1.elements[i] * v2.elements[i];
	}

	return ret;
}
GtVec gontiVecDivide(GtVec v1, GtVec v2) {
	if (v1.dim != v2.dim) {
		return VEC_UNDEFINED;
	}

	GtVec ret = gontiVecAllocate(v1.dim);

	for (GtU32 i = 0; i < ret.dim; i++) {
		ret.elements[i] = v1.elements[i] / v2.elements[i];
	}

	return ret;
}
GtVec gontiVecCross(GtVec v1, GtVec v2) {
	if (v1.dim != 3 || v2.dim != 3) {
		return VEC_UNDEFINED;
	}

	GtVec ret = gontiVecAllocate(3);

	ret.elements[0] = (v1.elements[1] * v2.elements[2]) - (v1.elements[2] * v2.elements[1]);
	ret.elements[1] = -1 * ((v1.elements[0] * v2.elements[2]) - (v1.elements[2] * v2.elements[0]));
	ret.elements[2] = (v1.elements[0] * v2.elements[1]) - (v1.elements[1] * v2.elements[0]);

	return ret;
}
GtVec gontiVecNormalized(GtVec v) {
	GtVec ret = gontiVecAllocate(v.dim);
	GtF32 mag = gontiVecMagnitude(v);

	for (GtU32 i = 0; i < ret.dim; i++) {
		ret.elements[i] = v.elements[i] / mag;
	}

	return ret;
}
GtVec gontiVecPower(GtVec v, GtF32 k) {
	GtVec ret = gontiVecAllocate(v.dim);

	for (GtU32 i = 0; i < ret.dim; i++)
	{
		ret.elements[i] = powf(v.elements[i], k);
	}

	return ret;
}

/*
* VOID
*/
void gontiVecPrint(GtVec v) {
	printf("\n[ ");

	for (GtB32 i = 0; i < v.dim; i++) {
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
void gontiVecScalarMultiplicationBy(GtVec* v, GtF32 k) {
	for (GtU32 i = 0; i < v->dim; i++) {
		v->elements[i] *= k;
	}
}
void gontiVecScalarDivisionBy(GtVec* v, GtF32 k) {
	for (GtU32 i = 0; i < v->dim; i++) {
		v->elements[i] /= k;
	}
}
void gontiVecNormalize(GtVec* v) {
	GtF32 mag = gontiVecMagnitude(*v);

	for (GtU32 i = 0; i < v->dim; i++) {
		v->elements[i] /= mag;
	}
}
void gontiVecPowerOf(GtVec* v, GtF32 k) {
	for (GtU32 i = 0; i < v->dim; i++) {
		v->elements[i] = powf(v->elements[i], k);
	}
}
void gontiVecFree(GtVec* v) {
	gt_free(v->elements);
	v->elements = NULL;
	v->dim = 0;
}

/*
* BOOL
*/
GtB8 gontiVecEquals(GtVec v1, GtVec v2) {
	if (v1.dim == v2.dim) {
		for (GtU32 i = 0; i < v1.dim; i++)
		{
			if (v1.elements[i] != v2.elements[i]) {
				return GtFalse;
			}
		}

		return GtTrue;
	}

	return GtFalse;
}
GtB8 gontiVecAddTo(GtVec* v1, GtVec v2) {
	if (v1->dim != v2.dim) {
		return GtFalse;
	}

	for (GtU32 i = 0; i < v1->dim; i++) {
		v1->elements[i] += v2.elements[i];
	}

	return GtTrue;
}
GtB8 gontiVecSubtractFrom(GtVec* v1, GtVec v2) {
	if (v1->dim != v2.dim) {
		return GtFalse;
	}

	for (GtU32 i = 0; i < v1->dim; i++) {
		v1->elements[i] -= v2.elements[i];
	}

	return GtTrue;
}
GtB8 gontiVecMultiplyBy(GtVec* v1, GtVec v2) {
	if (v1->dim != v2.dim) {
		return GtFalse;
	}

	for (GtU32 i = 0; i < v1->dim; i++) {
		v1->elements[i] *= v2.elements[i];
	}

	return GtTrue;
}
GtB8 gontiVecDivideBy(GtVec* v1, GtVec v2) {
	if (v1->dim != v2.dim) {
		return GtFalse;
	}

	for (GtU32 i = 0; i < v1->dim; i++) {
		v1->elements[i] /= v2.elements[i];
	}

	return GtTrue;
}
GtB8 gontiVecOrthogonal(GtVec v1, GtVec v2) {
	return v1.dim == v2.dim ?
		gontiVecDot(v1, v2) == 0.0f : GtFalse;
}

/*
* FLOAT
*/
GtF32 gontiVecDot(GtVec v1, GtVec v2) {
	GtF32 ret = 0.0f;

	if (v1.dim == v2.dim) {
		for (GtU32 i = 0; i < v1.dim; i++) {
			ret += v1.elements[i] * v2.elements[i];
		}
	}

	return ret;
}
GtF32 gontiVecMagnitude(GtVec v) {
	return sqrt(gontiVecMagnitudeSquared(v));
}
GtF32 gontiVecMagnitudeSquared(GtVec v) {
	GtF32 ret = 0.0f;

	for (GtU32 i = 0; i < v.dim; i++) {
		ret += v.elements[i] * v.elements[i];
	}

	return ret;
}
