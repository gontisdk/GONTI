#include "VulkanFence.h"
            
#ifdef GONTI_USE_VULKAN

#include <stdio.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Logging/Logger.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Memory/Memory.h>

b8 gontiVkFenceWait(
    GontiVulkanContext* context,
    GontiVulkanFence* fence,
    u64 timeoutNs
) {
    if (!fence) {
        KERROR("VK FENCE WAIT - fence is NULL");
        return false;
    }

    if (fence->handle == VK_NULL_HANDLE) {
        KERROR("VK FENCE WAIT - fence handle is VK_NULL_HANDLE");
        return false;
    }

    KDEBUG("VK FENCE WAIT - Waiting for fence handle: %p, isSignaled: %s", 
        (void*)fence->handle, fence->isSignaled ? "true" : "false");

    if (fence->isSignaled) return true;

    VkResult quickResult = vkWaitForFences(
        context->device.logicalDevice,
        1,
        &fence->handle,
        true,
        0
    );

    if (quickResult == VK_SUCCESS) {
        fence->isSignaled = true;
        KDEBUG("VK FENCE WAIT - Already signaled");
        return true;
    }

    KDEBUG("VK FENCE WAIT - Fence not signaled, waiting with timeout %llu", timeoutNs);

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
            KDEBUG("VK FENCE WAIT - SUCCESS");
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
            KERROR("VK FENCE WAIT - An unknown error has occurred: %d", result);
            break;
    }

    return false;
}

void gontiVkFenceCreate(
    GontiVulkanContext* context,
    b8 createSignaled,
    GontiVulkanFence* outFence
) {
    k_zeroMemory(outFence, sizeof(GontiVulkanFence));
    
    outFence->isSignaled = createSignaled;

    VkFenceCreateInfo fenceCreateInfo = {VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
    if (outFence->isSignaled) fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    VkResult result = vkCreateFence(
        context->device.logicalDevice,
        &fenceCreateInfo,
        context->allocator,
        &outFence->handle
    );

    if (result != VK_SUCCESS) {
        KERROR("Failed to create fence! VkResult: %d", result);
        outFence->handle = VK_NULL_HANDLE;
        outFence->isSignaled = false;
        return;
    }

    KDEBUG("VK FENCE CREATE - Created fence handle: %p, signaled: %s", 
        (void*)outFence->handle, outFence->isSignaled ? "true" : "false");
}
void gontiVkFenceDestroy(GontiVulkanContext* context, GontiVulkanFence* fence) {
    if (fence->handle) {
        vkDestroyFence(
            context->device.logicalDevice,
            fence->handle,
            context->allocator
        ); fence->handle = 0;

        fence->isSignaled = false;
    }
}
void gontiVkFenceReset(GontiVulkanContext* context, GontiVulkanFence* fence) {
    if (!fence) {
        KERROR("VK FENCE RESET - fence is NULL");
        return;
    }

    if (fence->handle == VK_NULL_HANDLE) {
        KWARN("VK FENCE RESET - fence handle is VK_NULL_HANDLE");
        return;
    }

    if (fence->isSignaled) {
        VkResult result = vkResetFences(context->device.logicalDevice, 1, &fence->handle);
        if (result == VK_SUCCESS) {
            fence->isSignaled = false;
            KDEBUG("VK FENCE RESET - Reset fence handle: %p", (void*)fence->handle);
        } else {
            KERROR("VK FENCE RESET - Failed to reset fence! VkResult: %d", result);
        }
    }
}

#endif