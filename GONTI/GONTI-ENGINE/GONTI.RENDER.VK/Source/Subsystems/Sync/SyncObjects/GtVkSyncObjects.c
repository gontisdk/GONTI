#include <Subsystems/Sync/SyncObjects/GtVkSyncObjects.h>

#include <stdio.h>

#include <GONTI-ENGINE/GONTI.CORE/Source/Containers/DynamicArray/GtDArray.h>
#include <GONTI-ENGINE/GONTI.CORE/Source/Logging/GtLogger.h>
#include <GONTI-ENGINE/GONTI.CORE/Source/Memory/GtMemory.h>
#include <Subsystems/Sync/Fence/GtVkFence.h>

void gontiVkSyncObjectsCreate(GtVkContext* context) {
    GTDEBUG("Creating sync objects for %d frames in flight and %d images", 
        context->swapchain.maxFramesInFlight, context->swapchain.imageCount);
    
    if (context->swapchain.maxFramesInFlight == 0 || context->swapchain.imageCount == 0) {
        GTERROR("Invalid maxFramesInFlight: %d or imageCount: %d", 
            context->swapchain.maxFramesInFlight, context->swapchain.imageCount);
        return;
    }

    context->imageAvailableSemaphores = gontiDarrayReserve(VkSemaphore, context->swapchain.maxFramesInFlight);
    context->queueCompleteSemaphore = gontiDarrayReserve(VkSemaphore, context->swapchain.imageCount);
    
    context->inFlightFences = gontiDarrayReserve(GtVkFence, context->swapchain.maxFramesInFlight);

    for (GtU8 i = 0; i < context->swapchain.maxFramesInFlight; i++) {
        context->imageAvailableSemaphores[i] = VK_NULL_HANDLE;
    }

    for (GtU32 i = 0; i < context->swapchain.imageCount; i++) {
        context->queueCompleteSemaphore[i] = VK_NULL_HANDLE;
    }

    for (GtU8 i = 0; i < context->swapchain.maxFramesInFlight; i++) {
        gt_zeroMemory(&context->inFlightFences[i], sizeof(GtVkFence));
    }

    for (GtU8 i = 0; i < context->swapchain.maxFramesInFlight; i++) {
        VkSemaphoreCreateInfo semaphoreCreateInfo = {VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
        
        VkResult result = vkCreateSemaphore(context->device.logicalDevice, &semaphoreCreateInfo, 
            context->allocator, &context->imageAvailableSemaphores[i]);

        if (result != VK_SUCCESS) {
            GTERROR("Failed to create imageAvailable semaphore for frame %d", i);
            return;
        }

        GTDEBUG("Created imageAvailable semaphore for frame %d: %p", 
            i, (void*)context->imageAvailableSemaphores[i]);
    }

    for (GtU32 i = 0; i < context->swapchain.imageCount; i++) {
        VkSemaphoreCreateInfo semaphoreCreateInfo = {VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
        
        VkResult result = vkCreateSemaphore(context->device.logicalDevice, &semaphoreCreateInfo, 
            context->allocator, &context->queueCompleteSemaphore[i]);

        if (result != VK_SUCCESS) {
            GTERROR("Failed to create queueComplete semaphore for image %d", i);
            return;
        }

        GTDEBUG("Created queueComplete semaphore for image %d: %p", 
            i, (void*)context->queueCompleteSemaphore[i]);
    }

    for (GtU8 i = 0; i < context->swapchain.maxFramesInFlight; i++) {
        gontiVkFenceCreate(context, GtTrue, &context->inFlightFences[i]);

        if (context->inFlightFences[i].handle == VK_NULL_HANDLE) {
            GTERROR("Failed to create fence %d", i);
            return;
        }

        GTDEBUG("Created fence %d: %p", i, (void*)context->inFlightFences[i].handle);
    }

    context->imagesInFlight = gontiDarrayReserve(GtVkFence*, context->swapchain.imageCount);

    for (GtU32 i = 0; i < context->swapchain.imageCount; i++) {
        context->imagesInFlight[i] = 0;
    }

    GTINFO("Sync objects created successfully - %d frame imageAvailable, %d image queueComplete, %d fences", 
        context->swapchain.maxFramesInFlight, context->swapchain.imageCount, context->swapchain.maxFramesInFlight);
}

void gontiVkSyncObjectsDestroy(GtVkContext* context) {
    vkDeviceWaitIdle(context->device.logicalDevice);
    
    for (GtU8 i = 0; i < context->swapchain.maxFramesInFlight; i++) {
        if (context->imageAvailableSemaphores && context->imageAvailableSemaphores[i]) {
            vkDestroySemaphore(
                context->device.logicalDevice,
                context->imageAvailableSemaphores[i],
                context->allocator
            );
        }
    }

    for (GtU32 i = 0; i < context->swapchain.imageCount; i++) {
        if (context->queueCompleteSemaphore && context->queueCompleteSemaphore[i]) {
            vkDestroySemaphore(
                context->device.logicalDevice,
                context->queueCompleteSemaphore[i],
                context->allocator
            );
        }
    }
    
    for (GtU8 i = 0; i < context->swapchain.maxFramesInFlight; i++) {
        if (context->inFlightFences) {
            gontiVkFenceDestroy(context, &context->inFlightFences[i]);
        }
    }

    if (context->imageAvailableSemaphores) {
        gontiDarrayDestroy(context->imageAvailableSemaphores);
        context->imageAvailableSemaphores = 0;
    }
    if (context->queueCompleteSemaphore) {
        gontiDarrayDestroy(context->queueCompleteSemaphore);
        context->queueCompleteSemaphore = 0;
    }
    if (context->inFlightFences) {
        gontiDarrayDestroy(context->inFlightFences);
        context->inFlightFences = 0;
    }
    if (context->imagesInFlight) {
        gontiDarrayDestroy(context->imagesInFlight);
        context->imagesInFlight = 0;
    }
}