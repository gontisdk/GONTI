#include <Hardware/LogicalDevice/GtVkLDevice.h>

#include <GONTI-CORE/GONTI.CORE/Source/Memory/GtMemory.h>
#include <GONTI-CORE/GONTI.CORE/Source/Logging/GtLogger.h>
#include <GONTI-RENDER/GONTI-RENDER-VK/GONTI.RENDER.UTILITIES.VK/Source/GtVkUtil.h>

GtB8 gontiVkLogicalDeviceCreate(
    VkPhysicalDevice physicalDevice, 
    VkDeviceCreateInfo* createInfo, 
    VkAllocationCallbacks* allocator, 
    VkDevice* outLogicalDevice
) {
    GTINFO("Creating logical device...");

    GTVK_CHECK(vkCreateDevice(
        physicalDevice,
        createInfo,
        allocator,
        outLogicalDevice
    ));

    return GtTrue;
}

void gontiVkLogicalDeviceInitialize(GtVkContext* context) {
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
    GtVkDevice* gVkDevice,
    GtVkLDeviceQueueSharedSetup* gVkLdQueueSharedSetup
) {
    if (!gVkLdQueueSharedSetup->presentSharesGraphicsQueue) {
        gVkLdQueueSharedSetup->indexCount++;
    }

    if (!gVkLdQueueSharedSetup->transferSharesGraphicsQueue) {
        gVkLdQueueSharedSetup->indexCount++;
    }

    gVkLdQueueSharedSetup->indices = gt_allocate(gVkLdQueueSharedSetup->indexCount * sizeof(GtU32), GT_MEM_TAG_RENDERER);
    gVkLdQueueSharedSetup->index = 0;

    gVkLdQueueSharedSetup->indices[gVkLdQueueSharedSetup->index++] = gVkDevice->graphicsQueueIndex;

    if (!gVkLdQueueSharedSetup->presentSharesGraphicsQueue) {
        gVkLdQueueSharedSetup->indices[gVkLdQueueSharedSetup->index++] = gVkDevice->presentQueueIndex;
    }

    if (!gVkLdQueueSharedSetup->transferSharesGraphicsQueue) {
        gVkLdQueueSharedSetup->indices[gVkLdQueueSharedSetup->index++] = gVkDevice->transferQueueIndex;
    }
}
void gontiVkLogicalDeviceSharedFree(GtVkLDeviceQueueSharedSetup* gVkLdQueueSharedSetup) {
    if (gVkLdQueueSharedSetup->indices) {
        gt_free(gVkLdQueueSharedSetup->indices);
        gVkLdQueueSharedSetup->indices = 0;
    }
}
void gontiVkLogicalDeviceCommandPoolCreate(
    GtVkContext* context,
    GtVkDevice* gVkDevice,
    VkCommandPool* outCommandPool
) {
    VkCommandPoolCreateInfo poolCreateInfo = {VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
    poolCreateInfo.queueFamilyIndex = gVkDevice->graphicsQueueIndex;
    poolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    GTVK_CHECK(vkCreateCommandPool(
        gVkDevice->logicalDevice,
        &poolCreateInfo,
        context->allocator,
        outCommandPool
    ));

    GTINFO("Graphics command pool created.");
}
void gontiVkLogicalDeviceCommandPoolDestroy(
    GtVkContext* context,
    GtVkDevice* gVkDevice
) {
    GTINFO("Destroying command pools...");

    vkDestroyCommandPool(
        gVkDevice->logicalDevice,
        gVkDevice->graphicsCommandPool,
        context->allocator
    );
}
void gontiVkLogicalDeviceQueueCreateInfos(
    GtVkDevice* gVkDevice,
    GtVkLDeviceQueueSharedSetup* gVkLdQueueSharedSetup,
    GtVkLDeviceQueueSharedInfo* gVkLdQueueSharedInfo
) {
    gVkLdQueueSharedInfo->queueCreateInfos = gt_allocate(gVkLdQueueSharedSetup->indexCount * sizeof(VkDeviceQueueCreateInfo), GT_MEM_TAG_RENDERER);
    
    GtU32 maxQueuesPerFamily = 2; 
    gVkLdQueueSharedSetup->queuePriority = gt_allocate(gVkLdQueueSharedSetup->indexCount * maxQueuesPerFamily * sizeof(GtF32), GT_MEM_TAG_RENDERER);

    for (GtU32 i = 0; i < gVkLdQueueSharedSetup->indexCount; i++) {
        gVkLdQueueSharedInfo->queueCreateInfos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        gVkLdQueueSharedInfo->queueCreateInfos[i].queueFamilyIndex = gVkLdQueueSharedSetup->indices[i];
        
        gVkLdQueueSharedInfo->queueCreateInfos[i].queueCount = 1;

        if (gVkLdQueueSharedSetup->indices[i] == gVkDevice->graphicsQueueIndex) {
            gVkLdQueueSharedInfo->queueCreateInfos[i].queueCount = 2;
        }

        gVkLdQueueSharedInfo->queueCreateInfos[i].flags = 0;
        gVkLdQueueSharedInfo->queueCreateInfos[i].pNext = 0;
        
        GtF32* currentPriorities = &gVkLdQueueSharedSetup->queuePriority[i * maxQueuesPerFamily];
        
        currentPriorities[0] = 1.0f;
        if (gVkLdQueueSharedInfo->queueCreateInfos[i].queueCount > 1) {
            currentPriorities[1] = 1.0f;
        }

        gVkLdQueueSharedInfo->queueCreateInfos[i].pQueuePriorities = currentPriorities;
    }
}
void gontiVkLogicalDeviceQueueFreeInfos(
    GtVkLDeviceQueueSharedSetup* gVkLdQueueSharedSetup,
    GtVkLDeviceQueueSharedInfo* gVkLdQueueSharedInfo
) {
    if (gVkLdQueueSharedInfo->queueCreateInfos) {
        gt_free(gVkLdQueueSharedInfo->queueCreateInfos);
        gVkLdQueueSharedInfo->queueCreateInfos = 0;
    }

    if (gVkLdQueueSharedSetup->queuePriority) {
        gt_free(gVkLdQueueSharedSetup->queuePriority);
        gVkLdQueueSharedSetup->queuePriority = 0;
    }
}
void gontiVkLogicalDeviceConfigDriven(
    GtVkDevice* gVkDevice
) {
    // TODO: should be config driven
    gVkDevice->features.samplerAnisotropy = VK_TRUE;
}
void gontiVkLogicalDeviceDeviceCreateInfo(
    GtVkLDeviceQueueSharedSetup* gVkLdQueueSharedSetup,
    GtVkLDeviceQueueSharedInfo* gVkLdQueueSharedInfo
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
    GtVkDevice* gVkDevice
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
    GtVkDevice* gVkDevice
) {
    gVkDevice->graphicsQueue = 0;
    gVkDevice->presentQueue = 0;
    gVkDevice->transferQueue = 0;
    gVkDevice->computeQueue = 0;
}
void gontiVkLogicalDeviceDestroy(VkDevice logicalDevice, VkAllocationCallbacks* allocator) {
    GTINFO("Vulkan Destroying handle to logical device...");

    if (logicalDevice) {
        vkDestroyDevice(logicalDevice, allocator);
        logicalDevice = 0;
    }
}
void gontiVkLogicalDeviceRelease(GtVkContext* context) {
    GTINFO("Release logical device resources...");

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
