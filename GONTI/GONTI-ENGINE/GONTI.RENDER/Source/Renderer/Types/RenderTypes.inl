#ifndef RENDERERTYPES_INL
#define RENDERERTYPES_INL

#ifdef __cplusplus 
    extern "C" {
#endif

        #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/Defines.h>

        typedef enum GontiRendererBackendType {
            RENDERER_BACKEND_TYPE_VULKAN,

            RENDERER_BACKEND_TYPE_OPENGL,

            RENDERER_BACKEND_TYPE_DIRECTX_11,
            RENDERER_BACKEND_TYPE_DIRECTX_12
        }GontiRendererBackendType;

        typedef struct GontiRendererBackend {
            struct GontiVulkanPlatformState* GontiVulkanPlatformState;
            u64 frameNumber;
            b8 (*initialize)(struct GontiRendererBackend* backend, const char* appName, struct GontiVulkanPlatformState* platState);
            void (*shutdown)(struct GontiRendererBackend* backend);
            void (*resized)(struct GontiRendererBackend* backend, u16 width, u16 height);
            b8 (*beginFrame)(struct GontiRendererBackend* backend, f32 deltaTime);
            b8 (*endFrame)(struct GontiRendererBackend* backend, f32 deltaTime);
        }GontiRendererBackend;

        typedef struct GontiRendererPacket {
            f32 deltaTime;
        }GontiRendererPacket;

#ifdef __cplusplus 
    }
#endif

#endif