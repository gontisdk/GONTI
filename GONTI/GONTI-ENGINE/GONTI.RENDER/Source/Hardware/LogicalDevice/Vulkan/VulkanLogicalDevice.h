#ifndef VULKANLOGICALDEVICE_H
#define VULKANLOGICALDEVICE_H

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../../../Config/SetupGraphicBackend/UserSetGraphicBackend.h"

        #if GONTI_USE_VULKAN

            #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/Defines.h>
            #include "../../Types/Vulkan/VulkanLogicalDeviceTypes.inl"
            #include "../../../Types/Vulkan/VulkanContextTypes.inl"

            KAPI b8 gontiVkLogicalDeviceCreate(
                VkPhysicalDevice physicalDevice, 
                VkDeviceCreateInfo* createInfo, 
                VkAllocationCallbacks* allocator, 
                VkDevice* outLogicalDevice
            );
                
            KAPI void gontiVkLogicalDeviceInitialize(GontiVulkanContext* context);
            KAPI void gontiVkLogicalDeviceSharedSetup(
                GontiVulkanDevice* gVkDevice,
                GontiVulkanLogicalDeviceQueueSharedSetup* gVkLdQueueSharedSetup
            );
            KAPI void gontiVkLogicalDeviceSharedFree(GontiVulkanLogicalDeviceQueueSharedSetup* gVkLdQueueSharedSetup);
            KAPI void gontiVkLogicalDeviceCommandPoolCreate(
                GontiVulkanContext* context,
                GontiVulkanDevice* gVkDevice,
                VkCommandPool* outCommandPool
            );
            KAPI void gontiVkLogicalDeviceQueueCreateInfos(
                GontiVulkanDevice* gVkDevice,
                GontiVulkanLogicalDeviceQueueSharedSetup* gVkLdQueueSharedSetup,
                GontiVulkanLogicalDeviceQueueSharedInfo* gVkLdQueueSharedInfo
            );
            KAPI void gontiVkLogicalDeviceQueueFreeInfos(
                GontiVulkanLogicalDeviceQueueSharedSetup* gVkLdQueueSharedSetup,
                GontiVulkanLogicalDeviceQueueSharedInfo* gVkLdQueueSharedInfo
            );
            KAPI void gontiVkLogicalDeviceConfigDriven(
                GontiVulkanDevice* gVkDevice
            );
            KAPI void gontiVkLogicalDeviceDeviceCreateInfo(
                GontiVulkanLogicalDeviceQueueSharedSetup* gVkLdQueueSharedSetup,
                GontiVulkanLogicalDeviceQueueSharedInfo* gVkLdQueueSharedInfo
            );
            KAPI void gontiVkLogicalDeviceQueuesGet(
                GontiVulkanDevice* gVkDevice
            );
            KAPI void gontiVkLogicalDeviceQueuesFree(
                GontiVulkanDevice* gVkDevice
            );
            KAPI void gontiVkLogicalDeviceDestroy(VkDevice logicalDevice, VkAllocationCallbacks* allocator);
            KAPI void gontiVkLogicalDeviceRelease(GontiVulkanContext* context);

        #endif

#ifdef __cplusplus
    }
#endif

#endif