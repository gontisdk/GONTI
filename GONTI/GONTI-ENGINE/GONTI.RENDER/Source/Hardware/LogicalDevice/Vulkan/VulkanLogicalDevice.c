#include "VulkanLogicalDevice.h"

#if GONTI_USE_VULKAN

#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Memory/Memory.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Logging/Logger.h>

b8 gontiVkLogicalDeviceCreate(
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

void gontiVkLogicalDeviceInitialize(GontiVulkanContext* context) {
    context->device.vkLdDevice.vkLdQueueSharedSetup.presentSharesGraphicsQueue = context->device.graphicsQueueIndex == context->device.presentQueueIndex;
    context->device.vkLdDevice.vkLdQueueSharedSetup.transferSharesGraphicsQueue = context->device.graphicsQueueIndex == context->device.transferQueueIndex;
    context->device.vkLdDevice.vkLdQueueSharedSetup.indexCount = 1;

    gontiVkLogicalDeviceSharedSetup(
        &context->device,
        &context->device.vkLdDevice.vkLdQueueSharedSetup
    );
    gontiVkLogicalDeviceQueueCreateInfos(
        &context->device,
        &context->device.vkLdDevice.vkLdQueueSharedSetup,
        &context->device.vkLdDevice.vkLdQueueSharedInfo
    );
    gontiVkLogicalDeviceConfigDriven(
        &context->device
    );
    gontiVkLogicalDeviceDeviceCreateInfo(
        &context->device.vkLdDevice.vkLdQueueSharedSetup,
        &context->device.vkLdDevice.vkLdQueueSharedInfo
    );
}
void gontiVkLogicalDeviceSharedSetup(
    GontiVulkanDevice* gVkDevice,
    GontiVulkanLogicalDeviceQueueSharedSetup* gVkLdQueueSharedSetup
) {
    if (!gVkLdQueueSharedSetup->presentSharesGraphicsQueue) {
        gVkLdQueueSharedSetup->indexCount++;
    }

    if (!gVkLdQueueSharedSetup->transferSharesGraphicsQueue) {
        gVkLdQueueSharedSetup->indexCount++;
    }

    gVkLdQueueSharedSetup->indices = k_allocate(gVkLdQueueSharedSetup->indexCount * sizeof(u32), GONTI_MEMORY_TAG_RENDERER);
    gVkLdQueueSharedSetup->index = 0;

    gVkLdQueueSharedSetup->indices[gVkLdQueueSharedSetup->index++] = gVkDevice->graphicsQueueIndex;

    if (!gVkLdQueueSharedSetup->presentSharesGraphicsQueue) {
        gVkLdQueueSharedSetup->indices[gVkLdQueueSharedSetup->index++] = gVkDevice->presentQueueIndex;
    }

    if (!gVkLdQueueSharedSetup->transferSharesGraphicsQueue) {
        gVkLdQueueSharedSetup->indices[gVkLdQueueSharedSetup->index++] = gVkDevice->transferQueueIndex;
    }
}
void gontiVkLogicalDeviceSharedFree(GontiVulkanLogicalDeviceQueueSharedSetup* gVkLdQueueSharedSetup) {
    if (gVkLdQueueSharedSetup->indices) {
        k_free(gVkLdQueueSharedSetup->indices, gVkLdQueueSharedSetup->indexCount * sizeof(u32), GONTI_MEMORY_TAG_RENDERER);

        gVkLdQueueSharedSetup->indices = 0;
    }
}
void gontiVkLogicalDeviceCommandPoolCreate(
    GontiVulkanContext* context,
    GontiVulkanDevice* gVkDevice,
    VkCommandPool* outCommandPool
) {
    VkCommandPoolCreateInfo poolCreateInfo = {VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
    poolCreateInfo.queueFamilyIndex = gVkDevice->graphicsQueueIndex;
    poolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    VK_CHECK(vkCreateCommandPool(
        gVkDevice->logicalDevice,
        &poolCreateInfo,
        context->allocator,
        outCommandPool
    ));

    KINFO("Graphics command pool created.");
}
void gontiVkLogicalDeviceCommandPoolDestroy(
    GontiVulkanContext* context,
    GontiVulkanDevice* gVkDevice
) {
    KINFO("Destroying command pools...");

    vkDestroyCommandPool(
        gVkDevice->logicalDevice,
        gVkDevice->graphicsCommandPool,
        context->allocator
    );
}
void gontiVkLogicalDeviceQueueCreateInfos(
    GontiVulkanDevice* gVkDevice,
    GontiVulkanLogicalDeviceQueueSharedSetup* gVkLdQueueSharedSetup,
    GontiVulkanLogicalDeviceQueueSharedInfo* gVkLdQueueSharedInfo
) {
    gVkLdQueueSharedInfo->queueCreateInfos = k_allocate(gVkLdQueueSharedSetup->indexCount * sizeof(VkDeviceQueueCreateInfo), GONTI_MEMORY_TAG_RENDERER);
    gVkLdQueueSharedSetup->queuePriority = k_allocate(gVkLdQueueSharedSetup->indexCount * sizeof(f32), GONTI_MEMORY_TAG_RENDERER);

    for (u32 i = 0; i < gVkLdQueueSharedSetup->indexCount; i++) {
        gVkLdQueueSharedInfo->queueCreateInfos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        gVkLdQueueSharedInfo->queueCreateInfos[i].queueFamilyIndex = gVkLdQueueSharedSetup->indices[i];
        gVkLdQueueSharedInfo->queueCreateInfos[i].queueCount = 1;

        if (gVkLdQueueSharedSetup->indices[i] == gVkDevice->graphicsQueueIndex) {
            gVkLdQueueSharedInfo->queueCreateInfos[i].queueCount = 2;
        }

        gVkLdQueueSharedInfo->queueCreateInfos[i].flags = 0;
        gVkLdQueueSharedInfo->queueCreateInfos[i].pNext = 0;
        
        gVkLdQueueSharedSetup->queuePriority[i] = 1.0f;
        gVkLdQueueSharedInfo->queueCreateInfos[i].pQueuePriorities = &gVkLdQueueSharedSetup->queuePriority[i];
    }
}
void gontiVkLogicalDeviceQueueFreeInfos(
    GontiVulkanLogicalDeviceQueueSharedSetup* gVkLdQueueSharedSetup,
    GontiVulkanLogicalDeviceQueueSharedInfo* gVkLdQueueSharedInfo
) {
    if (gVkLdQueueSharedInfo->queueCreateInfos) {
        k_free(gVkLdQueueSharedInfo->queueCreateInfos, gVkLdQueueSharedSetup->indexCount * sizeof(VkDeviceQueueCreateInfo), GONTI_MEMORY_TAG_RENDERER);

        gVkLdQueueSharedInfo->queueCreateInfos = 0;
    }

    if (gVkLdQueueSharedSetup->queuePriority) {
        k_free(gVkLdQueueSharedSetup->queuePriority, gVkLdQueueSharedSetup->indexCount * sizeof(f32), GONTI_MEMORY_TAG_RENDERER);

        gVkLdQueueSharedSetup->queuePriority = 0;
    }
}
void gontiVkLogicalDeviceConfigDriven(
    GontiVulkanDevice* gVkDevice
) {
    // TODO: should be config driven
    gVkDevice->features.samplerAnisotropy = VK_TRUE;
}
void gontiVkLogicalDeviceDeviceCreateInfo(
    GontiVulkanLogicalDeviceQueueSharedSetup* gVkLdQueueSharedSetup,
    GontiVulkanLogicalDeviceQueueSharedInfo* gVkLdQueueSharedInfo
) {
    gVkLdQueueSharedInfo->deviceCreateInfo = (VkDeviceCreateInfo){VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};

    gVkLdQueueSharedInfo->deviceCreateInfo.queueCreateInfoCount = gVkLdQueueSharedSetup->indexCount;
    gVkLdQueueSharedInfo->deviceCreateInfo.pQueueCreateInfos = gVkLdQueueSharedInfo->queueCreateInfos;
    gVkLdQueueSharedInfo->deviceCreateInfo.pEnabledFeatures = &gVkLdQueueSharedInfo->deviceFeatures;
    gVkLdQueueSharedInfo->deviceCreateInfo.enabledExtensionCount = 1;

    gVkLdQueueSharedSetup->extensionNames = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
    gVkLdQueueSharedInfo->deviceCreateInfo.ppEnabledExtensionNames = &gVkLdQueueSharedSetup->extensionNames;

    gVkLdQueueSharedInfo->deviceCreateInfo.enabledLayerCount = 0;
    gVkLdQueueSharedInfo->deviceCreateInfo.ppEnabledLayerNames = 0;
}
void gontiVkLogicalDeviceQueuesGet(
    GontiVulkanDevice* gVkDevice
) {
    vkGetDeviceQueue(
        gVkDevice->logicalDevice,
        gVkDevice->graphicsQueueIndex,
        0,
        &gVkDevice->graphicsQueue
    );

    vkGetDeviceQueue(
        gVkDevice->logicalDevice,
        gVkDevice->presentQueueIndex,
        0,
        &gVkDevice->presentQueue
    );

    vkGetDeviceQueue(
        gVkDevice->logicalDevice,
        gVkDevice->transferQueueIndex,
        0,
        &gVkDevice->transferQueue
    );

    vkGetDeviceQueue(
        gVkDevice->logicalDevice,
        gVkDevice->computeQueueIndex,
        0,
        &gVkDevice->computeQueue
    );
}
void gontiVkLogicalDeviceQueuesFree(
    GontiVulkanDevice* gVkDevice
) {
    gVkDevice->graphicsQueue = 0;
    gVkDevice->presentQueue = 0;
    gVkDevice->transferQueue = 0;
    gVkDevice->computeQueue = 0;
}
void gontiVkLogicalDeviceDestroy(VkDevice logicalDevice, VkAllocationCallbacks* allocator) {
    KINFO("Vulkan Destroying handle to logical device...");

    if (logicalDevice) {
        vkDestroyDevice(logicalDevice, allocator);
        logicalDevice = 0;
    }
}
void gontiVkLogicalDeviceRelease(GontiVulkanContext* context) {
    KINFO("Release logical device resources...");

    gontiVkLogicalDeviceCommandPoolDestroy(
        context,
        &context->device
    );

    gontiVkLogicalDeviceQueuesFree(
        &context->device
    );
    gontiVkLogicalDeviceDestroy(
        context->device.logicalDevice,
        context->allocator
    );
    gontiVkLogicalDeviceQueueFreeInfos(
        &context->device.vkLdDevice.vkLdQueueSharedSetup,
        &context->device.vkLdDevice.vkLdQueueSharedInfo
    );
    gontiVkLogicalDeviceSharedFree(
        &context->device.vkLdDevice.vkLdQueueSharedSetup
    );

    context->device.vkLdDevice.vkLdQueueSharedSetup.index = 0;
    context->device.vkLdDevice.vkLdQueueSharedSetup.indexCount = 0;
}

#endif