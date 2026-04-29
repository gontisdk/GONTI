#include <Math/GtMath.h>
#include <Platform/GtPlatform.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static GtB8 randSeeded = GtFalse;

/*
* VOID
*/
void gontiMathSwap(GtB32* a, GtB32* b) {
	*a ^= *b;
	*b = *a ^ *b;
	*a ^= *b;
}
void gontiMathSwapFloat(GtF32* a, GtF32* b) {
	gontiMathSwap((GtB32*)a, (GtB32*)b);
}
void gontiMathPrintUintArray(GtU32* arr, GtU32 noElements) {
	printf("\n[ ");

	for (GtU32 i = 0; i < noElements; i++) {
		printf("%d ", arr[i]);
	}

	printf("]\n");
}
void gontiMathDivMod(GtU32 num, GtU32 divisor, GtU32* quotient, GtU32* remainder) {
	div_t res = div(num, divisor);
	*quotient = res.quot;
	*remainder = res.rem;
}
void gontiMathDivModLL(GtU64 num, GtU64 divisor, GtU64* quotient, GtU64* remainder) {
	lldiv_t res = lldiv(num, divisor);
	*quotient = res.quot;
	*remainder = res.rem;
}

/*
* BOOL
*/
GtB8 gontiMathContainsUint(GtU32* arr, GtU32 noElements, GtU32 target) {
	for (GtU32 i = 0; i < noElements; i++) {
		if (arr[i] == target) return GtTrue;
	}

	return GtFalse;
}

/*
* UNSIGNED INT
*/
GtU32 gontiMathNumDigits(GtU32 val, GtU32 base) {
	if (base <= 1) return 0;
	if (val == 0) return 1;

	GtU32 ret = 0;

	while (val) {
		ret++;
		val /= base;
	}

	return ret;
}

/*
* UNSIGNED LONG LONG
*/
GtU64 gontiMathNumDigitsLL(GtU64 val, GtU64 base) {
	if (base <= 1LL) return 0;
	if (val == 0) return 1;

	GtU64 ret = 0;

	while (val) {
		ret++;
		val /= base;
	}

	return ret;
}

/* 
* F32 / FLOAT 
*/
GtF32 gontiMathSin(GtF32 x) {
	return sinf(x);
}
GtF32 gontiMathCos(GtF32 x) {
	return cosf(x);
}
GtF32 gontiMathTan(GtF32 x) {
	return tanf(x);
}
GtF32 gontiMathAcos(GtF32 x) {
	return acosf(x);
}
GtF32 gontiMathSqrt(GtF32 x) {
	return sqrtf(x);
}
GtF32 gontiMathAbs(GtF32 x) {
	return fabsf(x);
}
GtF32 gontiMathRandomFloat(void) {
	return (GtF32)gontiMathRandom() / (GtF32)RAND_MAX;
}
GtF32 gontiMathRandomFloatInRange(GtF32 min, GtF32 max) {
	return (GtF32)gontiMathRandom() / ((GtF32)RAND_MAX / (max - min));
}

/* 
* I32 / INT 
*/
GtI32 gontiMathRandom(void) {
	if (!randSeeded) {
		srand((GtU32)gontiPlatformGetAbsoluteTime());
		randSeeded = GtTrue;
	}

	return rand();
}
GtI32 gontiMathRandomInRange(GtI32 min, GtI32 max) {
	if (!randSeeded) {
		srand((GtU32)gontiPlatformGetAbsoluteTime());
		randSeeded = GtTrue;
	}

	return (rand() % (max - min + 1)) + min;
}