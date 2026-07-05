#ifndef GtVector4H
#define GtVector4H

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GtMath.h>
        #include <Algebra/Vectors/GtVecTypes.inl>

        /* VEC4 */
        GTAPI GtVec4 gontiVec4Create(GtF32 x, GtF32 y, GtF32 z, GtF32 w);
        GTAPI GtVec4 gontiVec4FromVec3(GtVec3 v, GtF32 w);
        GTAPI GtVec4 gontiVec4Zero(void);
        GTAPI GtVec4 gontiVec4One(void);
        GTAPI GtVec4 gontiVec4Add(GtVec4 v1, GtVec4 v2);
        GTAPI GtVec4 gontiVec4Subtract(GtVec4 v1, GtVec4 v2);
        GTAPI GtVec4 gontiVec4Multiply(GtVec4 v1, GtVec4 v2);
		GTAPI GtVec4 gontiVec4Divide(GtVec4 v1, GtVec4 v2);
        GTAPI GtVec4 gontiVec4Normalized(GtVec4 v);

        /* VEC3 */
        GTAPI GtVec3 gontiVec4ToVec3(GtVec4 v);

        /* F32 / FLOAT */
        GTAPI GtF32 gontiVec4LengthSquared(GtVec4 v);
        GTAPI GtF32 gontiVec4Length(GtVec4 v);
        GTAPI GtF32 gontiVec4DotF32(GtF32 a0, GtF32 a1, GtF32 a2, GtF32 a3, GtF32 b0, GtF32 b1, GtF32 b2, GtF32 b3);

        /* VOID */
        GTAPI void gontiVec4Normalize(GtVec4* v);

#ifdef __cplusplus
    }
#endif

#endif