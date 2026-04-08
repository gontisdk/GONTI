#ifndef GtVector2H
#define GtVector2H

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../../../GtMaths.h"
        #include "../GtVecTypes.inl"

        /* VEC2 */
        GTAPI GtVec2 gontiVec2Create(GtF32 x, GtF32 y);
        GTAPI GtVec2 gontiVec2Zero();
        GTAPI GtVec2 gontiVec2One();
        GTAPI GtVec2 gontiVec2Up();
        GTAPI GtVec2 gontiVec2Down();
        GTAPI GtVec2 gontiVec2Left();
        GTAPI GtVec2 gontiVec2Right();
        GTAPI GtVec2 gontiVec2Add(GtVec2 v1, GtVec2 v2);
        GTAPI GtVec2 gontiVec2Subtract(GtVec2 v1, GtVec2 v2);
        GTAPI GtVec2 gontiVec2Multiply(GtVec2 v1, GtVec2 v2);
		GTAPI GtVec2 gontiVec2Divide(GtVec2 v1, GtVec2 v2);
        GTAPI GtVec2 gontiVec2Normalized(GtVec2 v);

        /* FLOAT / F32 */
        GTAPI GtF32 gontiVec2LengthSquared(GtVec2 v);
        GTAPI GtF32 gontiVec2Length(GtVec2 v);
        GTAPI GtF32 gontiVec2Distance(GtVec2 v1, GtVec2 v2);

        /* VOID */
        GTAPI void gontiVec2Normalize(GtVec2* v);

        /* BOOL / B8 */
        GTAPI GtB8 gontiVec2Compare(GtVec2 v1, GtVec2 v2, GtF32 tolerance);

#ifdef __cplusplus
    }
#endif

#endif