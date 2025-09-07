#ifndef VULKANPHYSICALDEVICETYPES_INL
#define VULKANPHYSICALDEVICETYPES_INL

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../../../Config/SetupGraphicBackend/UserSetGraphicBackend.h"

        #if GONTI_USE_VULKAN

            #include <vulkan/vulkan.h>
            #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/Defines.h>
        
            typedef struct GontiVulkanPhysicalDeviceList {
                VkPhysicalDevice* devices;
                u32 count;
            }GontiVulkanPhysicalDeviceList;

            typedef struct GontiVulkanPhysicalDeviceRequirements {
                b8 graphics;
                b8 present;
                b8 compute;
                b8 transfer;

                b8 samplerAnisotropy;
                b8 discreteGpu;

                const char** deviceExtensionNames;
            }GontiVulkanPhysicalDeviceRequirements;

            typedef struct GontiVulkanPhysicalDeviceQueueFamilyInfo {
                i32 graphicsFamilyIndex;
                i32 presentFamilyIndex;
                i32 computeFamilyIndex;
                i32 transferFamilyIndex;
            }GontiVulkanPhysicalDeviceQueueFamilyInfo;

        #endif

#ifdef __cplusplus
    }
#endif

#endif