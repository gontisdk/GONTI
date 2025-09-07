#ifndef VULKANDEVICETYPES_INL
#define VULKANDEVICETYPES_INL

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../../../Config/SetupGraphicBackend/UserSetGraphicBackend.h"

        #if GONTI_USE_VULKAN
        
            #include "VulkanLogicalDeviceTypes.inl"
            #include "VulkanPhysicalDeviceTypes.inl"
            #include "../../../RendererSubsystem/Types/Vulkan/VulkanSwapchainTypes.inl"

            typedef struct GontiVulkanDeviceInfo{
                VkPhysicalDeviceProperties properties;
                VkPhysicalDeviceFeatures features;
                VkPhysicalDeviceMemoryProperties memory;
            }GontiVulkanDeviceInfo;

            typedef struct GontiVulkanDevice {
                VkPhysicalDevice physicalDevice;
                VkDevice logicalDevice;

                GontiVulkanSwapchainSupportInfo swapchainSupport;
                GontiVulkanLogicalDevice vkLdDevice;
                GontiVulkanPhysicalDeviceList vkDevices;
                GontiVulkanDeviceInfo* vkDeviceInfo;

                i32 graphicsQueueIndex;
                i32 presentQueueIndex;
                i32 transferQueueIndex;
                i32 computeQueueIndex;

                VkQueue graphicsQueue;
                VkQueue presentQueue;
                VkQueue transferQueue;
                VkQueue computeQueue;

                VkCommandPool graphicsCommandPool;

                VkPhysicalDeviceProperties properties;
                VkPhysicalDeviceFeatures features;
                VkPhysicalDeviceMemoryProperties memory;
                VkFormat depthFormat;
            }GontiVulkanDevice;

        #endif

#ifdef __cplusplus
    }
#endif

#endif