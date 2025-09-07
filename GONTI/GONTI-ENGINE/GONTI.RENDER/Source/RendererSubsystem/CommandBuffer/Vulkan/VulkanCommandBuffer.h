#ifndef VULKANCOMMANDBUFFER_H
#define VULKANCOMMANDBUFFER_H

    #ifdef __cplusplus
        extern "C" {
    #endif

            #include "../../../Config/SetupGraphicBackend/UserSetGraphicBackend.h"
            
            #ifdef GONTI_USE_VULKAN

                #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/Defines.h>
                #include "../../../Types/Vulkan/VulkanContextTypes.inl"

                KAPI void gontiVkCommandBufferAllocate(
                    GontiVulkanContext* context,
                    VkCommandPool pool,
                    b8 isPrimary,
                    GontiVulkanCommandBuffer* outCommandBuffer
                );
                KAPI void gontiVkCommandBufferFree(
                    GontiVulkanContext* context,
                    VkCommandPool pool,
                    GontiVulkanCommandBuffer* commandBuffer
                );
                KAPI void gontiVkCommandBufferBegin(
                    GontiVulkanCommandBuffer* commandBuffer,
                    b8 isSingleUse,
                    b8 isRenderpassContinue,
                    b8 isSimultaneous
                );
                KAPI void gontiVkCommandBufferAllocateAndBeginSingleUse(
                    GontiVulkanContext* context,
                    VkCommandPool pool,
                    GontiVulkanCommandBuffer* outCommandBuffer
                );
                KAPI void gontiVkCommandBufferEndSingleUse(
                    GontiVulkanContext* context,
                    VkCommandPool pool,
                    GontiVulkanCommandBuffer* commandBuffer,
                    VkQueue queue
                );
                KAPI void gontiVkCommandBufferEnd(GontiVulkanCommandBuffer* commandBuffer);
                KAPI void gontiVkCommandBufferSubmitted(GontiVulkanCommandBuffer* commandBuffer);
                KAPI void gontiVkCommandBufferReset(GontiVulkanCommandBuffer* commandBuffer);
                KAPI void gontiVkCommandBufferUpdateSubmitted(GontiVulkanCommandBuffer* commandBuffer);

            #endif

    #ifdef __cplusplus
        }
    #endif

#endif