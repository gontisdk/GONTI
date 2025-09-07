#include "VulkanSyncObjects.h"

#ifdef GONTI_USE_VULKAN

#include <stdio.h>

#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Containers/DynamicArray/DynamicArray.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Logging/Logger.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Memory/Memory.h>
#include "../../Fence/Vulkan/VulkanFence.h"

void gontiVkSyncObjectsCreate(GontiVulkanContext* context) {
    KDEBUG("Creating sync objects for %d frames in flight and %d images", 
        context->swapchain.maxFramesInFlight, context->swapchain.imageCount);
    
    if (context->swapchain.maxFramesInFlight == 0 || context->swapchain.imageCount == 0) {
        KERROR("Invalid maxFramesInFlight: %d or imageCount: %d", 
            context->swapchain.maxFramesInFlight, context->swapchain.imageCount);
        return;
    }

    context->imageAvailableSemaphores = darrayReserve(VkSemaphore, context->swapchain.maxFramesInFlight);
    context->queueCompleteSemaphore = darrayReserve(VkSemaphore, context->swapchain.imageCount);
    
    context->inFlightFences = darrayReserve(GontiVulkanFence, context->swapchain.maxFramesInFlight);

    for (u8 i = 0; i < context->swapchain.maxFramesInFlight; i++) {
        context->imageAvailableSemaphores[i] = VK_NULL_HANDLE;
    }

    for (u32 i = 0; i < context->swapchain.imageCount; i++) {
        context->queueCompleteSemaphore[i] = VK_NULL_HANDLE;
    }

    for (u8 i = 0; i < context->swapchain.maxFramesInFlight; i++) {
        k_zeroMemory(&context->inFlightFences[i], sizeof(GontiVulkanFence));
    }

    for (u8 i = 0; i < context->swapchain.maxFramesInFlight; i++) {
        VkSemaphoreCreateInfo semaphoreCreateInfo = {VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
        
        VkResult result = vkCreateSemaphore(context->device.logicalDevice, &semaphoreCreateInfo, 
            context->allocator, &context->imageAvailableSemaphores[i]);

        if (result != VK_SUCCESS) {
            KERROR("Failed to create imageAvailable semaphore for frame %d", i);
            return;
        }

        KDEBUG("Created imageAvailable semaphore for frame %d: %p", 
            i, (void*)context->imageAvailableSemaphores[i]);
    }

    for (u32 i = 0; i < context->swapchain.imageCount; i++) {
        VkSemaphoreCreateInfo semaphoreCreateInfo = {VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
        
        VkResult result = vkCreateSemaphore(context->device.logicalDevice, &semaphoreCreateInfo, 
            context->allocator, &context->queueCompleteSemaphore[i]);

        if (result != VK_SUCCESS) {
            KERROR("Failed to create queueComplete semaphore for image %d", i);
            return;
        }

        KDEBUG("Created queueComplete semaphore for image %d: %p", 
            i, (void*)context->queueCompleteSemaphore[i]);
    }

    for (u8 i = 0; i < context->swapchain.maxFramesInFlight; i++) {
        gontiVkFenceCreate(context, true, &context->inFlightFences[i]);

        if (context->inFlightFences[i].handle == VK_NULL_HANDLE) {
            KERROR("Failed to create fence %d", i);
            return;
        }

        KDEBUG("Created fence %d: %p", i, (void*)context->inFlightFences[i].handle);
    }

    context->imagesInFlight = darrayReserve(GontiVulkanFence*, context->swapchain.imageCount);

    for (u32 i = 0; i < context->swapchain.imageCount; i++) {
        context->imagesInFlight[i] = 0;
    }

    KINFO("Sync objects created successfully - %d frame imageAvailable, %d image queueComplete, %d fences", 
        context->swapchain.maxFramesInFlight, context->swapchain.imageCount, context->swapchain.maxFramesInFlight);
}

void gontiVkSyncObjectsDestroy(GontiVulkanContext* context) {
    vkDeviceWaitIdle(context->device.logicalDevice);
    
    for (u8 i = 0; i < context->swapchain.maxFramesInFlight; i++) {
        if (context->imageAvailableSemaphores && context->imageAvailableSemaphores[i]) {
            vkDestroySemaphore(
                context->device.logicalDevice,
                context->imageAvailableSemaphores[i],
                context->allocator
            );
        }
    }

    for (u32 i = 0; i < context->swapchain.imageCount; i++) {
        if (context->queueCompleteSemaphore && context->queueCompleteSemaphore[i]) {
            vkDestroySemaphore(
                context->device.logicalDevice,
                context->queueCompleteSemaphore[i],
                context->allocator
            );
        }
    }
    
    for (u8 i = 0; i < context->swapchain.maxFramesInFlight; i++) {
        if (context->inFlightFences) {
            gontiVkFenceDestroy(context, &context->inFlightFences[i]);
        }
    }

    if (context->imageAvailableSemaphores) {
        darrayDestroy(context->imageAvailableSemaphores);
        context->imageAvailableSemaphores = 0;
    }
    if (context->queueCompleteSemaphore) {
        darrayDestroy(context->queueCompleteSemaphore);
        context->queueCompleteSemaphore = 0;
    }
    if (context->inFlightFences) {
        darrayDestroy(context->inFlightFences);
        context->inFlightFences = 0;
    }
    if (context->imagesInFlight) {
        darrayDestroy(context->imagesInFlight);
        context->imagesInFlight = 0;
    }
}

#endif
