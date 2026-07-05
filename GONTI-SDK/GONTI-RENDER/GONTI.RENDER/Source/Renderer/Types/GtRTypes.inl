#ifndef GtRTypesINL
#define GtRTypesINL

#ifdef __cplusplus 
extern "C" {
#endif

    #include <GONTI-CORE/GONTI.CORE/Source/Defines/GtDefines.inl>
    #include <GONTI-CORE/GONTI.MATH/Source/Algebra/Matrices/GtMatTypes.inl>
    #include <GONTI-CORE/GONTI.MATH/Source/Algebra/Vectors/GtVecTypes.inl>

    typedef enum GtRendererBackendType {
        RENDERER_BACKEND_TYPE_VULKAN,
        RENDERER_BACKEND_TYPE_OPENGL,
        RENDERER_BACKEND_TYPE_DIRECTX_11,
        RENDERER_BACKEND_TYPE_DIRECTX_12
    }GtRendererBackendType;

    struct GtVkPlatformState;
    typedef struct GtRendererBackend {
        GtU64 frameNumber;
        GtB8 (*initialize)(const char* appName, struct GtVkPlatformState* platState);
        void (*shutdown)();
        void (*resized)(GtU16 width, GtU16 height);
        void (*updateGlobalStage)(GtMat4 projection, GtMat4 view, GtVec3 viewPosition, GtVec4 ambientColour, GtI32 mode);
        GtB8 (*beginFrame)(GtF32 deltaTime);
        GtB8 (*endFrame)(GtF32 deltaTime);
    }GtRendererBackend;

    typedef struct GtRendererPacket {
        GtF32 deltaTime;
        GtF32 interpolation;
    }GtRendererPacket;

#ifdef __cplusplus 
}
#endif

#endif