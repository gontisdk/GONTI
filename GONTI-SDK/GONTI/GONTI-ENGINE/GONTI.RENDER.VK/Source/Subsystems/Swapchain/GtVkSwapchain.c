#include "GtVkSwapchain.h"

#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Logging/GtLogger.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Memory/GtMemory.h>
#include "../../Hardware/Device/GtVkDevice.h"
#include "../../Hardware/LogicalDevice/GtVkLDevice.h"
#include "../../Hardware/PhysicalDevice/GtVkPDevice.h"
#include "../../Resources/Images/GtVkImgs.h"

/*PRIVATE*/
void create(GtVkContext* context, GtU32 width, GtU32 height, GtVkSwapchain* swapchain) {
    VkExtent2D swapchainExtent = {width, height};
    gontiVkDeviceQuerySwapchainSupport(context->device.physicalDevice, context->surface, &context->device.swapchainSupport);

    swapchain->imageCount = 0;
    swapchain->handle = VK_NULL_HANDLE;

    GtB8 found = GtFalse;
    for (GtU32 i = 0; i < context->device.swapchainSupport.formatCount; i++) {
        VkSurfaceFormatKHR format = context->device.swapchainSupport.formats[i];

        if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            swapchain->imageFormat = format;
            found = GtTrue;
            break;
        }
    }

    if (!found) {
        swapchain->imageFormat = context->device.swapchainSupport.formats[0];
    }

    VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
    for (GtU32 i = 0; i < context->device.swapchainSupport.presentModeCount; i++) {
        VkPresentModeKHR mode = context->device.swapchainSupport.presentMode[i];

        if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
            presentMode = mode;
            break;
        }
    }

    if (context->device.swapchainSupport.capabilities.currentExtent.width != GT_U32_MAX) {
        swapchainExtent = context->device.swapchainSupport.capabilities.currentExtent;
    }

    VkExtent2D min = context->device.swapchainSupport.capabilities.minImageExtent;
    VkExtent2D max = context->device.swapchainSupport.capabilities.maxImageExtent;
    swapchainExtent.width = GT_CLAMP(swapchainExtent.width, min.width, max.width);
    swapchainExtent.height = GT_CLAMP(swapchainExtent.height, min.height, max.height);

    GtU32 imageCount = context->device.swapchainSupport.capabilities.minImageCount + 1;
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
        GtU32 queueFamilyIndices[] = {
            (GtU32)context->device.graphicsQueueIndex,
            (GtU32)context->device.presentQueueIndex
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

    GTVK_CHECK(vkCreateSwapchainKHR(context->device.logicalDevice, &swapchainCreateInfo, context->allocator, &swapchain->handle));

    context->currentFrame = 0;

    GTVK_CHECK(vkGetSwapchainImagesKHR(context->device.logicalDevice, swapchain->handle, &swapchain->imageCount, 0));

    swapchain->maxFramesInFlight = swapchain->imageCount - 1;
    if (swapchain->maxFramesInFlight == 0) swapchain->maxFramesInFlight = 1;
    
    GTINFO("Swapchain imageCount: %d, maxFramesInFlight: %d", swapchain->imageCount, swapchain->maxFramesInFlight);

    swapchain->images = gt_allocate(swapchain->imageCount * sizeof(VkImage), GT_MEM_TAG_RENDERER);
    swapchain->views = gt_allocate(swapchain->imageCount * sizeof(VkImageView), GT_MEM_TAG_RENDERER);

    GTVK_CHECK(vkGetSwapchainImagesKHR(context->device.logicalDevice, swapchain->handle, &swapchain->imageCount, swapchain->images));

    for (GtU32 i = 0; i < swapchain->imageCount; i++) {
        VkImageViewCreateInfo viewInfo = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
        viewInfo.image = swapchain->images[i];
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = swapchain->imageFormat.format;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        GTVK_CHECK(vkCreateImageView(context->device.logicalDevice, &viewInfo, context->allocator, &swapchain->views[i]));
    }

    if (!gontiVkDeviceDetectDepthFormat(context)) {
        context->device.depthFormat = VK_FORMAT_UNDEFINED;
        GTFATAL("Failed to find supported format!");
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
        GtTrue,
        VK_IMAGE_ASPECT_DEPTH_BIT,
        &swapchain->depthAttachment
    );

    GTINFO("Swapchain created successfully.");
}
void destroy(GtVkContext* context, GtVkSwapchain* swapchain) {
    vkDeviceWaitIdle(context->device.logicalDevice);
    gontiVkImageDestroy(context, &swapchain->depthAttachment);

    for (GtU32 i = 0; i < swapchain->imageCount; i++) {
        vkDestroyImageView(context->device.logicalDevice, swapchain->views[i], context->allocator);
    }

    if (swapchain->images) { gt_free(swapchain->images); swapchain->images = 0; }
    if (swapchain->views) { gt_free(swapchain->views); swapchain->views = 0; }
    
    vkDestroySwapchainKHR(context->device.logicalDevice, swapchain->handle, context->allocator);
}

/*PUBLIC*/

GtB8 gontiVkSwapchainAcquireNextImageIndex(GtVkContext* context, GtVkSwapchain* swapchain, GtU64 timeoutNs, VkSemaphore imageAvailableSemaphore, VkFence fence, GtU32* outImageIndex) {
    if (!context || !swapchain || !outImageIndex) {
        GTERROR("Invalid parameters in gontiVkSwapchainAcquireNextImageIndex");
        return GtFalse;
    }

    if (imageAvailableSemaphore == VK_NULL_HANDLE) {
        GTERROR("Invalid semaphore handle passed to vkAcquireNextImageKHR");
        return GtFalse;
    }

    VkResult result = vkAcquireNextImageKHR(
        context->device.logicalDevice, 
        swapchain->handle, 
        timeoutNs, 
        imageAvailableSemaphore, 
        fence,
        outImageIndex
    );

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        GTINFO("Swapchain out of date, recreating...");
        gontiVkSwapchainRecreate(context, context->framebufferWidth, context->framebufferHeight, swapchain);
        return GtFalse;
    } else if (result == VK_SUBOPTIMAL_KHR) {
        GTWARN("Swapchain suboptimal but continuing...");
        return GtTrue;
    } else if (result == VK_TIMEOUT) {
        GTERROR("Timeout acquiring swapchain image");
        return GtFalse;
    } else if (result != VK_SUCCESS) {
        GTERROR("Failed to acquire swapchain image! Result: %d", result);
        return GtFalse;
    }

    if (*outImageIndex >= swapchain->imageCount) {
        GTERROR("Acquired invalid image index: %d (max: %d)", *outImageIndex, swapchain->imageCount);
        return GtFalse;
    }

    return GtTrue;
}

void gontiVkSwapchainCreate(GtVkContext* context, GtU32 width, GtU32 height, GtVkSwapchain* outSwapchain) {
    create(context, width, height, outSwapchain);
}
void gontiVkSwapchainRecreate(GtVkContext* context, GtU32 width, GtU32 height, GtVkSwapchain* swapchain) {
    destroy(context, swapchain);
    create(context, width, height, swapchain);
}
void gontiVkSwapchainDestroy(GtVkContext* context, GtVkSwapchain* swapchain) {
    destroy(context, swapchain);
}
void gontiVkSwapchainDarraysDestroy(GtVkSwapchain* swapchain) {
    if (swapchain->images) {
        gt_free(swapchain->images);
        swapchain->images = 0;
    }

    if (swapchain->views) {
        gt_free(swapchain->views);
        swapchain->views = 0;
    }
}
VkResult gontiVkSwapchainPresent(GtVkContext* context, GtVkSwapchain* swapchain, VkQueue graphicsQueue, VkQueue presentQueue, VkSemaphore renderCompleteSemaphore, GtU32 presentImageIndex) {
    if (!context || !swapchain || !presentQueue || renderCompleteSemaphore == VK_NULL_HANDLE) {
        GTERROR("Invalid parameters in gontiVkSwapchainPresent");
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    if (presentImageIndex >= swapchain->imageCount) {
        GTERROR("Invalid present image index: %d (max: %d)", presentImageIndex, swapchain->imageCount);
        return VK_ERROR_VALIDATION_FAILED_EXT;
    }

    VkPresentInfoKHR presentInfo = {VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &renderCompleteSemaphore;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapchain->handle;
    presentInfo.pImageIndices = &presentImageIndex;
    presentInfo.pResults = 0;
    
    VkResult result = vkQueuePresentKHR(presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        GTINFO("Swapchain out of date or suboptimal, will recreate");
        gontiVkSwapchainRecreate(context, context->framebufferWidth, context->framebufferHeight, swapchain);
    } else if (result != VK_SUCCESS) {
        GTERROR("Failed to present swapchain image! Result: %d", result);
        return result;
    }

    return result;
}
