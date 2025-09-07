#include "VulkanSyncObjects.h"

#ifdef GONTI_USE_VULKAN

#include <stdio.h>

#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Containers/DynamicArray/DynamicArray.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Logging/Logger.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Memory/Memory.h>
#include "../../Fence/Vulkan/VulkanFence.h"

void gontiVkSyncObjectsCreate(GontiVulkanContext* context) {
    KDEBUG("Creating sync objects for %d frames in flight", context->swapchain.maxFramesInFlight);
    
    if (context->swapchain.maxFramesInFlight == 0) {
        KERROR("Invalid maxFramesInFlight: %d", context->swapchain.maxFramesInFlight);
        return;
    }
    
    if (context->swapchain.maxFramesInFlight > 8) {
        KWARN("Very high maxFramesInFlight: %d, this might indicate a problem", context->swapchain.maxFramesInFlight);
    }

    context->imageAvailableSemaphores = darrayReserve(VkSemaphore, context->swapchain.maxFramesInFlight);
    context->queueCompleteSemaphore = darrayReserve(VkSemaphore, context->swapchain.maxFramesInFlight);
    context->inFlightFences = darrayReserve(GontiVulkanFence, context->swapchain.maxFramesInFlight);

    for (u8 i = 0; i < context->swapchain.maxFramesInFlight; i++) {
        context->imageAvailableSemaphores[i] = VK_NULL_HANDLE;
        context->queueCompleteSemaphore[i] = VK_NULL_HANDLE;
        k_zeroMemory(&context->inFlightFences[i], sizeof(GontiVulkanFence));
    }

    for (u8 i = 0; i < context->swapchain.maxFramesInFlight; i++) {
        VkSemaphoreCreateInfo semaphoreCreateInfo = {VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
        
        VkResult result1 = vkCreateSemaphore(context->device.logicalDevice, &semaphoreCreateInfo, context->allocator, &context->imageAvailableSemaphores[i]);
        VkResult result2 = vkCreateSemaphore(context->device.logicalDevice, &semaphoreCreateInfo, context->allocator, &context->queueCompleteSemaphore[i]);

        if (result1 != VK_SUCCESS) {
            KERROR("Failed to create image available semaphore %d: %d", i, result1);
            context->imageAvailableSemaphores[i] = VK_NULL_HANDLE;
        }

        if (result2 != VK_SUCCESS) {
            KERROR("Failed to create queue complete semaphore %d: %d", i, result2);
            context->queueCompleteSemaphore[i] = VK_NULL_HANDLE;
        }

        gontiVkFenceCreate(context, true, &context->inFlightFences[i]);

        if (context->inFlightFences[i].handle == VK_NULL_HANDLE) {
            KERROR("Failed to create in-flight fence %d", i);
        } else {
            KDEBUG("Created sync objects %d: ImageSem=%p, QueueSem=%p, Fence=%p", 
                i, (void*)context->imageAvailableSemaphores[i],
                (void*)context->queueCompleteSemaphore[i],
                (void*)context->inFlightFences[i].handle);
        }
    }

    context->imagesInFlight = darrayReserve(GontiVulkanFence*, context->swapchain.imageCount);

    for (u32 i = 0; i < context->swapchain.imageCount; i++) {
        context->imagesInFlight[i] = 0;
    }

    KINFO("Sync objects created successfully");
}
void gontiVkSyncObjectsDestroy(GontiVulkanContext* context) {
    vkDeviceWaitIdle(context->device.logicalDevice);
    for (u8 i = 0; i < context->swapchain.maxFramesInFlight; i++) {
        if (context->imageAvailableSemaphores[i]) {
            vkDestroySemaphore(
                context->device.logicalDevice,
                context->imageAvailableSemaphores[i],
                context->allocator
            );
        }
        if (context->queueCompleteSemaphore[i]) {
            vkDestroySemaphore(
                context->device.logicalDevice,
                context->queueCompleteSemaphore[i],
                context->allocator
            );
        }


        gontiVkFenceDestroy(context, &context->inFlightFences[i]);
    }

    darrayDestroy(context->imageAvailableSemaphores);
    darrayDestroy(context->queueCompleteSemaphore);
    darrayDestroy(context->inFlightFences);
    darrayDestroy(context->imagesInFlight);
}

#endif
