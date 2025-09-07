#ifndef VULKANPHYSICALDEVICE_H
#define VULKANPHYSICALDEVICE_H

    #ifdef __cplusplus
        extern "C" {
    #endif

            #include "../../../../__GONTI_INCLUDES.h"
            #ifdef USE_VULKAN
            #ifdef RendererSubsystem_ON

                #include "../../../RendererSubsystem/Types/Vulkan/VulkanContextTypes.inl"

                typedef struct VulkanPhysicalDeviceRequirements {
                    b8 graphics;
                    b8 present;
                    b8 compute;
                    b8 transfer;

                    b8 samplerAnisotropy;
                    b8 discreteGpu;

                    const char** deviceExtensionNames;
                }VulkanPhysicalDeviceRequirements;

                typedef struct VulkanPhysicalDeviceQueueFamilyInfo {
                    i32 graphicsFamilyIndex;
                    i32 presentFamilyIndex;
                    i32 computeFamilyIndex;
                    i32 transferFamilyIndex;
                }VulkanPhysicalDeviceQueueFamilyInfo;

                KAPI b8 vulkanSelectPhysicalDevice(struct gontiVulkanContext* context);
                KAPI b8 vulkanFindPhysicalDevices(struct gontiVulkanContext* context);
                KAPI b8 vulkanPhysicalDeviceMeetsRequirements(
                    VkPhysicalDevice device,
                    VkSurfaceKHR surface,
                    const VkPhysicalDeviceProperties* properties,
                    const VkPhysicalDeviceFeatures* features,
                    const VulkanPhysicalDeviceRequirements* requirements,
                    VulkanPhysicalDeviceQueueFamilyInfo* outQueueInfo,
                    gontiVulkanSwapchainSupportInfo* outSwapchainSupport
                );
                KAPI b8 vulkanNumeratePhysicalDevices(struct gontiVulkanContext* context);

                KAPI void vulkanPhysicalDeviceRelease(struct gontiVulkanContext* context);

            #endif
            #endif

    #ifdef __cplusplus
        }
    #endif

#endif