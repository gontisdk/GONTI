#ifndef VULKANSWAPCHAINTYPES_INL
#define VULKANSWAPCHAINTYPES_INL

#ifdef __cplusplus 
    extern "C" {
#endif

        #include "../../../Config/SetupGraphicBackend/UserSetGraphicBackend.h"

        #if GONTI_USE_VULKAN

            #include <vulkan/vulkan.h>
            #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/Defines.h>
            #include "VulkanImageTypes.inl"
            #include "VulkanFrameBufferTypes.inl"

            typedef struct GontiVulkanSwapchainSupportInfo {
                u32 formatCount;
                u32 presentModeCount;
                VkSurfaceCapabilitiesKHR capabilities;
                VkPresentModeKHR* presentMode;
                VkSurfaceFormatKHR* formats;
            }GontiVulkanSwapchainSupportInfo;

            typedef struct GontiVulkanSwapchain {
                VkImage* images;
                VkImageView* views;
                VkSwapchainKHR handle;
                VkSurfaceFormatKHR imageFormat;

                u32 imageCount;
                u8 maxFramesInFlight;
                b8 recreatingSwapchain;

                GontiVulkanImage depthAttachment;
                GontiVulkanSwapchainSupportInfo swapchainSupportInfo;
                GontiVulkanFramebuffer* framebuffers;
            }GontiVulkanSwapchain;

        #endif

#ifdef __cplusplus 
    }
#endif

#endif