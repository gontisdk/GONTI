#ifndef GtMatrix4H
#define GtMatrix4H

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../GtMatTypes.inl"
        #include "../../../GtMaths.h"

        /* MAT4 */
        GTAPI GtMat4 gontiMat4Identity();
        GTAPI GtMat4 gontiMat4Multiplication(GtMat4 m1, GtMat4 m2);
        GTAPI GtMat4 gontiMat4Orthographic(GtF32 left, GtF32 right, GtF32 bottom, GtF32 top, GtF32 nearClip, GtF32 farClip);
        GTAPI GtMat4 gontiMat4Perspective(GtF32 fovRadians, GtF32 aspectRatio, GtF32 nearClip, GtF32 farClip);
        GTAPI GtMat4 gontiMat4LookAt(GtVec3 position, GtVec3 target, GtVec3 up);
        GTAPI GtMat4 gontiMat4Inverse(GtMat4 m);
        GTAPI GtMat4 gontiMat4Transposed(GtMat4 m);
        GTAPI GtMat4 gontiMat4Translation(GtVec3 position);
        GTAPI GtMat4 gontiMat4Scale(GtVec3 scale);
        GTAPI GtMat4 gontiMat4EulerX(GtF32 angle_radians);
        GTAPI GtMat4 gontiMat4EulerY(GtF32 angle_radians);
        GTAPI GtMat4 gontiMat4EulerZ(GtF32 angle_radians);
        GTAPI GtMat4 gontiMat4EulerXYZ(GtF32 xRadians, GtF32 yRadians, GtF32 zRadians);

        /* VEC3 */
        GTAPI GtVec3 gontiMat4Forward(GtMat4 m);
        GTAPI GtVec3 gontiMat4Backward(GtMat4 m);
        GTAPI GtVec3 gontiMat4Up(GtMat4 m);
        GTAPI GtVec3 gontiMat4Down(GtMat4 m);
        GTAPI GtVec3 gontiMat4Left(GtMat4 m);
        GTAPI GtVec3 gontiMat4Right(GtMat4 m);

#ifdef __cplusplus
    }
#endif

#endif