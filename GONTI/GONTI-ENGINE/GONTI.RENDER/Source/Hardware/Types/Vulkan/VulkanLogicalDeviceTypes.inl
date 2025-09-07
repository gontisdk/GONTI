#ifndef VULKANLOGICALDEVICETYPES_INL
#define VULKANLOGICALDEVICETYPES_INL

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../../../Config/SetupGraphicBackend/UserSetGraphicBackend.h"

        #if GONTI_USE_VULKAN

            #include <vulkan/vulkan.h>
            #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/Defines.h>
        
            typedef struct GontiVulkanLogicalDeviceQueueSharedInfo {
                VkDeviceQueueCreateInfo* queueCreateInfos;
                VkDeviceCreateInfo deviceCreateInfo;
                VkPhysicalDeviceFeatures deviceFeatures;
            }GontiVulkanLogicalDeviceQueueSharedInfo;

            typedef struct GontiVulkanLogicalDeviceQueueSharedSetup {
                b8 presentSharesGraphicsQueue;
                b8 transferSharesGraphicsQueue;

                u32 indexCount;
                u32* indices;
                u32 index;

                f32* queuePriority;

                const char* extensionNames;
            }GontiVulkanLogicalDeviceQueueSharedSetup;

            typedef struct GontiVulkanLogicalDevice {
                GontiVulkanLogicalDeviceQueueSharedInfo vkLdQueueSharedInfo;
                GontiVulkanLogicalDeviceQueueSharedSetup vkLdQueueSharedSetup;
            }GontiVulkanLogicalDevice;

        #endif

#ifdef __cplusplus
    }
#endif

#endif