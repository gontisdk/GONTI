#include "VulkanSyncObjects.h"

#ifdef GONTI_USE_VULKAN

#include <stdio.h>

#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Containers/DynamicArray/DynamicArray.h>
#include "../../Fence/Vulkan/VulkanFence.h"

void gontiVkSyncObjectsCreate(GontiVulkanContext* context) {
    context->imageAvailableSemaphores = darrayReserve(VkSemaphore, context->swapchain.maxFramesInFlight);
    context->queueCompleteSemaphore = darrayReserve(VkSemaphore, context->swapchain.maxFramesInFlight);
    context->inFlightFences = darrayReserve(GontiVulkanFence, context->swapchain.maxFramesInFlight);

    for (u8 i = 0; i < context->swapchain.maxFramesInFlight; i++) {
        VkSemaphoreCreateInfo semaphoreCreateInfo = {VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
        
        vkCreateSemaphore(context->device.logicalDevice, &semaphoreCreateInfo, context->allocator, &context->imageAvailableSemaphores[i]);
        vkCreateSemaphore(context->device.logicalDevice, &semaphoreCreateInfo, context->allocator, &context->queueCompleteSemaphore[i]);

        gontiVkFenceCreate(context, true, &context->inFlightFences[i]);
    }

    context->imagesInFlight = darrayReserve(GontiVulkanFence*, context->swapchain.imageCount);

    for (u32 i = 0; i < context->swapchain.imageCount; i++) {
        context->imagesInFlight[i] = 0;
    }
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
