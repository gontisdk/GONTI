#ifndef VECTORSTYPES_INL
#define VECTORSTYPES_INL

#ifdef __cplusplus
    extern "C" {
#endif

        #include <Defines/GtDefines.inl>
        #include <stdalign.h>
        #include <xmmintrin.h>

        typedef struct {
			GtU32 dim;
			GtF32* elements;
		}GtVec;

        typedef union GtVec2_u {
            GtF32 elements[2];
            struct {
                union {
                    GtF32 x, r, s, u;
                };
                union {
                    GtF32 y, g, t, v;
                };
            };
        } GtVec2;

        typedef union GtVec3_u {
            GtF32 elements[3];
            struct {
                union {
                    GtF32 x, r, s, u;
                };
                union {
                    GtF32 y, g, t, v;
                };
                union {
                    GtF32 z, b, p, w;
                };
            };
        } GtVec3;

        typedef union GtVec4_u {
            #if defined(GT_USE_SIMD)
                alignas(16) __m128 data;
            #endif

            alignas(16) GtF32 elements[4];
            struct {
                union {
                    GtF32 x, r, s;
                };
                union {
                    GtF32 y, g, t;
                };
                union {
                    GtF32 z, b, p;
                };
                union {
                    GtF32 w, a, q;
                };
            };
        } GtVec4; 

#ifdef __cplusplus
    }
#endif

#endif