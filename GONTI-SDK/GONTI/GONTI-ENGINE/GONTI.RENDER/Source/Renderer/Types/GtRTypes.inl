#ifndef GtRTypesINL
#define GtRTypesINL

#ifdef __cplusplus 
    extern "C" {
#endif

        #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/GtDefines.h>

        typedef enum GtRendererBackendType {
            RENDERER_BACKEND_TYPE_VULKAN,

            RENDERER_BACKEND_TYPE_OPENGL,

            RENDERER_BACKEND_TYPE_DIRECTX_11,
            RENDERER_BACKEND_TYPE_DIRECTX_12
        }GtRendererBackendType;

        typedef struct GtRendererBackend {
            struct GtVkPlatformState* GtVkPlatformState;
            GtU64 frameNumber;
            GtB8 (*initialize)(const char* appName, struct GtVkPlatformState* platState);
            void (*shutdown)();
            void (*resized)(GtU16 width, GtU16 height);
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