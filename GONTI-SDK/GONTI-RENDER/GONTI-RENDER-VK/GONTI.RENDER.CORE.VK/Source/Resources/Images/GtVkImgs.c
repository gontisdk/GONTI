#include <Resources/Images/GtVkImgs.h>

#include <GONTI-CORE/GONTI.CORE/Source/Logging/GtLogger.h>
#include <GONTI-CORE/GONTI.CORE/Source/Memory/GtMemory.h>
#include <Hardware/Device/GtVkDevice.h>
#include <Hardware/LogicalDevice/GtVkLDevice.h>
#include <Hardware/PhysicalDevice/GtVkPDevice.h>
#include <GONTI-RENDER/GONTI-RENDER-VK/GONTI.RENDER.UTILITIES.VK/Source/GtVkUtil.h>

void gontiVkImageCreate(
    GtVkContext* context,
    VkImageType imageType,
    GtU32 width, GtU32 height,
    VkFormat format,
    VkImageTiling tiling,
    VkImageUsageFlags usage,
    VkMemoryPropertyFlags memoryFlags,
    GtB32 createView,
    VkImageAspectFlags viewAspectFlags,
    GtVkImage* outImage
) {
    outImage->width = width;
    outImage->height = height;

    VkImageCreateInfo imageCreateInfo = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imageCreateInfo.extent.width = width;
    imageCreateInfo.extent.height = height;
    imageCreateInfo.extent.depth = 1; // TODO: Support configurable depth
    imageCreateInfo.mipLevels = 4;    // TODO: Support mip mapping
    imageCreateInfo.arrayLayers = 1;  // TODO: Support number of layers in the image
    imageCreateInfo.format = format;
    imageCreateInfo.tiling = tiling;
    imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageCreateInfo.usage = usage;
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;         // TODO: Configurable sample count
    imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; // TODO: Configurable sharing mode

    GTVK_CHECK(vkCreateImage(context->device.logicalDevice, &imageCreateInfo, context->allocator, &outImage->handle));

    VkMemoryRequirements memoryRequirements;
    vkGetImageMemoryRequirements(context->device.logicalDevice, outImage->handle, &memoryRequirements);

    GtI32 memoryType = context->gontiVkFindMemoryIndex(memoryRequirements.memoryTypeBits, memoryFlags);

    if (memoryType == -1) {
        GTERROR("Required memory type not found. Image not valid.");
    }

    VkMemoryAllocateInfo memoryAllocateInfo = {VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    memoryAllocateInfo.allocationSize = memoryRequirements.size;
    memoryAllocateInfo.memoryTypeIndex = memoryType;

    GTVK_CHECK(vkAllocateMemory(context->device.logicalDevice, &memoryAllocateInfo, context->allocator, &outImage->memory));
    GTVK_CHECK(vkBindImageMemory(context->device.logicalDevice, outImage->handle, outImage->memory, 0)) // TODO: Configurable memory offset

    if (createView) {
        outImage->view = 0;
        gontiVkImageViewCreate(context, format, outImage, viewAspectFlags);
    }
}

void gontiVkImageViewCreate(
    GtVkContext* context,
    VkFormat format,
    GtVkImage* image,
    VkImageAspectFlags aspectFlags
) {
    VkImageViewCreateInfo viewCreateInfo = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
    viewCreateInfo.image = image->handle;
    viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D; // TODO: Make configurable
    viewCreateInfo.format = format;
    viewCreateInfo.subresourceRange.aspectMask = aspectFlags;

    // TODO: Make configurable
    viewCreateInfo.subresourceRange.baseMipLevel = 0;
    viewCreateInfo.subresourceRange.levelCount = 1;
    viewCreateInfo.subresourceRange.baseArrayLayer = 0;
    viewCreateInfo.subresourceRange.layerCount = 1;

    GTVK_CHECK(vkCreateImageView(context->device.logicalDevice, &viewCreateInfo, context->allocator, &image->view));
}

void gontiVkImageDestroy(
    GtVkContext* context,
    GtVkImage* image
) {
    if (image->view) {
        vkDestroyImageView(context->device.logicalDevice, image->view, context->allocator);
        image->view = 0;
    }

    if (image->memory) {
        vkFreeMemory(context->device.logicalDevice, image->memory, context->allocator);
        image->memory = 0;
    }

    if (image->handle) {
        vkDestroyImage(context->device.logicalDevice, image->handle, context->allocator);
        image->handle = 0;
    }
}
