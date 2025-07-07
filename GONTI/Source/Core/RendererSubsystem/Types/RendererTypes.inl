#ifndef RENDERERTYPES_INL
#define RENDERERTYPES_INL

#ifdef __cplusplus 
    extern "C" {
#endif

        #include "../../../__GONTI_INCLUDES.h"

        #ifdef RendererSubsystem_ON

            typedef enum gontiRendererBackendType {
                RENDERER_BACKEND_TYPE_VULKAN,

                RENDERER_BACKEND_TYPE_OPENGL_GLAD,
                RENDERER_BACKEND_TYPE_OPENGL_GLEW,

                RENDERER_BACKEND_TYPE_DIRECTX_11,
                RENDERER_BACKEND_TYPE_DIRECTX_12
            }gontiRendererBackendType;

            typedef struct gontiRendererBackend {
                struct gontiPlatformState* platformState;
                u64 frameNumber;
                b8 (*initialize)(struct gontiRendererBackend* backend, const char* appName, struct gontiPlatformState* platState);
                void (*shutdown)(struct gontiRendererBackend* backend);
                void (*resized)(struct gontiRendererBackend* backend, u16 width, u16 height);
                b8 (*beginFrame)(struct gontiRendererBackend* backend, f32 deltaTime);
                b8 (*endFrame)(struct gontiRendererBackend* backend, f32 deltaTime);
            }gontiRendererBackend;

            typedef struct gontiRendererPacket {
                f32 deltaTime;
            }gontiRendererPacket;

        #endif

#ifdef __cplusplus 
    }
#endif

#endif