#ifndef VULKANFRAMEBUFFER_H
#define VULKANFRAMEBUFFER_H

    #ifdef __cplusplus
        extern "C" {
    #endif

            #include "../../../Config/SetupGraphicBackend/UserSetGraphicBackend.h"
            
            #ifdef GONTI_USE_VULKAN

                #include "../../../Types/Vulkan/VulkanContextTypes.inl"

                KAPI void gontiVkFramebufferCreate(
                    GontiVulkanContext* context,
                    GontiVulkanRenderpass* renderpass,
                    u32 width, u32 height,
                    u32 attachmentCount,
                    VkImageView* attachments,
                    GontiVulkanFramebuffer* outFramebuffer
                );
                KAPI void gontiVkFramebufferDestroy(GontiVulkanContext* context, GontiVulkanFramebuffer* framebuffer);

            #endif

    #ifdef __cplusplus
        }
    #endif

#endif