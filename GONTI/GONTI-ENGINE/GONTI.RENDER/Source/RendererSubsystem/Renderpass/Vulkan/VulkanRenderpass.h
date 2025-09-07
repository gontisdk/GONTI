#ifndef VULKANRENDERPASS_H
#define VULKANRENDERPASS_H

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../../../Config/SetupGraphicBackend/UserSetGraphicBackend.h"

        #ifdef GONTI_USE_VULKAN

            #include "../../../Types/Vulkan/VulkanContextTypes.inl"

            KAPI void gontiVkRenderpassCreate(
                GontiVulkanContext* context, GontiVulkanRenderpass* outRenderpass,
                f32 x, f32 y, f32 w, f32 h,
                f32 r, f32 g, f32 b, f32 a,
                f32 depth, u32 stencil
            );
            KAPI void gontiVkRenderpassBegin(
                GontiVulkanCommandBuffer* commandBuffer,
                GontiVulkanRenderpass* renderpass,
                VkFramebuffer frameBuffer
            );
            KAPI void gontiVkRenderpassEnd(GontiVulkanCommandBuffer* commandBuffer, GontiVulkanRenderpass* renderpass);
            KAPI void gontiVkRenderpassDestroy(GontiVulkanContext* context, GontiVulkanRenderpass* renderpass);

        #endif

#ifdef __cplusplus
    }
#endif

#endif