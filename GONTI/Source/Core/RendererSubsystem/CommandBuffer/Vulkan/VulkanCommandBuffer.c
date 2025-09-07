#include "VulkanCommandBuffer.h"

#ifdef USE_VULKAN
#ifdef CommandBuffer_ON

#include "../../../MemorySubsystem/MemorySubsystem.h"

void vulkanCommandBufferAllocate(
    gontiVulkanContext* context,
    VkCommandPool pool,
    b8 isPrimary,
    gontiVulkanCommandBuffer* outCommandBuffer
) {
    k_zeroMemory(outCommandBuffer, sizeof(gontiVulkanCommandBuffer));

    VkCommandBufferAllocateInfo allocateInfo = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
    allocateInfo.commandPool = pool;
    allocateInfo.level = isPrimary ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
    allocateInfo.commandBufferCount = 1;
    allocateInfo.pNext = 0;

    outCommandBuffer->state = COMMAND_BUFFER_STATE_NOT_ALLOCATED;

    VK_CHECK(vkAllocateCommandBuffers(
        context->device.logicalDevice,
        &allocateInfo,
        &outCommandBuffer->handle
    ));

    outCommandBuffer->state = COMMAND_BUFFER_STATE_READY;
}
void vulkanCommandBufferFree(
    gontiVulkanContext* context,
    VkCommandPool pool,
    gontiVulkanCommandBuffer* commandBuffer
) {
    vkFreeCommandBuffers(
        context->device.logicalDevice,
        pool,
        1,
        &commandBuffer->handle
    );

    commandBuffer->handle = 0;
    commandBuffer->state = COMMAND_BUFFER_STATE_NOT_ALLOCATED;
}
void vulkanCommandBufferBegin(
    gontiVulkanCommandBuffer* commandBuffer,
    b8 isSingleUse,
    b8 isRenderpassContinue,
    b8 isSimultaneous
) {
    VkCommandBufferBeginInfo beginInfo = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
    beginInfo.flags = 0;
    if (isSingleUse) beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    if (isRenderpassContinue) beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
    if (isSimultaneous) beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

    VK_CHECK(vkBeginCommandBuffer(commandBuffer->handle, &beginInfo));
    commandBuffer->state = COMMAND_BUFFER_STATE_RECORDING;
}
void vulkanCommandBufferAllocateAndBeginSingleUse(
    gontiVulkanContext* context,
    VkCommandPool pool,
    gontiVulkanCommandBuffer* outCommandBuffer
) {
    vulkanCommandBufferAllocate(context, pool, true, outCommandBuffer);
    vulkanCommandBufferBegin(outCommandBuffer, true, false, false);
}
void vulkanCommandBufferEndSingleUse(
    gontiVulkanContext* context,
    VkCommandPool pool,
    gontiVulkanCommandBuffer* commandBuffer,
    VkQueue queue
) {
    vulkanCommandBufferEnd(commandBuffer);

    VkSubmitInfo submitInfo = {VK_STRUCTURE_TYPE_SUBMIT_INFO};
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer->handle;

    VK_CHECK(vkQueueSubmit(queue, 1, &submitInfo, 0));
    VK_CHECK(vkQueueWaitIdle(queue));

    vulkanCommandBufferFree(context, pool, commandBuffer);
}
void vulkanCommandBufferEnd(gontiVulkanCommandBuffer* commandBuffer) {
    VK_CHECK(vkEndCommandBuffer(commandBuffer->handle));
    commandBuffer->state = COMMAND_BUFFER_STATE_RECORDING_ENDED;
}
void vulkanCommandBufferSubmitted(gontiVulkanCommandBuffer* commandBuffer) {
    commandBuffer->state = COMMAND_BUFFER_STATE_SUBMITTED;
}
void vulkanCommandBufferReset(gontiVulkanCommandBuffer* commandBuffer) {
    commandBuffer->state = COMMAND_BUFFER_STATE_READY;
}
void vulkanCommandBufferUpdateSubmitted(gontiVulkanCommandBuffer* commandBuffer) {

}

#endif
#endif