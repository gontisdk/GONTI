#ifndef VULKANDEVICE_H
#define VULKANDEVICE_H

    #ifdef __cplusplus
        extern "C" {
    #endif

            #include "../../../../__GONTI_INCLUDES.h"
            #ifdef USE_VULKAN
            #ifdef RendererSubsystem_ON

                #include "../../../RendererSubsystem/Types/Vulkan/VulkanContextTypes.inl"
                #include "../../PhysicalDevice/Vulkan/VulkanPhysicalDevice.h"
                #include "../../LogicalDevice/Vulkan/VulkanLogicalDevice.h"

                extern KAPI const gontiVulkanPhysicalDeviceList ZERO_DEVICES;

                /*B8*/
                KAPI b8 vulkanDeviceCreate(struct gontiVulkanContext* context);
                KAPI b8 vulkanDeviceDetectDepthFormat(struct gontiVulkanContext* context);
                
                /*VOID*/
                KAPI void vulkanDeviceRelease(struct gontiVulkanContext* context);
                KAPI void vulkanDeviceQuerySwapchainSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, gontiVulkanSwapchainSupportInfo* outSupportInfo);

            #endif
            #endif

    #ifdef __cplusplus
        }
    #endif

#endif