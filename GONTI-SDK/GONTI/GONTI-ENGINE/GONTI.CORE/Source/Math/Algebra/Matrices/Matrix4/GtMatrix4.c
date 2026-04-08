#include "GtMatrix4.h"
#include "../../../../Memory/GtMemory.h"
#include "../../Vectors/Vector3/GtVector3.h"

/*
* MAT4
*/
GtMat4 gontiMat4Identity() {
    GtMat4 ret;
    gt_zeroMemory(ret.data, sizeof(GtF32) * 16);

    ret.data[0] = 1.0f;
    ret.data[5] = 1.0f;
    ret.data[10] = 1.0f;
    ret.data[15] = 1.0f;

    return ret;
}
GtMat4 gontiMat4Multiplication(GtMat4 m1, GtMat4 m2) {
    GtMat4 ret = gontiMat4Identity();

    const GtF32* m1Ptr = m1.data;
    const GtF32* m2Ptr = m2.data;
    GtF32* retPtr = ret.data;

    for (GtI32 i = 0; i < 4; i++) {
        for (GtI32 j = 0; j < 4; j++) {
            *retPtr =
                m1Ptr[0] * m2Ptr[0 + j] +
                m1Ptr[1] * m2Ptr[4 + j] +
                m1Ptr[2] * m2Ptr[8 + j] +
                m1Ptr[3] * m2Ptr[12 + j];
            retPtr++;
        }

        m1Ptr += 4;
    }

    return ret;
}
GtMat4 gontiMat4Orthographic(GtF32 left, GtF32 right, GtF32 bottom, GtF32 top, GtF32 nearClip, GtF32 farClip) {
    GtMat4 ret = gontiMat4Identity();

    GtF32 lr = 1.0f / (left - right);
    GtF32 bt = 1.0f / (bottom - top);
    GtF32 nf = 1.0f / (nearClip - farClip);

    ret.data[0] = -2.0f * lr;
    ret.data[5] = -2.0f * bt;
    ret.data[10] = 2.0f * nf;

    ret.data[12] = (left + right) * lr;
    ret.data[13] = (top + bottom) * bt;
    ret.data[14] = (farClip + nearClip) * nf;

    return ret;
}
GtMat4 gontiMat4Perspective(GtF32 fovRadians, GtF32 aspectRatio, GtF32 nearClip, GtF32 farClip) {
    GtF32 halfTanFov = gontiMathTan(fovRadians * 0.5f);

    GtMat4 ret;
    gt_zeroMemory(ret.data, sizeof(GtF32) * 16);

    ret.data[0] = 1.0f / (aspectRatio * halfTanFov);
    ret.data[5] = 1.0f / halfTanFov;
    ret.data[10] = -((farClip + nearClip) / (farClip - nearClip));
    ret.data[11] = -1.0f;
    ret.data[14] = -((2.0f * farClip * nearClip) / (farClip - nearClip));

    return ret;
}
GtMat4 gontiMat4LookAt(GtVec3 position, GtVec3 target, GtVec3 up) {
    GtMat4 ret;

    GtVec3 zAxis;
    zAxis.x = target.x - position.x;
    zAxis.y = target.y - position.y;
    zAxis.z = target.z - position.z;
    zAxis = gontiVec3Normalized(zAxis);

    GtVec3 xAxis = gontiVec3Normalized(gontiVec3Cross(zAxis, up));
    GtVec3 yAxis = gontiVec3Cross(xAxis, zAxis);

    ret.data[0] = xAxis.x;
    ret.data[1] = yAxis.x;
    ret.data[2] = -zAxis.x;
    ret.data[3] = 0;
    ret.data[4] = xAxis.y;
    ret.data[5] = yAxis.y;
    ret.data[6] = -zAxis.y;
    ret.data[7] = 0;
    ret.data[8] = xAxis.z;
    ret.data[9] = yAxis.z;
    ret.data[10] = -zAxis.z;
    ret.data[11] = 0;
    ret.data[12] = gontiVec3Dot(xAxis, position);
    ret.data[13] = gontiVec3Dot(yAxis, position);
    ret.data[14] = gontiVec3Dot(zAxis, position);
    ret.data[15] = 1.0f;

    return ret;
}
GtMat4 gontiMat4Inverse(GtMat4 m) {
    const GtF32* mf = m.data;

    GtF32 t0 = mf[10] * mf[15];
    GtF32 t1 = mf[14] * mf[11];
    GtF32 t2 = mf[6] * mf[15];
    GtF32 t3 = mf[14] * mf[7];
    GtF32 t4 = mf[6] * mf[11];
    GtF32 t5 = mf[10] * mf[7];
    GtF32 t6 = mf[2] * mf[15];
    GtF32 t7 = mf[14] * mf[3];
    GtF32 t8 = mf[2] * mf[11];
    GtF32 t9 = mf[10] * mf[3];
    GtF32 t10 = mf[2] * mf[7];
    GtF32 t11 = mf[6] * mf[3];
    GtF32 t12 = mf[8] * mf[13];
    GtF32 t13 = mf[12] * mf[9];
    GtF32 t14 = mf[4] * mf[13];
    GtF32 t15 = mf[12] * mf[5];
    GtF32 t16 = mf[4] * mf[9];
    GtF32 t17 = mf[8] * mf[5];
    GtF32 t18 = mf[0] * mf[13];
    GtF32 t19 = mf[12] * mf[1];
    GtF32 t20 = mf[0] * mf[9];
    GtF32 t21 = mf[8] * mf[1];
    GtF32 t22 = mf[0] * mf[5];
    GtF32 t23 = mf[4] * mf[1];

    GtMat4 ret;
    GtF32* o = ret.data;

    o[0] = (t0 * mf[5] + t3 * mf[9] + t4 * mf[13]) -
           (t1 * mf[5] + t2 * mf[9] + t5 * mf[13]);
    o[1] = (t1 * mf[1] + t6 * mf[9] + t9 * mf[13]) -
           (t0 * mf[1] + t7 * mf[9] + t8 * mf[13]);
    o[2] = (t2 * mf[1] + t7 * mf[5] + t10 * mf[13]) -
           (t3 * mf[1] + t6 * mf[5] + t11 * mf[13]);
    o[3] = (t5 * mf[1] + t8 * mf[5] + t11 * mf[9]) -
           (t4 * mf[1] + t9 * mf[5] + t10 * mf[9]);

    GtF32 d = 1.0f / (mf[0] * o[0] + mf[4] * o[1] + mf[8] * o[2] + mf[12] * o[3]);

    if (gontiMathAbs(d) < 1e-6f) {
        return gontiMat4Identity();
    }

    o[0] = d * o[0];
    o[1] = d * o[1];
    o[2] = d * o[2];
    o[3] = d * o[3];
    o[4] = d * ((t1 * mf[4] + t2 * mf[8] + t5 * mf[12]) -
                (t0 * mf[4] + t3 * mf[8] + t4 * mf[12]));
    o[5] = d * ((t0 * mf[0] + t7 * mf[8] + t8 * mf[12]) -
                (t1 * mf[0] + t6 * mf[8] + t9 * mf[12]));
    o[6] = d * ((t3 * mf[0] + t6 * mf[4] + t11 * mf[12]) -
                (t2 * mf[0] + t7 * mf[4] + t10 * mf[12]));
    o[7] = d * ((t4 * mf[0] + t9 * mf[4] + t10 * mf[8]) -
                (t5 * mf[0] + t8 * mf[4] + t11 * mf[8]));
    o[8] = d * ((t12 * mf[7] + t15 * mf[11] + t16 * mf[15]) -
                (t13 * mf[7] + t14 * mf[11] + t17 * mf[15]));
    o[9] = d * ((t13 * mf[3] + t18 * mf[11] + t21 * mf[15]) -
                (t12 * mf[3] + t19 * mf[11] + t20 * mf[15]));
    o[10] = d * ((t14 * mf[3] + t19 * mf[7] + t22 * mf[15]) -
                 (t15 * mf[3] + t18 * mf[7] + t23 * mf[15]));
    o[11] = d * ((t17 * mf[3] + t20 * mf[7] + t23 * mf[11]) -
                 (t16 * mf[3] + t21 * mf[7] + t22 * mf[11]));
    o[12] = d * ((t14 * mf[10] + t17 * mf[14] + t13 * mf[6]) -
                 (t16 * mf[14] + t12 * mf[6] + t15 * mf[10]));
    o[13] = d * ((t20 * mf[14] + t12 * mf[2] + t19 * mf[10]) -
                 (t18 * mf[10] + t21 * mf[14] + t13 * mf[2]));
    o[14] = d * ((t18 * mf[6] + t23 * mf[14] + t15 * mf[2]) -
                 (t22 * mf[14] + t14 * mf[2] + t19 * mf[6]));
    o[15] = d * ((t22 * mf[10] + t16 * mf[2] + t21 * mf[6]) -
                 (t20 * mf[6] + t23 * mf[10] + t17 * mf[2]));

    return ret;
}
GtMat4 gontiMat4Transposed(GtMat4 m) {
    GtMat4 ret = gontiMat4Identity();

    ret.data[0] = m.data[0];
    ret.data[1] = m.data[4];
    ret.data[2] = m.data[8];
    ret.data[3] = m.data[12];
    ret.data[4] = m.data[1];
    ret.data[5] = m.data[5];
    ret.data[6] = m.data[9];
    ret.data[7] = m.data[13];
    ret.data[8] = m.data[2];
    ret.data[9] = m.data[6];
    ret.data[10] = m.data[10];
    ret.data[11] = m.data[14];
    ret.data[12] = m.data[3];
    ret.data[13] = m.data[7];
    ret.data[14] = m.data[11];
    ret.data[15] = m.data[15];

    return ret;
}
GtMat4 gontiMat4Translation(GtVec3 position) {
    GtMat4 ret = gontiMat4Identity();

    ret.data[12] = position.x;
    ret.data[13] = position.y;
    ret.data[14] = position.z;

    return ret;
}
GtMat4 gontiMat4Scale(GtVec3 scale) {
    GtMat4 ret = gontiMat4Identity();

    ret.data[0] = scale.x;
    ret.data[5] = scale.y;
    ret.data[10] = scale.z;

    return ret;
}
GtMat4 gontiMat4EulerX(GtF32 angle_radians) {
    GtMat4 ret = gontiMat4Identity();
    GtF32 c = gontiMathCos(angle_radians);
    GtF32 s = gontiMathSin(angle_radians);

    ret.data[5] = c;
    ret.data[6] = s;
    ret.data[9] = -s;
    ret.data[10] = c;

    return ret;
}
GtMat4 gontiMat4EulerY(GtF32 angle_radians) {
    GtMat4 ret = gontiMat4Identity();
    GtF32 c = gontiMathCos(angle_radians);
    GtF32 s = gontiMathSin(angle_radians);

    ret.data[0] = c;
    ret.data[2] = -s;
    ret.data[8] = s;
    ret.data[10] = c;

    return ret;
}
GtMat4 gontiMat4EulerZ(GtF32 angle_radians) {
    GtMat4 ret = gontiMat4Identity();
    GtF32 c = gontiMathCos(angle_radians);
    GtF32 s = gontiMathSin(angle_radians);

    ret.data[0] = c;
    ret.data[1] = s;
    ret.data[4] = -s;
    ret.data[5] = c;

    return ret;
}
GtMat4 gontiMat4EulerXYZ(GtF32 xRadians, GtF32 yRadians, GtF32 zRadians) {
    GtMat4 rx = gontiMat4EulerX(xRadians);
    GtMat4 ry = gontiMat4EulerY(yRadians);
    GtMat4 rz = gontiMat4EulerZ(zRadians);

    GtMat4 ret = gontiMat4Multiplication(rx, ry);
    ret = gontiMat4Multiplication(ret, rz);

    return ret;
}

/*
* VEC3
*/
GtVec3 gontiMat4Forward(GtMat4 m) {
    GtVec3 ret;

    ret.x = -m.data[2];
    ret.y = -m.data[6];
    ret.z = -m.data[10];

    gontiVec3Normalize(&ret);
    return ret;
}
GtVec3 gontiMat4Backward(GtMat4 m) {
    GtVec3 ret;

    ret.x = m.data[2];
    ret.y = m.data[6];
    ret.z = m.data[10];

    gontiVec3Normalize(&ret);
    return ret;
}
GtVec3 gontiMat4Up(GtMat4 m) {
    GtVec3 ret;

    ret.x = m.data[1];
    ret.y = m.data[5];
    ret.z = m.data[9];

    gontiVec3Normalize(&ret);
    return ret;
}
GtVec3 gontiMat4Down(GtMat4 m) {
    GtVec3 ret;

    ret.x = -m.data[1];
    ret.y = -m.data[5];
    ret.z = -m.data[9];

    gontiVec3Normalize(&ret);
    return ret;
}
GtVec3 gontiMat4Left(GtMat4 m) {
    GtVec3 ret;

    ret.x = -m.data[0];
    ret.y = -m.data[4];
    ret.z = -m.data[8];

    gontiVec3Normalize(&ret);
    return ret;
}
GtVec3 gontiMat4Right(GtMat4 m) {
    GtVec3 ret;

    ret.x = m.data[0];
    ret.y = m.data[4];
    ret.z = m.data[8];

    gontiVec3Normalize(&ret);
    return ret;
}