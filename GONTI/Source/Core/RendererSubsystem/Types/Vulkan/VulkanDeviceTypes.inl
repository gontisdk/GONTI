#ifndef VULKANDEVICETYPES_INL
#define VULKANDEVICETYPES_INL

#ifdef __cplusplus 
    extern "C" {
#endif

        #include "../../../../__GONTI_INCLUDES.h"

        #ifdef USE_VULKAN
        #ifdef RendererSubsystem_ON

            #include <vulkan/vulkan.h>

            #include "VulkanSwapchainTypes.inl"

            typedef struct gontiVulkanPhysicalDeviceList {
                VkPhysicalDevice* devices;
                u32 count;
            }gontiVulkanPhysicalDeviceList;

            typedef struct gontiVulkanDeviceInfo{
                VkPhysicalDeviceProperties properties;
                VkPhysicalDeviceFeatures features;
                VkPhysicalDeviceMemoryProperties memory;
            }gontiVulkanDeviceInfo;

            typedef struct gontiVulkanLogicalDeviceQueueSharedInfo {
                VkDeviceQueueCreateInfo* queueCreateInfos;
                VkDeviceCreateInfo deviceCreateInfo;
                VkPhysicalDeviceFeatures deviceFeatures;
            }gontiVulkanLogicalDeviceQueueSharedInfo;

            typedef struct gontiVulkanLogicalDeviceQueueSharedSetup {
                b8 presentSharesGraphicsQueue;
                b8 transferSharesGraphicsQueue;

                u32 indexCount;
                u32* indices;
                u32 index;

                f32* queuePriority;

                const char* extensionNames;
            }gontiVulkanLogicalDeviceQueueSharedSetup;

            typedef struct VulkanLogicalDevice {
                gontiVulkanLogicalDeviceQueueSharedInfo vkLdQueueSharedInfo;
                gontiVulkanLogicalDeviceQueueSharedSetup vkLdQueueSharedSetup;
            }VulkanLogicalDevice;

            typedef struct gontiVulkanDevice {
                VkPhysicalDevice physicalDevice;
                VkDevice logicalDevice;

                gontiVulkanSwapchainSupportInfo swapchainSupport;
                VulkanLogicalDevice vkLdDevice;
                gontiVulkanPhysicalDeviceList vkDevices;
                gontiVulkanDeviceInfo* vkDeviceInfo;

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
            }gontiVulkanDevice;

        #endif
        #endif

#ifdef __cplusplus 
    }
#endif

#endif