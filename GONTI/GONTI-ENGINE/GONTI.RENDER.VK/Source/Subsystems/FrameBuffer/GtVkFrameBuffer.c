#include <Subsystems/FrameBuffer/GtVkFrameBuffer.h>

#include <GONTI-ENGINE/GONTI.CORE/Source/Memory/GtMemory.h>
#include <GONTI-ENGINE/GONTI.CORE/Source/Logging/GtLogger.h>

void gontiVkFramebufferCreate(
    GtVkContext* context,
    GtVkRenderpass* renderpass,
    GtU32 width, GtU32 height,
    GtU32 attachmentCount,
    VkImageView* attachments,
    GtVkFramebuffer* outFramebuffer
) {
    outFramebuffer->attachments = gt_allocate(attachmentCount * sizeof(VkImageView), GT_MEM_TAG_RENDERER);

    for (GtU32 i = 0; i < attachmentCount; i++) {
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

    GTVK_CHECK(vkCreateFramebuffer(
        context->device.logicalDevice,
        &framebufferCreateInfo,
        context->allocator,
        &outFramebuffer->handle
    ));

    GTINFO("Vulkan framebuffer created.");
}
void gontiVkFramebufferDestroy(GtVkContext* context, GtVkFramebuffer* framebuffer) {
    vkDeviceWaitIdle(context->device.logicalDevice);
    vkDestroyFramebuffer(context->device.logicalDevice, framebuffer->handle, context->allocator);
    framebuffer->handle = 0;

    if (framebuffer->attachments) {
        gt_free(framebuffer->attachments);
        framebuffer->attachments = 0;
        framebuffer->attachmentCount = 0;
    }

    framebuffer->renderpass = 0;
}
void gontiVkFramebuffersRegenerate(GtVkContext* context, GtVkSwapchain* swapchain, GtVkRenderpass* renderpass) {
    for (GtU32 i = 0; i < swapchain->imageCount; i++) {
        GTINFO("Vulkan creating framebuffers...");

        // TODO: make this dynamic based on the currently configured attachments
        GtU32 attachmentsCount = 2;
        VkImageView attachments[] = {
            swapchain->views[i],
            swapchain->depthAttachment.view
        };

        gontiVkFramebufferCreate(
            context,
            renderpass,
            context->framebufferWidth,
            context->framebufferHeight,
            attachmentsCount,
            attachments,
            &context->swapchain.framebuffers[i]
        );
    }
}