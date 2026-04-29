#include <Hardware/Device/GtVkDevice.h>

#include <GONTI-ENGINE/GONTI.CORE/Source/Logging/GtLogger.h>
#include <GONTI-ENGINE/GONTI.CORE/Source/CStringTools/GtCStrTools.h>
#include <GONTI-ENGINE/GONTI.CORE/Source/Memory/GtMemory.h>
#include <GONTI-ENGINE/GONTI.CORE/Source/Containers/DynamicArray/GtDArray.h>

#include <Hardware/PhysicalDevice/GtVkPDevice.h>
#include <Hardware/LogicalDevice/GtVkLDevice.h>

/*
* B8
*/
GtB8 gontiVkDeviceCreate(GtVkContext* context) {
    if (!gontiVkSelectPhysicalDevice(context)) return GtFalse;

    gontiVkLogicalDeviceInitialize(context);
    gontiVkLogicalDeviceCreate(
        context->device.physicalDevice, 
        &context->device.vkLdDevice.vkLdQueueSharedInfo.deviceCreateInfo, 
        context->allocator, 
        &context->device.logicalDevice
    );
    gontiVkLogicalDeviceQueuesGet(
        &context->device
    );
    gontiVkLogicalDeviceCommandPoolCreate(
        context,
        &context->device,
        &context->device.graphicsCommandPool
    );

    return GtTrue;
}
GtB8 gontiVkDeviceDetectDepthFormat(GtVkContext* context) {
    const GtU64 candidateCount = 3;
    VkFormat candidates[candidateCount];
    candidates[0] = VK_FORMAT_D32_SFLOAT;
    candidates[1] = VK_FORMAT_D32_SFLOAT_S8_UINT;
    candidates[2] = VK_FORMAT_D24_UNORM_S8_UINT;

    GtU32 flags = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;

    for (GtU64 i = 0; i < candidateCount; i++) {
        VkFormatProperties properties;

        vkGetPhysicalDeviceFormatProperties(context->device.physicalDevice, candidates[i], &properties);

        if ((properties.linearTilingFeatures & flags) == flags) {
            context->device.depthFormat = candidates[i];
            return GtTrue;
        } else if ((properties.optimalTilingFeatures & flags) == flags) {
            context->device.depthFormat = candidates[i];
            return GtTrue;
        }
    }

    return GtFalse;
}

/*
* VOID
*/
void gontiVkDeviceRelease(GtVkContext* context) {
    GTINFO("Release device resources...");

    if (context->device.swapchainSupport.formats) {
        gt_free(context->device.swapchainSupport.formats);

        context->device.swapchainSupport.formats = 0;
        context->device.swapchainSupport.formatCount = 0;
    }

    if (context->device.swapchainSupport.presentMode) {
        gt_free(context->device.swapchainSupport.presentMode);

        context->device.swapchainSupport.presentMode = 0;
        context->device.swapchainSupport.presentModeCount = 0;
    }

    gontiVkPhysicalDeviceRelease(context);
    gontiVkLogicalDeviceRelease(context);
}
void gontiVkDeviceQuerySwapchainSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, GtVkSwapchainInfo* outSupportInfo) {
    GTVK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &outSupportInfo->capabilities));
    GTVK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &outSupportInfo->formatCount, 0));

    if (outSupportInfo->formatCount != 0) {
        if (!outSupportInfo->formats) {
            outSupportInfo->formats = gt_allocate(outSupportInfo->formatCount * sizeof(VkSurfaceFormatKHR), GT_MEM_TAG_RENDERER);
        }

        GTVK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &outSupportInfo->formatCount, outSupportInfo->formats));
    }

    GTVK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &outSupportInfo->presentModeCount, 0));

    if (outSupportInfo->presentModeCount != 0) {
        if (!outSupportInfo->presentMode) {
            outSupportInfo->presentMode = gt_allocate(outSupportInfo->presentModeCount * sizeof(VkPresentModeKHR), GT_MEM_TAG_RENDERER);
        }

        GTVK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &outSupportInfo->presentModeCount, outSupportInfo->presentMode));
    }
}
