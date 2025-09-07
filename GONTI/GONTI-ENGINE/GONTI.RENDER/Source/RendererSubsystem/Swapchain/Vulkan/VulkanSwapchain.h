#ifndef VULKANSWAPCHAIN_H
#define VULKANSWAPCHAIN_H

    #ifdef __cplusplus
        extern "C" {
    #endif

            #include "../../../Config/SetupGraphicBackend/UserSetGraphicBackend.h"
            
            #ifdef GONTI_USE_VULKAN

                #include "../../../Types/Vulkan/VulkanContextTypes.inl"

                //#include <stdint.h>
                #ifdef UINT32_MAX
                    #undef UINT32_MAX
                #endif
                #ifndef UINT32_MAX
                    #define UINT32_MAX 4294967295U
                #endif

                KAPI b8 gontiVkSwapchainAcquireNextImageIndex(GontiVulkanContext* context, GontiVulkanSwapchain* swapchain, u64 timeoutNs, VkSemaphore imageAnavibleSemaphore, VkFence fence, u32* outImageIndex);

                KAPI void gontiVkSwapchainCreate(GontiVulkanContext* context, u32 width, u32 height, GontiVulkanSwapchain* outSwapchain);
                KAPI void gontiVkSwapchainRecreate(GontiVulkanContext* context, u32 width, u32 height, GontiVulkanSwapchain* swapchain);
                KAPI void gontiVkSwapchainDestroy(GontiVulkanContext* context, GontiVulkanSwapchain* swapchain);
                KAPI VkResult gontiVkSwapchainPresent(GontiVulkanContext* context, GontiVulkanSwapchain* swapchain, VkQueue graphicsQueue, VkQueue presentQueue, VkSemaphore renderCompleteSemaphore, u32 presentImageIndex);

            #endif

    #ifdef __cplusplus
        }
    #endif

#endif