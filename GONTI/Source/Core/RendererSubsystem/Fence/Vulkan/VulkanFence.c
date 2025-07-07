#include "VulkanFence.h"
            
#ifdef USE_VULKAN
#ifdef Fence_ON

#include "../../../../Utilities/Logging/Scripts/Logger.h"

b8 vulkanFenceWait(
    gontiVulkanContext* context,
    gontiVulkanFence* fence,
    u64 timeoutNs
) {
    if (fence->isSignaled) return true;

    VkResult result = vkWaitForFences(
        context->device.logicalDevice,
        1,
        &fence->handle,
        true,
        timeoutNs
    );

    switch (result) {
        case VK_SUCCESS:
            fence->isSignaled = true;
            return true;
        case VK_TIMEOUT:
            KWARN("VK FENCE WAIT - timed out");
            break;
        case VK_ERROR_DEVICE_LOST:
            KERROR("VK FENCE WAIT - VK_ERROR_DEVICE_LOST");
            break;
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            KERROR("VK FENCE WAIT - VK_ERROR_OUT_OF_HOST_MEMORY");
            break;
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            KERROR("VK FENCE WAIT - VK_ERROR_OUT_OF_DEVICE_MEMORY");
            break;
        default:
            KERROR("VK FENCE WAIT - An unknown error has occurred");
            break;
    }

    return false;
}

void vulkanFenceCreate(
    gontiVulkanContext* context,
    b8 createSignaled,
    gontiVulkanFence* outFence
) {
    outFence->isSignaled = createSignaled;

    VkFenceCreateInfo fenceCreateInfo = {VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
    if (outFence->isSignaled) fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    VK_CHECK(vkCreateFence(
        context->device.logicalDevice,
        &fenceCreateInfo,
        context->allocator,
        &outFence->handle
    ));
}
void vulkanFenceDestroy(gontiVulkanContext* context, gontiVulkanFence* fence) {
    if (fence->handle) {
        vkDestroyFence(
            context->device.logicalDevice,
            fence->handle,
            context->allocator
        ); fence->handle = 0;

        fence->isSignaled = false;
    }
}
void vulkanFenceReset(gontiVulkanContext* context, gontiVulkanFence* fence) {
    if (fence->isSignaled) {
        VK_CHECK(vkResetFences(context->device.logicalDevice, 1, &fence->handle));
        fence->isSignaled = false;
    }
}

#endif
#endif