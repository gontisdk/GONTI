#include "VulkanSwapchain.h"

#if GONTI_USE_VULKAN

#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Logging/Logger.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Memory/Memory.h>
#include "../../../Hardware/Device/Vulkan/VulkanDevice.h"
#include "../../../Hardware/LogicalDevice/Vulkan/VulkanLogicalDevice.h"
#include "../../../Hardware/PhysicalDevice/Vulkan/VulkanPhysicalDevice.h"
#include "../../Images/Vulkan/VulkanImages.h"

/*PRIVATE*/
void create(GontiVulkanContext* context, u32 width, u32 height, GontiVulkanSwapchain* swapchain) {
    VkExtent2D swapchainExtent = {width, height};

    if (!swapchain->images && !swapchain->views) {
        k_zeroMemory(swapchain, sizeof(GontiVulkanSwapchain));
    }

    b8 found = false;

    for (u32 i = 0; i < context->device.swapchainSupport.formatCount; i++) {
        VkSurfaceFormatKHR format = context->device.swapchainSupport.formats[i];

        if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            swapchain->imageFormat = format;
            found = true;
            break;
        }
    }

    if (!found) {
        swapchain->imageFormat = context->device.swapchainSupport.formats[0];
    }

    VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;

    for (u32 i = 0; i < context->device.swapchainSupport.presentModeCount; i++) {
        VkPresentModeKHR mode = context->device.swapchainSupport.presentMode[i];

        if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
            presentMode = mode;
            break;
        }
    }

    gontiVkDeviceQuerySwapchainSupport(context->device.physicalDevice, context->surface, &context->device.swapchainSupport);

    if (context->device.swapchainSupport.capabilities.currentExtent.width != UINT32_MAX) {
        swapchainExtent = context->device.swapchainSupport.capabilities.currentExtent;
    }

    VkExtent2D min = context->device.swapchainSupport.capabilities.minImageExtent;
    VkExtent2D max = context->device.swapchainSupport.capabilities.maxImageExtent;

    swapchainExtent.width = KCLAMP(swapchainExtent.width, min.width, max.width);
    swapchainExtent.height = KCLAMP(swapchainExtent.height, min.height, max.height);

    u32 imageCount = context->device.swapchainSupport.capabilities.minImageCount + 1;

    if (context->device.swapchainSupport.capabilities.maxImageCount > 0 && imageCount > context->device.swapchainSupport.capabilities.maxImageCount) {
        imageCount = context->device.swapchainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR swapchainCreateInfo = {VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
    swapchainCreateInfo.surface = context->surface;
    swapchainCreateInfo.minImageCount = imageCount;
    swapchainCreateInfo.imageFormat = swapchain->imageFormat.format;
    swapchainCreateInfo.imageColorSpace = swapchain->imageFormat.colorSpace;
    swapchainCreateInfo.imageExtent = swapchainExtent;
    swapchainCreateInfo.imageArrayLayers = 1;
    swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    if (context->device.graphicsQueueIndex != context->device.presentQueueIndex) {
        u32 queueFamilyIndices[] = {
            (u32)context->device.graphicsQueueIndex,
            (u32)context->device.presentQueueIndex
        };

        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchainCreateInfo.queueFamilyIndexCount = 2;
        swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapchainCreateInfo.queueFamilyIndexCount = 0;
        swapchainCreateInfo.pQueueFamilyIndices = 0;
    }

    swapchainCreateInfo.preTransform = context->device.swapchainSupport.capabilities.currentTransform;
    swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainCreateInfo.presentMode = presentMode;
    swapchainCreateInfo.clipped = VK_TRUE;
    swapchainCreateInfo.oldSwapchain = 0;

    VK_CHECK(vkCreateSwapchainKHR(context->device.logicalDevice, &swapchainCreateInfo, context->allocator, &swapchain->handle));

    context->currentFrame = 0;
    swapchain->imageCount = 0;

    VK_CHECK(vkGetSwapchainImagesKHR(context->device.logicalDevice, swapchain->handle, &swapchain->imageCount, 0));

    swapchain->maxFramesInFlight = (swapchain->imageCount > 2) ? 2 : swapchain->imageCount;
    
    if (swapchain->maxFramesInFlight == 0) {
        KERROR("Calculated maxFramesInFlight is 0, setting to 1");
        swapchain->maxFramesInFlight = 1;
    }
    
    KINFO("Swapchain imageCount: %d, maxFramesInFlight: %d", swapchain->imageCount, swapchain->maxFramesInFlight);

    if (!swapchain->images) {
        swapchain->images = k_allocate(swapchain->imageCount * sizeof(VkImage), GONTI_MEMORY_TAG_RENDERER);
    }

    if (!swapchain->views) {
        swapchain->views = k_allocate(swapchain->imageCount * sizeof(VkImage), GONTI_MEMORY_TAG_RENDERER);
    }

    VK_CHECK(vkGetSwapchainImagesKHR(context->device.logicalDevice, swapchain->handle, &swapchain->imageCount, swapchain->images));

    for (u32 i = 0; i < swapchain->imageCount; i++) {
        VkImageViewCreateInfo viewInfo = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
        viewInfo.image = swapchain->images[i];
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = swapchain->imageFormat.format;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        VK_CHECK(vkCreateImageView(context->device.logicalDevice, &viewInfo, context->allocator, &swapchain->views[i]));
    }

    if (!gontiVkDeviceDetectDepthFormat(context)) {
        context->device.depthFormat = VK_FORMAT_UNDEFINED;
        KFATAL("Failed to find supported format!");
    }

    gontiVkImageCreate(
        context,
        VK_IMAGE_TYPE_2D,
        swapchainExtent.width,
        swapchainExtent.height,
        context->device.depthFormat,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        true,
        VK_IMAGE_ASPECT_DEPTH_BIT,
        &swapchain->depthAttachment
    );

    KINFO("Swapchain created successfully.");
}
void destroy(GontiVulkanContext* context, GontiVulkanSwapchain* swapchain) {
    vkDeviceWaitIdle(context->device.logicalDevice);
    gontiVkImageDestroy(context, &swapchain->depthAttachment);

    for (u32 i = 0; i < swapchain->imageCount; i++) {
        vkDestroyImageView(context->device.logicalDevice, swapchain->views[i], context->allocator);
    }

    vkDestroySwapchainKHR(context->device.logicalDevice, swapchain->handle, context->allocator);
}

/*PUBLIC*/

b8 gontiVkSwapchainAcquireNextImageIndex(GontiVulkanContext* context, GontiVulkanSwapchain* swapchain, u64 timeoutNs, VkSemaphore imageAnavibleSemaphore, VkFence fence, u32* outImageIndex) {
    if (imageAnavibleSemaphore == VK_NULL_HANDLE) {
        KERROR("Invalid semaphore handle passed to vkAcquireNextImageKHR");
        return false;
    }

    VkResult result = vkAcquireNextImageKHR(
        context->device.logicalDevice, 
        swapchain->handle, 
        timeoutNs, 
        imageAnavibleSemaphore, 
        fence,
        outImageIndex
    );

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        gontiVkSwapchainRecreate(context, context->framebufferWidth, context->framebufferHeight, swapchain);
        return false;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        KFATAL("Failed to acquire swapchain image! Result: %d", result);
        return false;
    }

    return true;
}

void gontiVkSwapchainCreate(GontiVulkanContext* context, u32 width, u32 height, GontiVulkanSwapchain* outSwapchain) {
    create(context, width, height, outSwapchain);
}
void gontiVkSwapchainRecreate(GontiVulkanContext* context, u32 width, u32 height, GontiVulkanSwapchain* swapchain) {
    destroy(context, swapchain);
    create(context, width, height, swapchain);
}
void gontiVkSwapchainDestroy(GontiVulkanContext* context, GontiVulkanSwapchain* swapchain) {
    destroy(context, swapchain);
}
void gontiVkSwapchainPresent(GontiVulkanContext* context, GontiVulkanSwapchain* swapchain, VkQueue graphicsQueue, VkQueue presentQueue, VkSemaphore renderCompleteSemaphore, u32 presentImageIndex) {
    VkPresentInfoKHR presentInfo = {VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &renderCompleteSemaphore;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapchain->handle;
    presentInfo.pImageIndices = &presentImageIndex;
    presentInfo.pResults = 0;
    
    VkResult result = vkQueuePresentKHR(presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        gontiVkSwapchainRecreate(context, context->framebufferWidth, context->framebufferHeight, swapchain);
    } else if (result != VK_SUCCESS) {
        KFATAL("Failed to present swapchain image!");
    }

    context->currentFrame = (context->currentFrame + 1) % swapchain->maxFramesInFlight;
}

#endif