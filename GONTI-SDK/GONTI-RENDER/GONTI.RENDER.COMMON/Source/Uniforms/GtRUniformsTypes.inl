#ifndef GtRUniformsTypesINL
#define GtRUniformsTypesINL

#ifdef __cplusplus
extern "C" {
#endif

    #include <GONTI-CORE/GONTI.MATH/Source/Algebra/Matrices/GtMatTypes.inl>

    typedef struct GtGlobalUniformObject {
        GtMat4 projection;
        GtMat4 view;
        GtMat4 _gtMemReserved0;
        GtMat4 _gtMemReserved1;
    }GtGlobalUniformObject;

#ifdef __cplusplus
}
#endif

#endif