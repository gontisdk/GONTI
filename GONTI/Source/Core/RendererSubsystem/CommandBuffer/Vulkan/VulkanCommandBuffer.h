#ifndef VULKANCOMMANDBUFFER_H
#define VULKANCOMMANDBUFFER_H

    #ifdef __cplusplus
        extern "C" {
    #endif

            #include "../../../../__GONTI_INCLUDES.h"
            
            #ifdef USE_VULKAN
            #ifdef CommandBuffer_ON

                #include "../../Types/Vulkan/VulkanContextTypes.inl"

                KAPI void vulkanCommandBufferAllocate(
                    gontiVulkanContext* context,
                    VkCommandPool pool,
                    b8 isPrimary,
                    gontiVulkanCommandBuffer* outCommandBuffer
                );
                KAPI void vulkanCommandBufferFree(
                    gontiVulkanContext* context,
                    VkCommandPool pool,
                    gontiVulkanCommandBuffer* commandBuffer
                );
                KAPI void vulkanCommandBufferBegin(
                    gontiVulkanCommandBuffer* commandBuffer,
                    b8 isSingleUse,
                    b8 isRenderpassContinue,
                    b8 isSimultaneous
                );
                KAPI void vulkanCommandBufferAllocateAndBeginSingleUse(
                    gontiVulkanContext* context,
                    VkCommandPool pool,
                    gontiVulkanCommandBuffer* outCommandBuffer
                );
                KAPI void vulkanCommandBufferEndSingleUse(
                    gontiVulkanContext* context,
                    VkCommandPool pool,
                    gontiVulkanCommandBuffer* commandBuffer,
                    VkQueue queue
                );
                KAPI void vulkanCommandBufferEnd(gontiVulkanCommandBuffer* commandBuffer);
                KAPI void vulkanCommandBufferSubmitted(gontiVulkanCommandBuffer* commandBuffer);
                KAPI void vulkanCommandBufferReset(gontiVulkanCommandBuffer* commandBuffer);
                KAPI void vulkanCommandBufferUpdateSubmitted(gontiVulkanCommandBuffer* commandBuffer);

            #endif
            #endif

    #ifdef __cplusplus
        }
    #endif

#endif