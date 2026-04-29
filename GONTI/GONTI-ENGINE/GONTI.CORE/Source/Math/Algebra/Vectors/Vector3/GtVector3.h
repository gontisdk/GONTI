#ifndef GtVector3H
#define GtVector3H

#ifdef __cplusplus
    extern "C" {
#endif

        #include <Math/GtMath.h>
        #include <Math/Algebra/Vectors/GtVecTypes.inl>

        /* VEC2 */
        GTAPI GtVec3 gontiVec3Create(GtF32 x, GtF32 y, GtF32 z);
        GTAPI GtVec3 gontiVec3FromVec4(GtVec4 v);
        GTAPI GtVec3 gontiVec3Zero(void);
        GTAPI GtVec3 gontiVec3One(void);
        GTAPI GtVec3 gontiVec3Up(void);
        GTAPI GtVec3 gontiVec3Down(void);
        GTAPI GtVec3 gontiVec3Left(void);
        GTAPI GtVec3 gontiVec3Right(void);
        GTAPI GtVec3 gontiVec3Forward(void);
        GTAPI GtVec3 gontiVec3Backward(void);
        GTAPI GtVec3 gontiVec3Add(GtVec3 v1, GtVec3 v2);
        GTAPI GtVec3 gontiVec3Subtract(GtVec3 v1, GtVec3 v2);
        GTAPI GtVec3 gontiVec3Multiply(GtVec3 v1, GtVec3 v2);
        GTAPI GtVec3 gontiVec3MultiplyScalar(GtVec3 v, GtF32 scalar);
		GTAPI GtVec3 gontiVec3Divide(GtVec3 v1, GtVec3 v2);
        GTAPI GtVec3 gontiVec3Normalized(GtVec3 v);
        GTAPI GtVec3 gontiVec3Cross(GtVec3 v1, GtVec3 v2);

        /* VEC4 */
        GTAPI GtVec4 gontiVec3ToVec4(GtVec3 v, GtF32 w);

        /* FLOAT / F32 */
        GTAPI GtF32 gontiVec3LengthSquared(GtVec3 v);
        GTAPI GtF32 gontiVec3Length(GtVec3 v);
        GTAPI GtF32 gontiVec3Distance(GtVec3 v1, GtVec3 v2);
        GTAPI GtF32 gontiVec3Dot(GtVec3 v1, GtVec3 v2);

        /* VOID */
        GTAPI void gontiVec3Normalize(GtVec3* v);

        /* BOOL / B8 */
        GTAPI GtB8 gontiVec3Compare(GtVec3 v1, GtVec3 v2, GtF32 tolerance);

#ifdef __cplusplus
    }
#endif

#endif