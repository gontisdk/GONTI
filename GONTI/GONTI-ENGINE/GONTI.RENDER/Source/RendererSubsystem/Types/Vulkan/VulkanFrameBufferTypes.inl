#ifndef VULKANFRAMEBUFFERTYPES_INL
#define VULKANFRAMEBUFFERTYPES_INL

#ifdef __cplusplus 
    extern "C" {
#endif

        #include "../../../Config/SetupGraphicBackend/UserSetGraphicBackend.h"

        #ifdef GONTI_USE_VULKAN

            #include <vulkan/vulkan.h>
            #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/Defines.h>
            #include "VulkanRenderpassTypes.inl"

            typedef struct GontiVulkanFramebuffer {
                VkFramebuffer handle;
                u32 attachmentCount;
                VkImageView* attachments;
                GontiVulkanRenderpass* renderpass;
            }GontiVulkanFramebuffer;

        #endif

#ifdef __cplusplus 
    }
#endif

#endif