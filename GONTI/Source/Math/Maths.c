#include "Maths.h"

#ifdef Maths_ON

#include <stdio.h>
#include <stdlib.h>

/*
* }]===========================================[{ PRIVATE }]===========================================
*/

/*
* VOID
*/
void swap(int* a, int* b) {
	*a ^= *b;
	*b = *a ^ *b;
	*a ^= *b;
}
void swapFloat(float* a, float* b) {
	swap((int*)a, (int*)b);
}
void printUintArray(unsigned int* arr, unsigned int noElements) {
	printf("\n[ ");

	for (unsigned int i = 0; i < noElements; i++) {
		printf("%d ", arr[i]);
	}

	printf("]\n");
}
void divMod(unsigned int num, unsigned int divisor, unsigned int* quotient, unsigned int* remainder) {
	div_t res = div(num, divisor);
	*quotient = res.quot;
	*remainder = res.rem;
}
void divModLL(unsigned long long num, unsigned long long divisor, unsigned long long* quotient, unsigned long long* remainder) {
	lldiv_t res = lldiv(num, divisor);
	*quotient = res.quot;
	*remainder = res.rem;
}

/*
* BOOL
*/
bool containsUint(unsigned int* arr, unsigned int noElements, unsigned int target) {
	for (unsigned int i = 0; i < noElements; i++) {
		if (arr[i] == target) return true;
	}

	return false;
}

/*
* UNSIGNED INT
*/
unsigned int numDigits(unsigned int val, unsigned int base) {
	if (base <= 1) return 0;

	unsigned int ret = 0;

	while (val) {
		ret++;
		val /= base;
	}

	return ret;
}

/*
* UNSIGNED LONG LONG
*/
unsigned long long numDigitsLL(unsigned long long val, unsigned long long base) {
	if (base <= 1LL) return 0;

	unsigned long long ret = 0;

	while (val) {
		ret++;
		val /= base;
	}

	return ret;
}

/*
* ============================================[{ PUBLIC }]============================================
*/

/*m_math*/
Maths m_math = {
	/*VOID*/
	.swap = swap,
	.swapFloat = swapFloat,
	.printUintArray = printUintArray,
	.divMod = divMod,
	.divModLL = divModLL,

	/*BOOL*/
	.containsUint = containsUint,

	/*UNSIGNED INT*/
	.numDigits = numDigits,

	/*UNSIGNED LONG LONG*/
	.numDigitsLL = numDigitsLL
};

#endif