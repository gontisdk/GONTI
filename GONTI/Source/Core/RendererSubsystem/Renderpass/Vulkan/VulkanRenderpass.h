#ifndef VULKANRENDERPASS_H
#define VULKANRENDERPASS_H

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../../../../__GONTI_INCLUDES.h"

        #ifdef USE_VULKAN
        #ifdef Renderpass_ON

            #include "../../Types/Vulkan/VulkanContextTypes.inl"

            KAPI void vulkanRenderpassCreate(
                gontiVulkanContext* context, gontiVulkanRenderpass* outRenderpass,
                f32 x, f32 y, f32 w, f32 h,
                f32 r, f32 g, f32 b, f32 a,
                f32 depth, u32 stencil
            );
            KAPI void vulkanRenderpassBegin(
                gontiVulkanCommandBuffer* commandBuffer,
                gontiVulkanRenderpass* renderpass,
                VkFramebuffer frameBuffer
            );
            KAPI void vulkanRenderpassEnd(gontiVulkanCommandBuffer* commandBuffer, gontiVulkanRenderpass* renderpass);
            KAPI void vulkanRenderpassDestroy(gontiVulkanContext* context, gontiVulkanRenderpass* renderpass);

        #endif
        #endif

#ifdef __cplusplus
    }
#endif

#endif