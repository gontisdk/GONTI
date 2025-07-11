#ifndef VULKANLOGICALDEVICE_H
#define VULKANLOGICALDEVICE_H

    #ifdef __cplusplus
        extern "C" {
    #endif

            #include "../../../../__GONTI_INCLUDES.h"
            #ifdef USE_VULKAN
            #ifdef RendererSubsystem_ON

                #include "../../../RendererSubsystem/Types/Vulkan/VulkanContextTypes.inl"

                KAPI b8 vulkanLogicalDeviceCreate(
                    VkPhysicalDevice physicalDevice, 
                    VkDeviceCreateInfo* createInfo, 
                    VkAllocationCallbacks* allocator, 
                    VkDevice* outLogicalDevice
                );
                
                KAPI void vulkanLogicalDeviceInitialize(struct gontiVulkanContext* context);
                KAPI void vulkanLogicalDeviceSharedSetup(
                    b8 presentSharesGraphicsQueue,
                    b8 transferSharesGraphicsQueue,
                    u32** indices,
                    u32* indexCount,
                    u32* index,
                    i32 graphicsQueueIndex,
                    i32 presentQueueIndex,
                    i32 transferQueueIndex
                );
                KAPI void vulkanLogicalDeviceSharedFree(u32** indices, u32 indexCount);
                KAPI void vulkanLogicalDeviceCommandPoolCreateInfo(
                    VkDevice logicalDevice,
                    VkAllocationCallbacks* allocator,
                    i32 graphicsQueueIndex,
                    VkCommandPool* outCommandPool
                );
                KAPI void vulkanLogicalDeviceQueueCreateInfos(
                    VkDeviceQueueCreateInfo** queueCreateInfos,
                    f32** queuePriority,
                    u32** indices,
                    u32 indexCount,
                    i32 graphicsQueueIndex
                );
                KAPI void vulkanLogicalDeviceQueueFreeInfos(
                    VkDeviceQueueCreateInfo** queueCreateInfos,
                    f32** queuePriority,
                    u32 indexCount
                );
                KAPI void vulkanLogicalDeviceConfigDriven(
                    VkPhysicalDeviceFeatures* features
                    // TODO: add more 
                );
                KAPI void vulkanLogicalDeviceDeviceCreateInfo(
                    VkDeviceQueueCreateInfo** queueCreateInfos,
                    VkDeviceCreateInfo* deviceCreateInfo,
                    VkPhysicalDeviceFeatures* deviceFeatures,
                    u32 indexCount,
                    const char** extensionNames
                );
                KAPI void vulkanLogicalDeviceQueuesGet(
                    VkDevice logicalDevice,
                    VkQueue* graphicsQueue,
                    i32 graphicsQueueIndex,
                    VkQueue* presentQueue,
                    i32 presentQueueIndex,
                    VkQueue* transferQueue,
                    i32 transferQueueIndex,
                    VkQueue* computeQueue,
                    i32 computeQueueIndex
                );
                KAPI void vulkanLogicalDeviceQueuesFree(
                    VkQueue* graphicsQueue,
                    VkQueue* presentQueue,
                    VkQueue* transferQueue,
                    VkQueue* computeQueue
                );
                KAPI void vulkanLogicalDeviceDestroy(VkDevice logicalDevice, VkAllocationCallbacks* allocator);
                KAPI void vulkanLogicalDeviceRelease(struct gontiVulkanContext* context);

            #endif
            #endif

    #ifdef __cplusplus
        }
    #endif

#endif