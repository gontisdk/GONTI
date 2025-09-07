#ifndef VULKANRENDERERBACKEND_H
#define VULKANRENDERERBACKEND_H

#ifdef __cplusplus 
    extern "C" {
#endif

        #include "../../../../../__GONTI_INCLUDES.h"

        #ifdef USE_VULKAN
        #ifdef RendererBackend_ON

            #include "../RendererBackend.h"

            //#include <stdint.h>
            #ifdef UINT64_MAX
                #undef UINT64_MAX
            #endif
            #ifndef UINT64_MAX
                #define UINT64_MAX 18446744073709551615ULL 
            #endif
            
            KAPI b8 vulkanRendererBackendInitialize(gontiRendererBackend* backend, const char* appName, struct gontiPlatformState* platState);
            KAPI b8 vulkanRendererBackendBeginFrame(gontiRendererBackend* backend, f32 deltaTime);
            KAPI b8 vulkanRendererBackendEndFrame(gontiRendererBackend* backend, f32 deltaTime);

            KAPI void vulkanRendererBackendShutdown(gontiRendererBackend* backend);
            KAPI void vulkanRendererBackendOnResized(gontiRendererBackend* backend, u16 width, u16 height);

        #endif
        #endif

#ifdef __cplusplus 
    }
#endif

#endif