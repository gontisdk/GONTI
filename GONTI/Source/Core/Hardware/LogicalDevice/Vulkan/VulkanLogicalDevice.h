#ifndef VULKANLOGICALDEVICE_H
#define VULKANLOGICALDEVICE_H

    #ifdef __cplusplus
        extern "C" {
    #endif

            #include "../../../../__GONTI_INCLUDES.h"
            #ifdef USE_VULKAN
            #ifdef RendererSubsystem_ON

                #include "../../../RendererSubsystem/Types/Vulkan/VulkanContextTypes.inl"

                KAPI b8 vulkanLogicalDeviceCreate(struct gontiVulkanContext* context);
                
                KAPI void vulkanLogicalDeviceInitialize(struct gontiVulkanContext* context);
                KAPI void vulkanLogicalDeviceSharedSetup(struct gontiVulkanContext* context);
                KAPI void vulkanLogicalDeviceSharedFree(struct gontiVulkanContext* context);
                KAPI void vulkanLogicalDeviceCommandPoolCreateInfo(struct gontiVulkanContext* context);
                KAPI void vulkanLogicalDeviceQueueCreateInfos(struct gontiVulkanContext* context);
                KAPI void vulkanLogicalDeviceQueueFreeInfos(struct gontiVulkanContext* context);
                KAPI void vulkanLogicalDeviceConfigDriven(struct gontiVulkanContext* context);
                KAPI void vulkanLogicalDeviceDeviceCreateInfo(struct gontiVulkanContext* context);
                KAPI void vulkanLogicalDeviceQueuesGet(struct gontiVulkanContext* context);
                KAPI void vulkanLogicalDeviceQueuesFree(struct gontiVulkanContext* context);
                KAPI void vulkanLogicalDeviceDestroy(struct gontiVulkanContext* context);
                KAPI void vulkanLogicalDeviceRelease(struct gontiVulkanContext* context);

            #endif
            #endif

    #ifdef __cplusplus
        }
    #endif

#endif