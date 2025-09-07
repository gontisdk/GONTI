#ifndef VULKANSWAPCHAIN_H
#define VULKANSWAPCHAIN_H

    #ifdef __cplusplus
        extern "C" {
    #endif

            #include "../../../../__GONTI_INCLUDES.h"
            
            #ifdef USE_VULKAN
            #ifdef Swapchain_ON

                #include "../../Types/Vulkan/VulkanContextTypes.inl"

                //#include <stdint.h>
                #ifdef UINT32_MAX
                    #undef UINT32_MAX
                #endif
                #ifndef UINT32_MAX
                    #define UINT32_MAX 4294967295U
                #endif

                KAPI b8 vulkanSwapchainAcquireNextImageIndex(gontiVulkanContext* context, gontiVulkanSwapchain* swapchain, u64 timeoutNs, VkSemaphore imageAnavibleSemaphore, VkFence fence, u32* outImageIndex);

                KAPI void vulkanSwapchainCreate(gontiVulkanContext* context, u32 width, u32 height, gontiVulkanSwapchain* outSwapchain);
                KAPI void vulkanSwapchainRecreate(gontiVulkanContext* context, u32 width, u32 height, gontiVulkanSwapchain* swapchain);
                KAPI void vulkanSwapchainDestroy(gontiVulkanContext* context, gontiVulkanSwapchain* swapchain);
                KAPI void vulkanSwapchainPresent(gontiVulkanContext* context, gontiVulkanSwapchain* swapchain, VkQueue graphicsQueue, VkQueue presentQueue, VkSemaphore renderCompleteSemaphore, u32 presentImageIndex);

            #endif
            #endif

    #ifdef __cplusplus
        }
    #endif

#endif