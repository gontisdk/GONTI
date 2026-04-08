#ifndef GtMatTypesINL
#define GtMatTypesINL

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../../../Defines/GtDefines.h"
        #include "../Vectors/GtVecTypes.inl"
        #include <stdalign.h>
        #include <xmmintrin.h>

        typedef struct {
			GtU32 rows;
			GtU32 cols;
			GtF32** elements;
		}GtMat;

        typedef union GtMat4_u {
            #if defined(GT_USE_SIMD)
                alignas(16) GtVec4 rows[4];
            #endif

            alignas(16) GtF32 data[16];
        } GtMat4;

#ifdef __cplusplus
    }
#endif

#endif