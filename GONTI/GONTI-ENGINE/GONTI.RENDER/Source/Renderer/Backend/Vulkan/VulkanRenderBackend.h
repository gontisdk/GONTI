#ifndef VULKANRENDERERBACKEND_H
#define VULKANRENDERERBACKEND_H

#ifdef __cplusplus 
    extern "C" {
#endif

        #include "../../../Config/SetupGraphicBackend/UserSetGraphicBackend.h"

        #ifdef GONTI_USE_VULKAN

            #include "../RenderBackend.h"

            //#include <stdint.h>
            #ifdef UINT64_MAX
                #undef UINT64_MAX
            #endif
            #ifndef UINT64_MAX
                #define UINT64_MAX 18446744073709551615ULL 
            #endif
    
            KAPI b8 gontiVkRendererBackendInitialize(GontiRendererBackend* backend, const char* appName, struct GontiVulkanPlatformState* platState);
            KAPI b8 gontiVkRendererBackendBeginFrame(GontiRendererBackend* backend, f32 deltaTime);
            KAPI b8 gontiVkRendererBackendEndFrame(GontiRendererBackend* backend, f32 deltaTime);

            KAPI void gontiVkRendererBackendShutdown(GontiRendererBackend* backend);
            KAPI void gontiVkRendererBackendOnResized(GontiRendererBackend* backend, u16 width, u16 height);

        #endif

#ifdef __cplusplus 
    }
#endif

#endif