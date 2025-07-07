#ifndef VULKANFRAMEBUFFER_H
#define VULKANFRAMEBUFFER_H

    #ifdef __cplusplus
        extern "C" {
    #endif

            #include "../../../../__GONTI_INCLUDES.h"
            
            #ifdef USE_VULKAN
            #ifdef FrameBuffer_ON

                #include "../../Types/Vulkan/VulkanContextTypes.inl"

                KAPI void vulkanFramebufferCreate(
                    gontiVulkanContext* context,
                    gontiVulkanRenderpass* renderpass,
                    u32 width, u32 height,
                    u32 attachmentCount,
                    VkImageView* attachments,
                    gontiVulkanFramebuffer* outFramebuffer
                );
                KAPI void vulkanFramebufferDestroy(gontiVulkanContext* context, gontiVulkanFramebuffer* framebuffer);

            #endif
            #endif

    #ifdef __cplusplus
        }
    #endif

#endif