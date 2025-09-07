#include "VulkanCommandBuffer.h"

#ifdef GONTI_USE_VULKAN

#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Memory/Memory.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Logging/Logger.h>

void gontiVkCommandBufferAllocate(
    GontiVulkanContext* context,
    VkCommandPool pool,
    b8 isPrimary,
    GontiVulkanCommandBuffer* outCommandBuffer
) {
    k_zeroMemory(outCommandBuffer, sizeof(GontiVulkanCommandBuffer));

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
void gontiVkCommandBufferFree(
    GontiVulkanContext* context,
    VkCommandPool pool,
    GontiVulkanCommandBuffer* commandBuffer
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
void gontiVkCommandBufferBegin(
    GontiVulkanCommandBuffer* commandBuffer,
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
void gontiVkCommandBufferAllocateAndBeginSingleUse(
    GontiVulkanContext* context,
    VkCommandPool pool,
    GontiVulkanCommandBuffer* outCommandBuffer
) {
    gontiVkCommandBufferAllocate(context, pool, true, outCommandBuffer);
    gontiVkCommandBufferBegin(outCommandBuffer, true, false, false);
}
void gontiVkCommandBufferEndSingleUse(
    GontiVulkanContext* context,
    VkCommandPool pool,
    GontiVulkanCommandBuffer* commandBuffer,
    VkQueue queue
) {
    gontiVkCommandBufferEnd(commandBuffer);

    VkSubmitInfo submitInfo = {VK_STRUCTURE_TYPE_SUBMIT_INFO};
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer->handle;

    VK_CHECK(vkQueueSubmit(queue, 1, &submitInfo, 0));
    VK_CHECK(vkQueueWaitIdle(queue));

    gontiVkCommandBufferFree(context, pool, commandBuffer);
}
void gontiVkCommandBufferEnd(GontiVulkanCommandBuffer* commandBuffer) {
    VK_CHECK(vkEndCommandBuffer(commandBuffer->handle));
    commandBuffer->state = COMMAND_BUFFER_STATE_RECORDING_ENDED;
}
void gontiVkCommandBufferSubmitted(GontiVulkanCommandBuffer* commandBuffer) {
    commandBuffer->state = COMMAND_BUFFER_STATE_SUBMITTED;
}
void gontiVkCommandBufferReset(GontiVulkanCommandBuffer* commandBuffer) {
    commandBuffer->state = COMMAND_BUFFER_STATE_READY;
}
void gontiVkCommandBufferUpdateSubmitted(GontiVulkanCommandBuffer* commandBuffer) {

}

#endif