#ifndef GtVertexTypesINL
#define GtVertexTypesINL

#ifdef __cplusplus
    extern "C" {
#endif

        #include <Defines/GtDefines.inl>
        #include <Math/Algebra/Vectors/GtVecTypes.inl>

        typedef struct GtVertex2D {
            GtVec2 position;
            GtVec2 texcoord;
        } GtVertex2D;

        typedef struct GtVertex3D {
            GtVec3 position;
            GtVec3 normal;
            GtVec2 texcoord;
            GtVec4 colour;
            GtVec3 tangent;
        } GtVertex3D;

#ifdef __cplusplus
    }
#endif

#endif