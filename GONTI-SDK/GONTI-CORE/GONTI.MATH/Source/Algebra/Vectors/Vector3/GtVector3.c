#include <Algebra/Vectors/Vector3/GtVector3.h>

/*
* VEC3
*/
GtVec3 gontiVec3Create(GtF32 x, GtF32 y, GtF32 z) {
    return (GtVec3){.elements = {x, y, z}};
}
GtVec3 gontiVec3FromVec4(GtVec4 v) {
    return (GtVec3){.elements = {v.x, v.y, v.z}};
}
GtVec3 gontiVec3Zero(void) {
    return (GtVec3){.elements = {0.0f, 0.0f, 0.0f}};
}
GtVec3 gontiVec3One(void) {
    return (GtVec3){.elements = {1.0f, 1.0f, 1.0f}};
}
GtVec3 gontiVec3Up(void) {
    return (GtVec3){.elements = {0.0f, 1.0f, 0.0f}};
}
GtVec3 gontiVec3Down(void) {
    return (GtVec3){.elements = {0.0f, -1.0f, 0.0f}};
}
GtVec3 gontiVec3Left(void) {
    return (GtVec3){.elements = {-1.0f, 0.0f, 0.0f}};
}
GtVec3 gontiVec3Right(void) {
    return (GtVec3){.elements = {1.0f, 0.0f, 0.0f}};
}
GtVec3 gontiVec3Forward(void) {
    return (GtVec3){.elements = {0.0f, 0.0f, -1.0f}};
}
GtVec3 gontiVec3Backward(void) {
    return (GtVec3){.elements = {0.0f, 0.0f, 1.0f}};
}
GtVec3 gontiVec3Add(GtVec3 v1, GtVec3 v2) {
    return (GtVec3){
        .elements = {
            v1.x + v2.x,
            v1.y + v2.y,
            v1.z + v2.z
        }
    };
}
GtVec3 gontiVec3Subtract(GtVec3 v1, GtVec3 v2) {
    return (GtVec3){
        .elements = {
            v1.x - v2.x,
            v1.y - v2.y,
            v1.z - v2.z
        }
    };
}
GtVec3 gontiVec3Multiply(GtVec3 v1, GtVec3 v2) {
    return (GtVec3){
        .elements = {
            v1.x * v2.x,
            v1.y * v2.y,
            v1.z * v2.z
        }
    };
}
GtVec3 gontiVec3MultiplyScalar(GtVec3 v, GtF32 scalar) {
    return (GtVec3){
        .elements = {
            v.x * scalar,
            v.y * scalar,
            v.z * scalar
        }
    };
}
GtVec3 gontiVec3Divide(GtVec3 v1, GtVec3 v2) {
    return (GtVec3){
        .elements = {
            v1.x / v2.x,
            v1.y / v2.y,
            v1.z / v2.z
        }
    };
}
GtVec3 gontiVec3Normalized(GtVec3 v) {
    gontiVec3Normalize(&v);
    return v;
}
GtVec3 gontiVec3Cross(GtVec3 v1, GtVec3 v2) {
    return (GtVec3){
        .elements = {
            v1.y * v2.z - v1.z * v2.y,
            v1.z * v2.x - v1.x * v2.z,
            v1.x * v2.y - v1.y * v2.x
        }
    };
}

/*
* VEC4
*/
GtVec4 gontiVec3ToVec4(GtVec3 v, GtF32 w) {
    return (GtVec4){.elements = {v.x, v.y, v.z, w}};
}

/*
* F32 / FLOAT
*/
GtF32 gontiVec3LengthSquared(GtVec3 v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}
GtF32 gontiVec3Length(GtVec3 v) {
    return gontiMathSqrt(gontiVec3LengthSquared(v));
}
GtF32 gontiVec3Distance(GtVec3 v1, GtVec3 v2) {
    GtVec3 d = (GtVec3) {
        .elements = {
            v1.x - v2.x,
            v1.y - v2.y,
            v1.z - v2.z
        }
    };

    return gontiVec3Length(d);
}
GtF32 gontiVec3Dot(GtVec3 v1, GtVec3 v2) {
    GtF32 ret = 0;

    ret += v1.x * v2.x;
    ret += v1.y * v2.y;
    ret += v1.z * v2.z;

    return ret;
}

/*
* VOID
*/
void gontiVec3Normalize(GtVec3* v) {
    const GtF32 length = gontiVec3Length(*v);
    v->x /= length;
    v->y /= length;
    v->z /= length;
}

/*
* B8 / BOOL
*/
GtB8 gontiVec3Compare(GtVec3 v1, GtVec3 v2, GtF32 tolerance) {
    if (gontiMathAbs(v1.x - v2.x) > tolerance)
        return GtFalse;

    if (gontiMathAbs(v1.y - v2.y) > tolerance)
        return GtFalse;
    
    if (gontiMathAbs(v1.z - v2.z) > tolerance)
        return GtFalse;

    return GtTrue;
}
