#include <Subsystems/CommandBuffer/GtVkCmdBuffer.h>

#include <GONTI-ENGINE/GONTI.CORE/Source/Memory/GtMemory.h>
#include <GONTI-ENGINE/GONTI.CORE/Source/Logging/GtLogger.h>
#include <GONTI-ENGINE/GONTI.CORE/Source/Containers/DynamicArray/GtDArray.h>

void gontiVkCommandBufferAllocate(
    GtVkContext* context,
    VkCommandPool pool,
    GtB8 isPrimary,
    GtVkCmdBuffer* outCommandBuffer
) {
    gt_zeroMemory(outCommandBuffer, sizeof(GtVkCmdBuffer));

    VkCommandBufferAllocateInfo allocateInfo = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
    allocateInfo.commandPool = pool;
    allocateInfo.level = isPrimary ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
    allocateInfo.commandBufferCount = 1;
    allocateInfo.pNext = 0;

    outCommandBuffer->state = GT_VK_CMD_BUFFER_STATE_NOT_ALLOCATED;

    GTVK_CHECK(vkAllocateCommandBuffers(
        context->device.logicalDevice,
        &allocateInfo,
        &outCommandBuffer->handle
    ));

    outCommandBuffer->state = GT_VK_CMD_BUFFER_STATE_READY;
}
void gontiVkCommandBufferFree(
    GtVkContext* context,
    VkCommandPool pool,
    GtVkCmdBuffer* commandBuffer
) {
    vkFreeCommandBuffers(
        context->device.logicalDevice,
        pool,
        1,
        &commandBuffer->handle
    );

    commandBuffer->handle = 0;
    commandBuffer->state = GT_VK_CMD_BUFFER_STATE_NOT_ALLOCATED;
}
void gontiVkCommandBufferBegin(
    GtVkCmdBuffer* commandBuffer,
    GtB8 isSingleUse,
    GtB8 isRenderpassContinue,
    GtB8 isSimultaneous
) {
    VkCommandBufferBeginInfo beginInfo = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
    beginInfo.flags = 0;
    if (isSingleUse) beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    if (isRenderpassContinue) beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
    if (isSimultaneous) beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

    GTVK_CHECK(vkBeginCommandBuffer(commandBuffer->handle, &beginInfo));
    commandBuffer->state = GT_VK_CMD_BUFFER_STATE_RECORDING;
}
void gontiVkCommandBufferAllocateAndBeginSingleUse(
    GtVkContext* context,
    VkCommandPool pool,
    GtVkCmdBuffer* outCommandBuffer
) {
    gontiVkCommandBufferAllocate(context, pool, GtTrue, outCommandBuffer);
    gontiVkCommandBufferBegin(outCommandBuffer, GtTrue, GtFalse, GtFalse);
}
void gontiVkCommandBufferEndSingleUse(
    GtVkContext* context,
    VkCommandPool pool,
    GtVkCmdBuffer* commandBuffer,
    VkQueue queue
) {
    gontiVkCommandBufferEnd(commandBuffer);

    VkSubmitInfo submitInfo = {VK_STRUCTURE_TYPE_SUBMIT_INFO};
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer->handle;

    GTVK_CHECK(vkQueueSubmit(queue, 1, &submitInfo, 0));
    GTVK_CHECK(vkQueueWaitIdle(queue));

    gontiVkCommandBufferFree(context, pool, commandBuffer);
}
void gontiVkCommandBufferEnd(GtVkCmdBuffer* commandBuffer) {
    GTVK_CHECK(vkEndCommandBuffer(commandBuffer->handle));
    commandBuffer->state = GT_VK_CMD_BUFFER_STATE_RECORDING_ENDED;
}
void gontiVkCommandBufferSubmitted(GtVkCmdBuffer* commandBuffer) {
    commandBuffer->state = GT_VK_CMD_BUFFER_STATE_SUBMITTED;
}
void gontiVkCommandBufferReset(GtVkCmdBuffer* commandBuffer) {
    commandBuffer->state = GT_VK_CMD_BUFFER_STATE_READY;
}
void gontiVkCommandBufferUpdateSubmitted(GtVkCmdBuffer* commandBuffer) {

}
void gontiVkCommandBuffersCreate(GtVkContext* context) {
    if (!context->graphicsCommandBuffers) {
        context->graphicsCommandBuffers = gontiDarrayReserve(GtVkCmdBuffer, context->swapchain.imageCount);

        for (GtU32 i = 0; i < context->swapchain.imageCount; i++) {
            gt_zeroMemory(&context->graphicsCommandBuffers[i], sizeof(GtVkCmdBuffer));
        }
    }

    for (GtU32 i = 0; i < context->swapchain.imageCount; i++) {
        if (context->graphicsCommandBuffers[i].handle) {
            gontiVkCommandBufferFree(
                context,
                context->device.graphicsCommandPool,
                &context->graphicsCommandBuffers[i]
            );
        }
        
        gontiVkCommandBufferAllocate(
            context,
            context->device.graphicsCommandPool,
            GtTrue,
            &context->graphicsCommandBuffers[i]
        );
    }

    GTINFO("Vulkan command buffers created");
}
