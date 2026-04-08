#include "GtQuaternion.h"

#include "../../../GtMaths.h"
#include "../../Matrices/Matrix4/GtMatrix4.h"

/*
* QUAT
*/
GtQuat gontiQuatIdentity() {
    return (GtQuat){.elements = {0, 0, 0, 1.0f}};
}
GtQuat gontiQuatNormalize(GtQuat q) {
    GtF32 n = gontiQuatNormal(q);

    return (GtQuat){.elements = {q.x / n, q.y / n, q.z / n, q.w / n}};
}
GtQuat gontiQuatConjugate(GtQuat q) {
    return (GtQuat){.elements = {-q.x, -q.y, -q.z, -q.w}};
}
GtQuat gontiQuatInverse(GtQuat q) {
    return gontiQuatNormalize(gontiQuatConjugate(q));
}
GtQuat gontiQuatMultiply(GtQuat q1, GtQuat q2) {
    GtQuat ret;

    ret.x = q1.x * q2.w + 
            q1.y * q2.z - 
            q1.z * q2.y + 
            q1.w * q2.x;

    ret.y = -q1.x * q2.z + 
            q1.y * q2.w + 
            q1.z * q2.x + 
            q1.w * q2.y;

    ret.z = q1.x * q2.y - 
            q1.y * q2.x + 
            q1.z * q2.w + 
            q1.w * q2.z;

    ret.w = -q1.x * q2.x - 
            q1.y * q2.y - 
            q1.z * q2.z + 
            q1.w * q2.w;

    return ret;
}
GtQuat gontiQuatFromAxisAngle(GtVec3 axis, GtF32 angle, GtB8 normalize) {
    const GtF32 halfAngle = 0.5f * angle;
    GtF32 s = gontiMathSin(halfAngle);
    GtF32 c = gontiMathCos(halfAngle);

    GtQuat ret = (GtQuat){.elements = {s * axis.x, s * axis.y, s * axis.z, c}};
    if (normalize) return gontiQuatNormalize(ret);

    return ret;
}
GtQuat gontiQuatSlerp(GtQuat q1, GtQuat q2, GtF32 percentage) {
    GtQuat ret;
    GtQuat v0 = gontiQuatNormalize(q1);
    GtQuat v1 = gontiQuatNormalize(q2);
    GtF32 dot = gontiQuatDot(v0, v1);

    if (dot < 0.0f) {
        v1.x = -v1.x;
        v1.y = -v1.y;
        v1.z = -v1.z;
        v1.w = -v1.w;
        dot = -dot;
    }

    const GtF32 DOT_THRESHOLD = 0.9995f;
    if (dot > DOT_THRESHOLD) {
        ret = (GtQuat){.elements = {
            v0.x + ((v1.x - v0.x) * percentage),
            v0.y + ((v1.y - v0.y) * percentage),
            v0.z + ((v1.z - v0.z) * percentage),
            v0.w + ((v1.w - v0.w) * percentage),
        }};
        return gontiQuatNormalize(ret);
    }

    GtF32 theta0 = gontiMathAcos(dot);
    GtF32 theta = theta0 * percentage;
    GtF32 sinTheta = gontiMathSin(theta);
    GtF32 sinTheta0 = gontiMathSin(theta0);
    GtF32 s0 = gontiMathCos(theta) - dot * sinTheta / sinTheta0;
    GtF32 s1 = sinTheta / sinTheta0;

    return (GtQuat){.elements = {
        (v0.x * s0) + (v1.x * s1),
        (v0.y * s0) + (v1.y * s1),
        (v0.z * s0) + (v1.z * s1),
        (v0.w * s0) + (v1.w * s1),
    }};
}

/*
* F32
*/
GtF32 gontiQuatNormal(GtQuat q) {
    return gontiMathSqrt(
        q.x * q.x +
        q.y * q.y +
        q.z * q.z +
        q.w * q.w
    );
}
GtF32 gontiQuatDot(GtQuat q1, GtQuat q2) {
    return q1.x * q2.x +
            q1.y * q2.y +
            q1.z * q2.z +
            q1.w * q2.w;
}
GtF32 gontiQuatDegToRad(GtF32 degrees) {
    return degrees * GT_DEG2RAD_MULTIPLIER;
}
GtF32 gontiQuatRadToDeg(GtF32 radians) {
    return radians * GT_RAD2DEG_MULTIPLIER;
}

/*
* MAT4
*/
GtMat4 gontiQuatToMat4(GtQuat q) {
    GtMat4 ret = gontiMat4Identity();
    GtQuat n = gontiQuatNormalize(q);

    ret.data[0] = 1.0f - 2.0f * n.y * n.y - 2.0f * n.z * n.z;
    ret.data[1] = 2.0f * n.x * n.y - 2.0f * n.z * n.w;
    ret.data[2] = 2.0f * n.x * n.z + 2.0f * n.y * n.w;

    ret.data[4] = 2.0f * n.x * n.y + 2.0f * n.z * n.w;
    ret.data[5] = 1.0f - 2.0f * n.x * n.x - 2.0f * n.z * n.z;
    ret.data[6] = 2.0f * n.y * n.z - 2.0f * n.x * n.w;

    ret.data[8] = 2.0f * n.x * n.z - 2.0f * n.y * n.w;
    ret.data[9] = 2.0f * n.y * n.z + 2.0f * n.x * n.w;
    ret.data[10] = 1.0f - 2.0f * n.x * n.x - 2.0f * n.y * n.y;

    return ret;
}
GtMat4 gontiQuatToRotationMat4(GtQuat q, GtVec3 center) {
    GtMat4 ret;
    GtF32* o = ret.data;

    o[0] = (q.x * q.x) - (q.y * q.y) - (q.z * q.z) - (q.w * q.w);
    o[1] = 2.0f * ((q.x * q.y) + (q.z * q.w));
    o[2] = 2.0f * ((q.x * q.z) - (q.y * q.w));
    o[3] = center.x - center.x * o[0] - center.y * o[1] - center.z * o[2];

    o[4] = 2.0f * ((q.x * q.y) - (q.z * q.w));
    o[5] = -(q.x * q.x) + (q.y * q.y) + (q.z * q.z) + (q.w * q.w);
    o[6] = 2.0f * ((q.y * q.z) + (q.x * q.w));
    o[7] = center.y - center.x * o[4] - center.y * o[5] - center.z * o[6];
    
    o[8] = 2.0f * ((q.x * q.z) + (q.y * q.w));
    o[9] = 2.0f * ((q.y * q.z) - (q.x * q.w));
    o[10] = -(q.x * q.x) - (q.y * q.y) + (q.z * q.z) + (q.w * q.w);
    o[11] = center.z - center.x * o[8] - center.y * o[9] - center.z * o[10];

    o[12] = 0.0f;
    o[13] = 0.0f;
    o[14] = 0.0f;
    o[15] = 1.0f;

    return ret;
}