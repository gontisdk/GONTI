#ifndef GtQuaternionH
#define GtQuaternionH

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../GtQuatTypes.inl"
        #include "../../Matrices/GtMatTypes.inl"

        /* QUAT */
        GTAPI GtQuat gontiQuatIdentity();
        GTAPI GtQuat gontiQuatNormalize(GtQuat q);
        GTAPI GtQuat gontiQuatConjugate(GtQuat q);
        GTAPI GtQuat gontiQuatInverse(GtQuat q);
        GTAPI GtQuat gontiQuatMultiply(GtQuat q1, GtQuat q2);
        GTAPI GtQuat gontiQuatFromAxisAngle(GtVec3 axis, GtF32 angle, GtB8 normalize);
        GTAPI GtQuat gontiQuatSlerp(GtQuat q1, GtQuat q2, GtF32 percentage);

        /* F32 */
        GTAPI GtF32 gontiQuatNormal(GtQuat q);
        GTAPI GtF32 gontiQuatDot(GtQuat q1, GtQuat q2);
        GTAPI GtF32 gontiQuatDegToRad(GtF32 degrees);
        GTAPI GtF32 gontiQuatRadToDeg(GtF32 radians);

        /* MAT4 */
        GTAPI GtMat4 gontiQuatToMat4(GtQuat q);
        GTAPI GtMat4 gontiQuatToRotationMat4(GtQuat q, GtVec3 center);

#ifdef __cplusplus
    }
#endif

#endif