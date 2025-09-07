#include "VulkanLogicalDevice.h"

#ifdef USE_VULKAN
#ifdef RendererSubsystem_ON

#include "../../../MemorySubsystem/MemorySubsystem.h"
#include "../../../../Utilities/Logging/Scripts/Logger.h"

b8 vulkanLogicalDeviceCreate(
    VkPhysicalDevice physicalDevice, 
    VkDeviceCreateInfo* createInfo, 
    VkAllocationCallbacks* allocator, 
    VkDevice* outLogicalDevice
) {
    KINFO("Creating logical device...");

    VK_CHECK(vkCreateDevice(
        physicalDevice,
        createInfo,
        allocator,
        outLogicalDevice
    ));

    return true;
}

void vulkanLogicalDeviceInitialize(gontiVulkanContext* context) {
    context->device.vkLdDevice.vkLdQueueSharedSetup.presentSharesGraphicsQueue = context->device.graphicsQueueIndex == context->device.presentQueueIndex;
    context->device.vkLdDevice.vkLdQueueSharedSetup.transferSharesGraphicsQueue = context->device.graphicsQueueIndex == context->device.transferQueueIndex;
    context->device.vkLdDevice.vkLdQueueSharedSetup.indexCount = 1;

    vulkanLogicalDeviceSharedSetup(
        context->device.vkLdDevice.vkLdQueueSharedSetup.presentSharesGraphicsQueue,
        context->device.vkLdDevice.vkLdQueueSharedSetup.transferSharesGraphicsQueue,
        &context->device.vkLdDevice.vkLdQueueSharedSetup.indices,
        &context->device.vkLdDevice.vkLdQueueSharedSetup.indexCount,
        &context->device.vkLdDevice.vkLdQueueSharedSetup.index,
        context->device.graphicsQueueIndex,
        context->device.presentQueueIndex,
        context->device.transferQueueIndex
    );
    vulkanLogicalDeviceQueueCreateInfos(
        &context->device.vkLdDevice.vkLdQueueSharedInfo.queueCreateInfos,
        &context->device.vkLdDevice.vkLdQueueSharedSetup.queuePriority,
        &context->device.vkLdDevice.vkLdQueueSharedSetup.indices,
        context->device.vkLdDevice.vkLdQueueSharedSetup.indexCount,
        context->device.graphicsQueueIndex
    );
    vulkanLogicalDeviceConfigDriven(
        &context->device.features
    );
    vulkanLogicalDeviceDeviceCreateInfo(
        &context->device.vkLdDevice.vkLdQueueSharedInfo.queueCreateInfos,
        &context->device.vkLdDevice.vkLdQueueSharedInfo.deviceCreateInfo,
        &context->device.vkLdDevice.vkLdQueueSharedInfo.deviceFeatures,
        context->device.vkLdDevice.vkLdQueueSharedSetup.indexCount,
        &context->device.vkLdDevice.vkLdQueueSharedSetup.extensionNames
    );
}
void vulkanLogicalDeviceSharedSetup(
    b8 presentSharesGraphicsQueue,
    b8 transferSharesGraphicsQueue,
    u32** indices,
    u32* indexCount,
    u32* index,
    i32 graphicsQueueIndex,
    i32 presentQueueIndex,
    i32 transferQueueIndex
) {
    if (!presentSharesGraphicsQueue) {
        (*indexCount)++;
    }

    if (!transferSharesGraphicsQueue) {
        (*indexCount)++;
    }

    *indices = k_allocate(*indexCount * sizeof(u32), MEMORY_TAG_RENDERER);
    *index = 0;

    (*indices)[(*index)++] = graphicsQueueIndex;

    if (!presentSharesGraphicsQueue) {
        (*indices)[(*index)++] = presentQueueIndex;
    }

    if (!transferSharesGraphicsQueue) {
        (*indices)[(*index)++] = transferQueueIndex;
    }
}
void vulkanLogicalDeviceSharedFree(u32** indices, u32 indexCount) {
    if ((*indices)) {
        k_free((*indices), indexCount * sizeof(u32), MEMORY_TAG_RENDERER);

        indices = 0;
    }
}
void vulkanLogicalDeviceCommandPoolCreateInfo(
    VkDevice logicalDevice,
    VkAllocationCallbacks* allocator,
    i32 graphicsQueueIndex,
    VkCommandPool* outCommandPool
) {
    VkCommandPoolCreateInfo poolCreateInfo = {VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
    poolCreateInfo.queueFamilyIndex = graphicsQueueIndex;
    poolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    VK_CHECK(vkCreateCommandPool(
        logicalDevice,
        &poolCreateInfo,
        allocator,
        outCommandPool
    ));

    KINFO("Graphics command pool created.");
}
void vulkanLogicalDeviceQueueCreateInfos(
    VkDeviceQueueCreateInfo** queueCreateInfos,
    f32** queuePriority,
    u32** indices,
    u32 indexCount,
    i32 graphicsQueueIndex
) {
    *queueCreateInfos = k_allocate(indexCount * sizeof(VkDeviceQueueCreateInfo), MEMORY_TAG_RENDERER);
    *queuePriority = k_allocate(indexCount * sizeof(f32), MEMORY_TAG_RENDERER);

    for (u32 i = 0; i < indexCount; i++) {
        (*queueCreateInfos)[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        (*queueCreateInfos)[i].queueFamilyIndex = (*indices)[i];
        (*queueCreateInfos)[i].queueCount = 1;

        if ((*indices)[i] == graphicsQueueIndex) {
            (*queueCreateInfos)[i].queueCount = 2;
        }

        (*queueCreateInfos)[i].flags = 0;
        (*queueCreateInfos)[i].pNext = 0;
        
        (*queuePriority)[i] = 1.0f;
        (*queueCreateInfos)[i].pQueuePriorities = &(*queuePriority)[i];
    }
}
void vulkanLogicalDeviceQueueFreeInfos(
    VkDeviceQueueCreateInfo** queueCreateInfos,
    f32** queuePriority,
    u32 indexCount
) {
    if (*queueCreateInfos) {
        k_free(*queueCreateInfos, indexCount * sizeof(VkDeviceQueueCreateInfo), MEMORY_TAG_RENDERER);

        *queueCreateInfos = 0;
    }

    if (*queuePriority) {
        k_free(*queuePriority, indexCount * sizeof(f32), MEMORY_TAG_RENDERER);

        *queuePriority = 0;
    }
}
void vulkanLogicalDeviceConfigDriven(
    VkPhysicalDeviceFeatures* features
    // TODO: add more 
) {
    // TODO: should be config driven
    (*features).samplerAnisotropy = VK_TRUE;
}
void vulkanLogicalDeviceDeviceCreateInfo(
    VkDeviceQueueCreateInfo** queueCreateInfos,
    VkDeviceCreateInfo* deviceCreateInfo,
    VkPhysicalDeviceFeatures* deviceFeatures,
    u32 indexCount,
    const char** extensionNames
) {
    *deviceCreateInfo = (VkDeviceCreateInfo){VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};

    (*deviceCreateInfo).queueCreateInfoCount = indexCount;
    (*deviceCreateInfo).pQueueCreateInfos = (*queueCreateInfos);
    (*deviceCreateInfo).pEnabledFeatures = &(*deviceFeatures);
    (*deviceCreateInfo).enabledExtensionCount = 1;

    *extensionNames = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
    (*deviceCreateInfo).ppEnabledExtensionNames = &(*extensionNames);

    (*deviceCreateInfo).enabledLayerCount = 0;
    (*deviceCreateInfo).ppEnabledLayerNames = 0;
}
void vulkanLogicalDeviceQueuesGet(
    VkDevice logicalDevice,
    VkQueue* graphicsQueue,
    i32 graphicsQueueIndex,
    VkQueue* presentQueue,
    i32 presentQueueIndex,
    VkQueue* transferQueue,
    i32 transferQueueIndex,
    VkQueue* computeQueue,
    i32 computeQueueIndex
) {
    vkGetDeviceQueue(
        logicalDevice,
        graphicsQueueIndex,
        0,
        graphicsQueue
    );

    vkGetDeviceQueue(
        logicalDevice,
        presentQueueIndex,
        0,
        presentQueue
    );

    vkGetDeviceQueue(
        logicalDevice,
        transferQueueIndex,
        0,
        transferQueue
    );

    vkGetDeviceQueue(
        logicalDevice,
        computeQueueIndex,
        0,
        computeQueue
    );
}
void vulkanLogicalDeviceQueuesFree(
    VkQueue* graphicsQueue,
    VkQueue* presentQueue,
    VkQueue* transferQueue,
    VkQueue* computeQueue
) {
    *graphicsQueue = 0;
    *presentQueue = 0;
    *transferQueue = 0;
    *computeQueue = 0;
}
void vulkanLogicalDeviceDestroy(VkDevice logicalDevice, VkAllocationCallbacks* allocator) {
    KINFO("Vulkan Destroying handle to logical device...");

    if (logicalDevice) {
        vkDestroyDevice(logicalDevice, allocator);
        logicalDevice = 0;
    }
}
void vulkanLogicalDeviceRelease(gontiVulkanContext* context) {
    KINFO("Release logical device resources...");

    KINFO("Destroying command pools...");
    vkDestroyCommandPool(
        context->device.logicalDevice,
        context->device.graphicsCommandPool,
        context->allocator
    );

    vulkanLogicalDeviceQueuesFree(
        &context->device.graphicsQueue,
        &context->device.presentQueue,
        &context->device.transferQueue,
        &context->device.computeQueue
    );
    vulkanLogicalDeviceDestroy(
        context->device.logicalDevice,
        context->allocator
    );
    vulkanLogicalDeviceQueueFreeInfos(
        &context->device.vkLdDevice.vkLdQueueSharedInfo.queueCreateInfos,
        &context->device.vkLdDevice.vkLdQueueSharedSetup.queuePriority,
        context->device.vkLdDevice.vkLdQueueSharedSetup.indexCount
    );
    vulkanLogicalDeviceSharedFree(
        &context->device.vkLdDevice.vkLdQueueSharedSetup.indices,
        context->device.vkLdDevice.vkLdQueueSharedSetup.indexCount
    );

    context->device.vkLdDevice.vkLdQueueSharedSetup.index = 0;
    context->device.vkLdDevice.vkLdQueueSharedSetup.indexCount = 0;
}

#endif
#endif