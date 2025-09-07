#ifndef VULKANSWAPCHAINTYPES_INL
#define VULKANSWAPCHAINTYPES_INL

#ifdef __cplusplus 
    extern "C" {
#endif

        #include "../../../../__GONTI_INCLUDES.h"

        #ifdef USE_VULKAN
        #ifdef Swapchain_ON

            #include <vulkan/vulkan.h>
            #include "VulkanImageTypes.inl"
            #include "VulkanFramebufferTypes.inl"

            typedef struct gontiVulkanSwapchainSupportInfo {
                u32 formatCount;
                u32 presentModeCount;
                VkSurfaceCapabilitiesKHR capabilities;
                VkPresentModeKHR* presentMode;
                VkSurfaceFormatKHR* formats;
            }gontiVulkanSwapchainSupportInfo;

            typedef struct gontiVulkanSwapchain {
                VkImage* images;
                VkImageView* views;
                VkSwapchainKHR handle;
                VkSurfaceFormatKHR imageFormat;

                u32 imageCount;
                u8 maxFramesInFlight;
                b8 recreatingSwapchain;

                gontiVulkanImage depthAttachment;
                gontiVulkanSwapchainSupportInfo swapchainSupportInfo;
                gontiVulkanFramebuffer* framebuffers;
            }gontiVulkanSwapchain;

        #endif
        #endif

#ifdef __cplusplus 
    }
#endif

#endif