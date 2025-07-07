#include "VulkanSyncObjects.h"

#ifdef USE_VULKAN
#ifdef SyncObjects_ON

#include "../../../../Containers/DynamicArray/DynamicArray.h"
#include "../../Fence/Vulkan/VulkanFence.h"

void vulkanSyncObjectsCreate(gontiVulkanContext* context) {
    context->imageAvailableSemaphores = darrayReserve(VkSemaphore, context->swapchain.maxFramesInFlight);
    context->queueCompleteSemaphore = darrayReserve(VkSemaphore, context->swapchain.maxFramesInFlight);
    context->inFlightFences = darrayReserve(gontiVulkanFence, context->swapchain.maxFramesInFlight);

    for (u8 i = 0; i < context->swapchain.maxFramesInFlight; i++) {
        VkSemaphoreCreateInfo semaphoreCreateInfo = {VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
        
        vkCreateSemaphore(context->device.logicalDevice, &semaphoreCreateInfo, context->allocator, &context->imageAvailableSemaphores[i]);
        vkCreateSemaphore(context->device.logicalDevice, &semaphoreCreateInfo, context->allocator, &context->queueCompleteSemaphore[i]);

        vulkanFenceCreate(context, true, &context->inFlightFences[i]);
    }

    context->imagesInFlight = darrayReserve(gontiVulkanFence, context->swapchain.imageCount);

    for (u32 i = 0; i < context->swapchain.imageCount; i++) {
        context->imagesInFlight[i] = 0;
    }
}
void vulkanSyncObjectsDestroy(gontiVulkanContext* context) {
    vkDeviceWaitIdle(context->device.logicalDevice);

    for (u8 i = 0; i < context->swapchain.maxFramesInFlight; i++) {
        if (context->imageAvailableSemaphores[i]) {
            vkDestroySemaphore(
                context->device.logicalDevice,
                context->imageAvailableSemaphores[i],
                context->allocator
            );
            context->imageAvailableSemaphores[i] = 0;
        }
        if (context->queueCompleteSemaphore[i]) {
            vkDestroySemaphore(
                context->device.logicalDevice,
                context->queueCompleteSemaphore[i],
                context->allocator
            );
            context->queueCompleteSemaphore[i] = 0;
        }

        vulkanFenceDestroy(context, &context->inFlightFences[i]);
    }

    darrayDestroy(context->imageAvailableSemaphores);
    context->imageAvailableSemaphores = 0;

    darrayDestroy(context->queueCompleteSemaphore);
    context->queueCompleteSemaphore = 0;

    darrayDestroy(context->inFlightFences);
    context->inFlightFences = 0;

    darrayDestroy(context->imagesInFlight);
    context->imagesInFlight = 0;
}

#endif
#endif