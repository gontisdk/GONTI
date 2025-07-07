#include "VulkanRenderpass.h"

#ifdef USE_VULKAN
#ifdef Renderpass_ON

#include "../../../MemorySubsystem/MemorySubsystem.h"
#include "../../../../Utilities/Logging/Scripts/Logger.h"

void vulkanRenderpassCreate(
    gontiVulkanContext* context, gontiVulkanRenderpass* outRenderpass,
    f32 x, f32 y, f32 w, f32 h,
    f32 r, f32 g, f32 b, f32 a,
    f32 depth, u32 stencil
) {
    outRenderpass->x = x;
    outRenderpass->y = y;
    outRenderpass->w = w;
    outRenderpass->h = h;

    outRenderpass->r = r;
    outRenderpass->g = g;
    outRenderpass->b = b;
    outRenderpass->a = a;

    outRenderpass->depth = depth;
    outRenderpass->stencil = stencil;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

    // TODO: Make this configurable
    u32 attachmentDescriptionCount = 2;
    VkAttachmentDescription attachmentDescription[attachmentDescriptionCount];

    VkAttachmentDescription colorAttachment;
    colorAttachment.format = context->swapchain.imageFormat.format; // TODO: configurable
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    colorAttachment.flags = 0;

    attachmentDescription[0] = colorAttachment;

    VkAttachmentReference colorAttachmentReference;
    colorAttachmentReference.attachment = 0;
    colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentReference;

    VkAttachmentDescription depthAttachment = {};
    depthAttachment.format = context->device.depthFormat;
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    attachmentDescription[1] = depthAttachment;

    VkAttachmentReference depthAttachmentReference;
    depthAttachmentReference.attachment = 1;
    depthAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    
    subpass.pDepthStencilAttachment = &depthAttachmentReference;
    
    // (TODO: other attachment types: input, resolve, preserve)
    subpass.inputAttachmentCount = 0;
    subpass.pInputAttachments = 0;
    subpass.pResolveAttachments = 0;
    subpass.preserveAttachmentCount = 0;
    subpass.pPreserveAttachments = 0;

    // TODO: make this configurable
    VkSubpassDependency dependency;
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependency.dependencyFlags = 0;
    
    VkRenderPassCreateInfo renderpassCreateInfo = {VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};
    renderpassCreateInfo.attachmentCount = attachmentDescriptionCount;
    renderpassCreateInfo.pAttachments = attachmentDescription;
    renderpassCreateInfo.subpassCount = 1;
    renderpassCreateInfo.pSubpasses = &subpass;
    renderpassCreateInfo.dependencyCount = 1;
    renderpassCreateInfo.pDependencies = &dependency;
    renderpassCreateInfo.pNext = 0;
    renderpassCreateInfo.flags = 0;

    VK_CHECK(vkCreateRenderPass(
        context->device.logicalDevice,
        &renderpassCreateInfo,
        context->allocator,
        &outRenderpass->handle
    ));

    KINFO("Vulkan renderpass created successfully");
}
void vulkanRenderpassBegin(
    gontiVulkanCommandBuffer* commandBuffer,
    gontiVulkanRenderpass* renderpass,
    VkFramebuffer frameBuffer
) {
    VkRenderPassBeginInfo beginInfo = {VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
    beginInfo.renderPass = renderpass->handle;
    beginInfo.framebuffer = frameBuffer;
    beginInfo.renderArea.offset.x = renderpass->x;
    beginInfo.renderArea.offset.y = renderpass->y;
    beginInfo.renderArea.extent.width = renderpass->w;
    beginInfo.renderArea.extent.height = renderpass->h;

    VkClearValue clearValues[2];
    k_zeroMemory(clearValues, 2 * sizeof(VkClearValue));
    clearValues[0].color.float32[0] = renderpass->r;
    clearValues[0].color.float32[1] = renderpass->g;
    clearValues[0].color.float32[2] = renderpass->b;
    clearValues[0].color.float32[3] = renderpass->a;
    clearValues[1].depthStencil.depth = renderpass->depth;
    clearValues[1].depthStencil.stencil = renderpass->stencil;

    beginInfo.clearValueCount = 2;
    beginInfo.pClearValues = clearValues;

    vkCmdBeginRenderPass(commandBuffer->handle, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
    commandBuffer->state = COMMAND_BUFFER_STATE_IN_RENDER_PASS;
}
void vulkanRenderpassEnd(gontiVulkanCommandBuffer* commandBuffer, gontiVulkanRenderpass* renderpass) {
    vkCmdEndRenderPass(commandBuffer->handle);
    commandBuffer->state = COMMAND_BUFFER_STATE_RECORDING;
}
void vulkanRenderpassDestroy(gontiVulkanContext* context, gontiVulkanRenderpass* renderpass) {
    if (renderpass && renderpass->handle) {
        vkDestroyRenderPass(context->device.logicalDevice, renderpass->handle, context->allocator);
        renderpass->handle = 0;
    }
}

#endif
#endif