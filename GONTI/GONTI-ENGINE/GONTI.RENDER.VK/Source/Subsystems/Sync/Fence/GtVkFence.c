#include <Subsystems/Sync/Fence/GtVkFence.h>

#include <stdio.h>
#include <GONTI-ENGINE/GONTI.CORE/Source/Logging/GtLogger.h>
#include <GONTI-ENGINE/GONTI.CORE/Source/Memory/GtMemory.h>

GtB8 gontiVkFenceWait(
    GtVkContext* context,
    GtVkFence* fence,
    GtU64 timeoutNs
) {
    if (!fence) {
        GTERROR("VK FENCE WAIT - fence is NULL");
        return GtFalse;
    }

    if (fence->handle == VK_NULL_HANDLE) {
        GTERROR("VK FENCE WAIT - fence handle is VK_NULL_HANDLE");
        return GtFalse;
    }

    GTDEBUG("VK FENCE WAIT - Waiting for fence handle: %p, isSignaled: %s", 
        (void*)fence->handle, fence->isSignaled ? "GtTrue" : "GtFalse");

    if (fence->isSignaled) {
        GTDEBUG("VK FENCE WAIT - Already marked as signaled, returning GtTrue");
        return GtTrue;
    }

    VkResult quickResult = vkWaitForFences(
        context->device.logicalDevice,
        1,
        &fence->handle,
        GtTrue,
        0 
    );

    if (quickResult == VK_SUCCESS) {
        fence->isSignaled = GtTrue;
        GTDEBUG("VK FENCE WAIT - Already signaled (quick check)");
        return GtTrue;
    }

    if (timeoutNs == GT_U64_MAX) {
        GTWARN("VK FENCE WAIT - Using infinite timeout, this may cause hang!");
        timeoutNs = 5000000000ULL;
    }

    GTDEBUG("VK FENCE WAIT - Fence not signaled, waiting with timeout %llu ns (%.2f seconds)", 
        timeoutNs, (GtF64)timeoutNs / 1000000000.0);

    VkResult result = vkWaitForFences(
        context->device.logicalDevice,
        1,
        &fence->handle,
        GtTrue,
        timeoutNs
    );

    switch (result) {
        case VK_SUCCESS:
            fence->isSignaled = GtTrue;
            GTDEBUG("VK FENCE WAIT - SUCCESS after wait");
            return GtTrue;
        case VK_TIMEOUT:
            GTWARN("VK FENCE WAIT - timed out after %llu ns (%.2f seconds)", 
                timeoutNs, (GtF64)timeoutNs / 1000000000.0);
            return GtFalse;
        case VK_ERROR_DEVICE_LOST:
            GTERROR("VK FENCE WAIT - VK_ERROR_DEVICE_LOST");
            return GtFalse;
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            GTERROR("VK FENCE WAIT - VK_ERROR_OUT_OF_HOST_MEMORY");
            return GtFalse;
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            GTERROR("VK FENCE WAIT - VK_ERROR_OUT_OF_DEVICE_MEMORY");
            return GtFalse;
        default:
            GTERROR("VK FENCE WAIT - Unknown error: %d", result);
            return GtFalse;
    }

    return GtFalse;
}

void gontiVkFenceCreate(
    GtVkContext* context,
    GtB8 createSignaled,
    GtVkFence* outFence
) {
    gt_zeroMemory(outFence, sizeof(GtVkFence));
    
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
        GTERROR("Failed to create fence! VkResult: %d", result);
        outFence->handle = VK_NULL_HANDLE;
        outFence->isSignaled = GtFalse;
        return;
    }

    GTDEBUG("VK FENCE CREATE - Created fence handle: %p, signaled: %s", 
        (void*)outFence->handle, outFence->isSignaled ? "GtTrue" : "GtFalse");
}
void gontiVkFenceDestroy(GtVkContext* context, GtVkFence* fence) {
    if (fence->handle) {
        vkDestroyFence(
            context->device.logicalDevice,
            fence->handle,
            context->allocator
        ); fence->handle = 0;

        fence->isSignaled = GtFalse;
    }
}
void gontiVkFenceReset(GtVkContext* context, GtVkFence* fence) {
    if (!fence) {
        GTERROR("VK FENCE RESET - fence is NULL");
        return;
    }

    if (fence->handle == VK_NULL_HANDLE) {
        GTWARN("VK FENCE RESET - fence handle is VK_NULL_HANDLE");
        return;
    }

    if (fence->isSignaled) {
        VkResult result = vkResetFences(context->device.logicalDevice, 1, &fence->handle);
        if (result == VK_SUCCESS) {
            fence->isSignaled = GtFalse;
            GTDEBUG("VK FENCE RESET - Reset fence handle: %p", (void*)fence->handle);
        } else {
            GTERROR("VK FENCE RESET - Failed to reset fence! VkResult: %d", result);
        }
    } else {
        GTDEBUG("VK FENCE RESET - Fence already not signaled, skipping reset");
    }
}