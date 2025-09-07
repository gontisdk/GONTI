#ifndef VULKANRENDERERTYPES_INL
#define VULKANRENDERERTYPES_INL

#ifdef __cplusplus 
    extern "C" {
#endif

        #include "../../../Config/SetupGraphicBackend/UserSetGraphicBackend.h"

        #ifdef GONTI_USE_VULKAN

            #include <vulkan/vulkan.h>
            #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/Defines.h>

            typedef enum GontiVulkanRenderpassState {
                READY,
                RECORDING,
                IN_RENDER_PASS,
                RECORDING_ENDED,
                SUBMITTED,
                NOT_ALLOCATED
            }GontiVulkanRenderpassState;

            typedef struct GontiVulkanRenderpass {
                VkRenderPass handle;

                f32 x, y, w, h;
                f32 r, g, b, a;
                f32 depth;

                u32 stencil;

                GontiVulkanRenderpassState state;
            }GontiVulkanRenderpass;

        #endif

#ifdef __cplusplus 
    }
#endif

#endif