#include "GtVector2.h"

/*
* VEC2
*/
GtVec2 gontiVec2Create(GtF32 x, GtF32 y) {
    return (GtVec2){.elements = {x, y}};
}
GtVec2 gontiVec2Zero() {
    return (GtVec2){.elements = {0.0f, 0.0f}};
}
GtVec2 gontiVec2One() {
    return (GtVec2){.elements = {1.0f, 1.0f}};
}
GtVec2 gontiVec2Up() {
    return (GtVec2){.elements = {0.0f, 1.0f}};
}
GtVec2 gontiVec2Down() {
    return (GtVec2){.elements = {0.0f, -1.0f}};
}
GtVec2 gontiVec2Left() {
    return (GtVec2){.elements = {-1.0f, 0.0f}};
}
GtVec2 gontiVec2Right() {
    return (GtVec2){.elements = {1.0f, 0.0f}};
}
GtVec2 gontiVec2Add(GtVec2 v1, GtVec2 v2) {
    return (GtVec2){
        .elements = {
            v1.x + v2.x,
            v1.y + v2.y
        }
    };
}
GtVec2 gontiVec2Subtract(GtVec2 v1, GtVec2 v2) {
    return (GtVec2){
        .elements = {
            v1.x - v2.x,
            v1.y - v2.y
        }
    };
}
GtVec2 gontiVec2Multiply(GtVec2 v1, GtVec2 v2) {
    return (GtVec2){
        .elements = {
            v1.x * v2.x,
            v1.y * v2.y
        }
    };
}
GtVec2 gontiVec2Divide(GtVec2 v1, GtVec2 v2) {
    return (GtVec2){
        .elements = {
            v1.x / v2.x,
            v1.y / v2.y
        }
    };
}
GtVec2 gontiVec2Normalized(GtVec2 v) {
    gontiVec2Normalize(&v);
    return v;
}

/*
* F32 / FLOAT
*/
GtF32 gontiVec2LengthSquared(GtVec2 v) {
    return v.x * v.x + v.y * v.y;
}
GtF32 gontiVec2Length(GtVec2 v) {
    return gontiMathSqrt(gontiVec2LengthSquared(v));
}
GtF32 gontiVec2Distance(GtVec2 v1, GtVec2 v2) {
    GtVec2 d = (GtVec2) {
        .elements = {
            v1.x - v2.x,
            v1.y - v2.y
        }
    };

    return gontiVec2Length(d);
}

/*
* VOID
*/
void gontiVec2Normalize(GtVec2* v) {
    const GtF32 length = gontiVec2Length(*v);
    v->x /= length;
    v->y /= length;
}

/*
* B8 / BOOL
*/
GtB8 gontiVec2Compare(GtVec2 v1, GtVec2 v2, GtF32 tolerance) {
    if (gontiMathAbs(v1.x - v2.x) > tolerance)
        return GtFalse;

    if (gontiMathAbs(v1.y - v2.y) > tolerance)
        return GtFalse;
    
    return GtTrue;
}