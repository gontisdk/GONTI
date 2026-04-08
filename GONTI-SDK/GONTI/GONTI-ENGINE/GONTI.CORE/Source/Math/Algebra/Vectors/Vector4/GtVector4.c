#include "GtVector4.h"

/*
* VEC4
*/
GtVec4 gontiVec4Create(GtF32 x, GtF32 y, GtF32 z, GtF32 w) {
    GtVec4 ret;

    #if defined(GT_USE_SIMD)
        ret.data = _mm_setr_ps(x, y, z, w);
    #else
        ret.x = x;
        ret.y = y;
        ret.z = z;
        ret.w = w;
    #endif

    return ret;
}
GtVec4 gontiVec4FromVec3(GtVec3 v, GtF32 w) {
    #if defined(GT_USE_SIMD)
        GtVec4 ret;
        ret.data = _mm_setr_ps(v.x, v.y, v.z, w);
        return ret;
    #else
        return (GtVec4){.elements = {v.x, v.y, v.z, w}};
    #endif
}
GtVec4 gontiVec4Zero() {
    return (GtVec4){.elements = {0.0f, 0.0f, 0.0f, 0.0f}};
}
GtVec4 gontiVec4One() {
    return (GtVec4){.elements = {1.0f, 1.0f, 1.0f, 1.0f}};
}
GtVec4 gontiVec4Add(GtVec4 v1, GtVec4 v2) {
    GtVec4 ret;

    for (GtU64 i = 0; i < 4; i++) {
        ret.elements[i] = v1.elements[i] + v2.elements[i];
    }

    return ret;
}
GtVec4 gontiVec4Subtract(GtVec4 v1, GtVec4 v2) {
    GtVec4 ret;

    for (GtU64 i = 0; i < 4; i++) {
        ret.elements[i] = v1.elements[i] - v2.elements[i];
    }

    return ret;
}
GtVec4 gontiVec4Multiply(GtVec4 v1, GtVec4 v2) {
    GtVec4 ret;

    for (GtU64 i = 0; i < 4; i++) {
        ret.elements[i] = v1.elements[i] * v2.elements[i];
    }

    return ret;
}
GtVec4 gontiVec4Divide(GtVec4 v1, GtVec4 v2) {
    GtVec4 ret;

    for (GtU64 i = 0; i < 4; i++) {
        ret.elements[i] = v1.elements[i] * v2.elements[i];
    }

    return ret;
}
GtVec4 gontiVec4Normalized(GtVec4 v) {
    gontiVec4Normalize(&v);
    return v;
}

/*
* VEC3
*/
GtVec3 gontiVec4ToVec3(GtVec4 v) {
    return (GtVec3){.elements = {v.x, v.y, v.z}};
}

/*
* F32 / FLOAT
*/
GtF32 gontiVec4LengthSquared(GtVec4 v) {
    return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
}
GtF32 gontiVec4Length(GtVec4 v) {
    return gontiMathSqrt(gontiVec4LengthSquared(v));
}
GtF32 gontiVec4DotF32(GtF32 a0, GtF32 a1, GtF32 a2, GtF32 a3, GtF32 b0, GtF32 b1, GtF32 b2, GtF32 b3) {
    GtF32 ret;
    
    ret =
        a0 * b0 +
        a1 * b1 +
        a2 * b2 +
        a3 * b3;

    return ret;
}

/*
* VOID
*/
void gontiVec4Normalize(GtVec4* v) {
    const GtF32 length = gontiVec4Length(*v);
    v->x /= length;
    v->y /= length;
    v->z /= length;
    v->w /= length;
}