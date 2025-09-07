#ifndef VULKANFRAMEBUFFERTYPES_INL
#define VULKANFRAMEBUFFERTYPES_INL

#ifdef __cplusplus 
    extern "C" {
#endif

        #include "../../../../__GONTI_INCLUDES.h"

        #ifdef USE_VULKAN
        #ifdef FrameBuffer_ON

            #include <vulkan/vulkan.h>
            #include "VulkanRenderpassTypes.inl"

            typedef struct gontiVulkanFramebuffer {
                VkFramebuffer handle;
                u32 attachmentCount;
                VkImageView* attachments;
                gontiVulkanRenderpass* renderpass;
            }gontiVulkanFramebuffer;

        #endif
        #endif

#ifdef __cplusplus 
    }
#endif

#endif