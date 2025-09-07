#ifndef VULKANRENDERERTYPES_INL
#define VULKANRENDERERTYPES_INL

#ifdef __cplusplus 
    extern "C" {
#endif

        #include "../../../../__GONTI_INCLUDES.h"

        #ifdef USE_VULKAN
        #ifdef RendererSubsystem_ON

            #include <vulkan/vulkan.h>

            typedef enum gontiVulkanRenderpassState {
                READY,
                RECORDING,
                IN_RENDER_PASS,
                RECORDING_ENDED,
                SUBMITTED,
                NOT_ALLOCATED
            }gontiVulkanRenderpassState;

            typedef struct gontiVulkanRenderpass {
                VkRenderPass handle;

                f32 x, y, w, h;
                f32 r, g, b, a;
                f32 depth;

                u32 stencil;

                gontiVulkanRenderpassState state;
            }gontiVulkanRenderpass;

        #endif
        #endif

#ifdef __cplusplus 
    }
#endif

#endif