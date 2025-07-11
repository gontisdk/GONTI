#include "VulkanDevice.h"

#ifdef USE_VULKAN
#ifdef RendererSubsystem_ON

#include "../../../../Utilities/Logging/Scripts/Logger.h"
#include "../../../../Libs/StringLib/StringLib.h"
#include "../../../../Core/MemorySubsystem/MemorySubsystem.h"
#include "../../../../Containers/DynamicArray/DynamicArray.h"

const gontiVulkanPhysicalDeviceList ZERO_DEVICES = { VK_NULL_HANDLE, 0 };

/*
* B8
*/
b8 vulkanDeviceCreate(gontiVulkanContext* context) {
    if (!vulkanSelectPhysicalDevice(context)) return false;

    vulkanLogicalDeviceInitialize(context);
    vulkanLogicalDeviceCreate(
        context->device.physicalDevice, 
        &context->device.vkLdDevice.vkLdQueueSharedInfo.deviceCreateInfo, 
        context->allocator, 
        &context->device.logicalDevice
    );
    vulkanLogicalDeviceQueuesGet(
        context->device.logicalDevice,
        &context->device.graphicsQueue,
        context->device.graphicsQueueIndex,
        &context->device.presentQueue,
        context->device.presentQueueIndex,
        &context->device.transferQueue,
        context->device.transferQueueIndex,
        &context->device.computeQueue,
        context->device.computeQueueIndex
    );
    vulkanLogicalDeviceCommandPoolCreateInfo(
        context->device.logicalDevice,
        context->allocator,
        context->device.graphicsQueueIndex,
        &context->device.graphicsCommandPool
    );

    return true;
}
b8 vulkanDeviceDetectDepthFormat(gontiVulkanContext* context) {
    const u64 candidateCount = 3;
    VkFormat candidates[candidateCount] = {
        VK_FORMAT_D32_SFLOAT,
        VK_FORMAT_D32_SFLOAT_S8_UINT,
        VK_FORMAT_D24_UNORM_S8_UINT
    };

    u32 flags = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;

    for (u64 i = 0; i < candidateCount; i++) {
        VkFormatProperties properties;

        vkGetPhysicalDeviceFormatProperties(context->device.physicalDevice, candidates[i], &properties);

        if ((properties.linearTilingFeatures & flags) == flags) {
            context->device.depthFormat = candidates[i];
            return true;
        } else if ((properties.optimalTilingFeatures & flags) == flags) {
            context->device.depthFormat = candidates[i];
            return true;
        }
    }

    return false;
}

/*
* VOID
*/
void vulkanDeviceRelease(gontiVulkanContext* context) {
    KINFO("Release device resources...");

    if (context->device.swapchainSupport.formats) {
        k_free(context->device.swapchainSupport.formats, context->device.swapchainSupport.formatCount * sizeof(VkSurfaceFormatKHR), MEMORY_TAG_RENDERER);

        context->device.swapchainSupport.formats = 0;
        context->device.swapchainSupport.formatCount = 0;
    }

    if (context->device.swapchainSupport.presentMode) {
        k_free(context->device.swapchainSupport.presentMode, context->device.swapchainSupport.presentModeCount * sizeof(VkPresentModeKHR), MEMORY_TAG_RENDERER);

        context->device.swapchainSupport.presentMode = 0;
        context->device.swapchainSupport.presentModeCount = 0;
    }

    vulkanPhysicalDeviceRelease(context);
    vulkanLogicalDeviceRelease(context);
}
void vulkanDeviceQuerySwapchainSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, gontiVulkanSwapchainSupportInfo* outSupportInfo) {
    VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &outSupportInfo->capabilities));
    VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &outSupportInfo->formatCount, 0));

    if (outSupportInfo->formatCount != 0) {
        if (!outSupportInfo->formats) {
            outSupportInfo->formats = k_allocate(outSupportInfo->formatCount * sizeof(VkSurfaceFormatKHR), MEMORY_TAG_RENDERER);
        }

        VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &outSupportInfo->formatCount, outSupportInfo->formats));
    }

    VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &outSupportInfo->presentModeCount, 0));

    if (outSupportInfo->presentModeCount != 0) {
        if (!outSupportInfo->presentMode) {
            outSupportInfo->presentMode = k_allocate(outSupportInfo->presentModeCount * sizeof(VkPresentModeKHR), MEMORY_TAG_RENDERER);
        }

        VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &outSupportInfo->presentModeCount, outSupportInfo->presentMode));
    }
}

#endif
#endif