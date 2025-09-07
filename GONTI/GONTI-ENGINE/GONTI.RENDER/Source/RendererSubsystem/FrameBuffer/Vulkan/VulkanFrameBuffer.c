#include "VulkanFrameBuffer.h"

#ifdef GONTI_USE_VULKAN

#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Memory/Memory.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Logging/Logger.h>

void gontiVkFramebufferCreate(
    GontiVulkanContext* context,
    GontiVulkanRenderpass* renderpass,
    u32 width, u32 height,
    u32 attachmentCount,
    VkImageView* attachments,
    GontiVulkanFramebuffer* outFramebuffer
) {
    outFramebuffer->attachments = k_allocate(attachmentCount * sizeof(VkImageView), GONTI_MEMORY_TAG_RENDERER);

    for (u32 i = 0; i < attachmentCount; i++) {
        outFramebuffer->attachments[i] = attachments[i];
    }

    outFramebuffer->attachmentCount = attachmentCount;
    outFramebuffer->renderpass = renderpass;

    VkFramebufferCreateInfo framebufferCreateInfo = {VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
    framebufferCreateInfo.renderPass = renderpass->handle;
    framebufferCreateInfo.attachmentCount = attachmentCount;
    framebufferCreateInfo.pAttachments = outFramebuffer->attachments;
    framebufferCreateInfo.width = width;
    framebufferCreateInfo.height = height;
    framebufferCreateInfo.layers = 1;

    VK_CHECK(vkCreateFramebuffer(
        context->device.logicalDevice,
        &framebufferCreateInfo,
        context->allocator,
        &outFramebuffer->handle
    ));

    KINFO("Vulkan framebuffer created.");
}
void gontiVkFramebufferDestroy(GontiVulkanContext* context, GontiVulkanFramebuffer* framebuffer) {
    vkDestroyFramebuffer(context->device.logicalDevice, framebuffer->handle, context->allocator);
    framebuffer->handle = 0;

    if (framebuffer->attachments) {
        k_free(framebuffer->attachments, framebuffer->attachmentCount * sizeof(VkImageView), GONTI_MEMORY_TAG_RENDERER);
        framebuffer->attachments = 0;
        framebuffer->attachmentCount = 0;
    }

    framebuffer->renderpass = 0;
}

#endif