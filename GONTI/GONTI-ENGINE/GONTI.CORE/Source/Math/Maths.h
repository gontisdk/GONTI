#ifndef MATHS_H
#define MATHS_H

#ifdef __cplusplus
extern "C" {
#endif

	/*=============================================={[ CODE ]}==============================================*/

	#include "../Defines/Defines.h"

	// Global Defines
	#define NUMARGS(type, ...) (sizeof((type[]){(type)0, ##__VA_ARGS__}) / sizeof(type) - 1)

	#ifdef MAX
		#undef MAX
	#endif
	#ifndef MAX
		#define MAX(i1, i2) ((i1) > (i2) ? (i1) : (i2))
	#endif

	#ifdef MIN
		#undef MIN
	#endif
	#ifndef MIN
		#define MIN(i1, i2) ((i1) < (i2) ? (i1) : (i2))
	#endif

	// Functions
	typedef struct {
		/*VOID*/
		void(*swap)(int* a, int* b);
		void(*swapFloat)(float* a, float* b);
		void(*printUintArray)(unsigned int* arr, unsigned int noElements);
		void(*divMod)(unsigned int num, unsigned int divisor, unsigned int* quotient, unsigned int* remainder);
		void(*divModLL)(unsigned long long num, unsigned long long divisor, unsigned long long* quotient, unsigned long long* remainder);

		/*BOOL*/
		bool(*containsUint)(unsigned int* arr, unsigned int noElements, unsigned int target);

		/*UNSIGNED INT*/
		unsigned int(*numDigits)(unsigned int val, unsigned int base);

		/*UNSIGNED LONG LONG*/
		unsigned long long(*numDigitsLL)(unsigned long long val, unsigned long long base);
	}gontiMath;
	extern KAPI gontiMath gonti_math;

	/*=============================================={[ !CODE ]}==============================================*/

#ifdef __cplusplus
}
#endif

#endif // !MATHS_H