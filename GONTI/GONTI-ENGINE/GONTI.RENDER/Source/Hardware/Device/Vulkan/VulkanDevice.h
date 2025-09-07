#ifndef VULKANDEVICE_H
#define VULKANDEVICE_H

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../../../Config/SetupGraphicBackend/UserSetGraphicBackend.h"

        #if GONTI_USE_VULKAN

            #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/Defines.h>
            #include "../../Types/Vulkan/VulkanDeviceTypes.inl"
            #include "../../../Types/Vulkan/VulkanContextTypes.inl"

            /*B8*/
            KAPI b8 gontiVkDeviceCreate(struct GontiVulkanContext* context);
            KAPI b8 gontiVkDeviceDetectDepthFormat(struct GontiVulkanContext* context);
                
            /*VOID*/
            KAPI void gontiVkDeviceRelease(struct GontiVulkanContext* context);
            KAPI void gontiVkDeviceQuerySwapchainSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, GontiVulkanSwapchainSupportInfo* outSupportInfo);

        #endif

#ifdef __cplusplus
    }
#endif

#endif