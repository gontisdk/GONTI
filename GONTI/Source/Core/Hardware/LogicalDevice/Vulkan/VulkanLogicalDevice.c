#include "VulkanLogicalDevice.h"

#ifdef USE_VULKAN
#ifdef RendererSubsystem_ON

#include "../../../MemorySubsystem/MemorySubsystem.h"
#include "../../../../Utilities/Logging/Scripts/Logger.h"

b8 vulkanLogicalDeviceCreate(gontiVulkanContext* context) {
    KINFO("Creating logical device...");

    VK_CHECK(vkCreateDevice(
        context->device.physicalDevice,
        &context->device.vkLdDevice.vkLdQueueSharedInfo.deviceCreateInfo,
        context->allocator,
        &context->device.logicalDevice
    ));

    return true;
}

void vulkanLogicalDeviceInitialize(gontiVulkanContext* context) {
    context->device.vkLdDevice.vkLdQueueSharedSetup.presentSharesGraphicsQueue = context->device.graphicsQueueIndex == context->device.presentQueueIndex;
    context->device.vkLdDevice.vkLdQueueSharedSetup.transferSharesGraphicsQueue = context->device.graphicsQueueIndex == context->device.transferQueueIndex;
    context->device.vkLdDevice.vkLdQueueSharedSetup.indexCount = 1;

    vulkanLogicalDeviceSharedSetup(context);
    vulkanLogicalDeviceQueueCreateInfos(context);
    vulkanLogicalDeviceConfigDriven(context);
    vulkanLogicalDeviceDeviceCreateInfo(context);
}
void vulkanLogicalDeviceSharedSetup(gontiVulkanContext* context) {
    if (!context->device.vkLdDevice.vkLdQueueSharedSetup.presentSharesGraphicsQueue) {
        context->device.vkLdDevice.vkLdQueueSharedSetup.indexCount++;
    }

    if (!context->device.vkLdDevice.vkLdQueueSharedSetup.transferSharesGraphicsQueue) {
        context->device.vkLdDevice.vkLdQueueSharedSetup.indexCount++;
    }

    context->device.vkLdDevice.vkLdQueueSharedSetup.indices = k_allocate(context->device.vkLdDevice.vkLdQueueSharedSetup.indexCount * sizeof(u32), MEMORY_TAG_RENDERER);
    context->device.vkLdDevice.vkLdQueueSharedSetup.index = 0;

    context->device.vkLdDevice.vkLdQueueSharedSetup.indices[context->device.vkLdDevice.vkLdQueueSharedSetup.index++] = context->device.graphicsQueueIndex;

    if (!context->device.vkLdDevice.vkLdQueueSharedSetup.presentSharesGraphicsQueue) {
        context->device.vkLdDevice.vkLdQueueSharedSetup.indices[context->device.vkLdDevice.vkLdQueueSharedSetup.index++] = context->device.presentQueueIndex;
    }

    if (!context->device.vkLdDevice.vkLdQueueSharedSetup.transferSharesGraphicsQueue) {
        context->device.vkLdDevice.vkLdQueueSharedSetup.indices[context->device.vkLdDevice.vkLdQueueSharedSetup.index++] = context->device.transferQueueIndex;
    }
}
void vulkanLogicalDeviceSharedFree(gontiVulkanContext* context) {
    if (context->device.vkLdDevice.vkLdQueueSharedSetup.indices) {
        k_free(context->device.vkLdDevice.vkLdQueueSharedSetup.indices, context->device.vkLdDevice.vkLdQueueSharedSetup.indexCount * sizeof(u32), MEMORY_TAG_RENDERER);

        context->device.vkLdDevice.vkLdQueueSharedSetup.indices = 0;
    }
}
void vulkanLogicalDeviceCommandPoolCreateInfo(gontiVulkanContext* context) {
    VkCommandPoolCreateInfo poolCreateInfo = {VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
    poolCreateInfo.queueFamilyIndex = context->device.graphicsQueueIndex;
    poolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    VK_CHECK(vkCreateCommandPool(
        context->device.logicalDevice,
        &poolCreateInfo,
        context->allocator,
        &context->device.graphicsCommandPool
    ));

    KINFO("Graphics command pool created.");
}
void vulkanLogicalDeviceQueueCreateInfos(gontiVulkanContext* context) {
    context->device.vkLdDevice.vkLdQueueSharedInfo.queueCreateInfos = k_allocate(context->device.vkLdDevice.vkLdQueueSharedSetup.indexCount * sizeof(VkDeviceQueueCreateInfo), MEMORY_TAG_RENDERER);
    context->device.vkLdDevice.vkLdQueueSharedSetup.queuePriority = k_allocate(context->device.vkLdDevice.vkLdQueueSharedSetup.indexCount * sizeof(f32), MEMORY_TAG_RENDERER);

    for (u32 i = 0; i < context->device.vkLdDevice.vkLdQueueSharedSetup.indexCount; i++) {
        context->device.vkLdDevice.vkLdQueueSharedInfo.queueCreateInfos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        context->device.vkLdDevice.vkLdQueueSharedInfo.queueCreateInfos[i].queueFamilyIndex = context->device.vkLdDevice.vkLdQueueSharedSetup.indices[i];
        context->device.vkLdDevice.vkLdQueueSharedInfo.queueCreateInfos[i].queueCount = 1;

        if (context->device.vkLdDevice.vkLdQueueSharedSetup.indices[i] == context->device.graphicsQueueIndex) {
            context->device.vkLdDevice.vkLdQueueSharedInfo.queueCreateInfos[i].queueCount = 2;
        }

        context->device.vkLdDevice.vkLdQueueSharedInfo.queueCreateInfos[i].flags = 0;
        context->device.vkLdDevice.vkLdQueueSharedInfo.queueCreateInfos[i].pNext = 0;
        
        context->device.vkLdDevice.vkLdQueueSharedSetup.queuePriority[i] = 1.0f;
        context->device.vkLdDevice.vkLdQueueSharedInfo.queueCreateInfos[i].pQueuePriorities = &context->device.vkLdDevice.vkLdQueueSharedSetup.queuePriority[i];
    }
}
void vulkanLogicalDeviceQueueFreeInfos(gontiVulkanContext* context) {
    if (context->device.vkLdDevice.vkLdQueueSharedInfo.queueCreateInfos) {
        k_free(context->device.vkLdDevice.vkLdQueueSharedInfo.queueCreateInfos, context->device.vkLdDevice.vkLdQueueSharedSetup.indexCount * sizeof(VkDeviceQueueCreateInfo), MEMORY_TAG_RENDERER);

        context->device.vkLdDevice.vkLdQueueSharedInfo.queueCreateInfos = 0;
    }

    if (context->device.vkLdDevice.vkLdQueueSharedSetup.queuePriority) {
        k_free(context->device.vkLdDevice.vkLdQueueSharedSetup.queuePriority, context->device.vkLdDevice.vkLdQueueSharedSetup.indexCount * sizeof(f32), MEMORY_TAG_RENDERER);

        context->device.vkLdDevice.vkLdQueueSharedSetup.queuePriority = 0;
    }
}
void vulkanLogicalDeviceConfigDriven(gontiVulkanContext* context) {
    // TODO: should be config driven
    context->device.features.samplerAnisotropy = VK_TRUE;
}
void vulkanLogicalDeviceDeviceCreateInfo(gontiVulkanContext* context) {
    context->device.vkLdDevice.vkLdQueueSharedInfo.deviceCreateInfo = (VkDeviceCreateInfo){VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};

    context->device.vkLdDevice.vkLdQueueSharedInfo.deviceCreateInfo.queueCreateInfoCount = context->device.vkLdDevice.vkLdQueueSharedSetup.indexCount;
    context->device.vkLdDevice.vkLdQueueSharedInfo.deviceCreateInfo.pQueueCreateInfos = context->device.vkLdDevice.vkLdQueueSharedInfo.queueCreateInfos;
    context->device.vkLdDevice.vkLdQueueSharedInfo.deviceCreateInfo.pEnabledFeatures = &context->device.vkLdDevice.vkLdQueueSharedInfo.deviceFeatures;
    context->device.vkLdDevice.vkLdQueueSharedInfo.deviceCreateInfo.enabledExtensionCount = 1;

    context->device.vkLdDevice.vkLdQueueSharedSetup.extensionNames = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
    context->device.vkLdDevice.vkLdQueueSharedInfo.deviceCreateInfo.ppEnabledExtensionNames = &context->device.vkLdDevice.vkLdQueueSharedSetup.extensionNames;

    context->device.vkLdDevice.vkLdQueueSharedInfo.deviceCreateInfo.enabledLayerCount = 0;
    context->device.vkLdDevice.vkLdQueueSharedInfo.deviceCreateInfo.ppEnabledLayerNames = 0;
}
void vulkanLogicalDeviceQueuesGet(gontiVulkanContext* context) {
    vkGetDeviceQueue(
        context->device.logicalDevice,
        context->device.graphicsQueueIndex,
        0,
        &context->device.graphicsQueue
    );

    vkGetDeviceQueue(
        context->device.logicalDevice,
        context->device.presentQueueIndex,
        0,
        &context->device.presentQueue
    );

    vkGetDeviceQueue(
        context->device.logicalDevice,
        context->device.transferQueueIndex,
        0,
        &context->device.transferQueue
    );

    vkGetDeviceQueue(
        context->device.logicalDevice,
        context->device.computeQueueIndex,
        0,
        &context->device.computeQueue
    );
}
void vulkanLogicalDeviceQueuesFree(gontiVulkanContext* context) {
    context->device.graphicsQueue = 0;
    context->device.presentQueue = 0;
    context->device.transferQueue = 0;
    context->device.computeQueue = 0;
}
void vulkanLogicalDeviceDestroy(gontiVulkanContext* context) {
    KINFO("Vulkan Destroying handle to logical device...");

    if (context->device.logicalDevice) {
        vkDestroyDevice(context->device.logicalDevice, context->allocator);
        context->device.logicalDevice = 0;
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

    vulkanLogicalDeviceQueuesFree(context);
    vulkanLogicalDeviceDestroy(context);
    vulkanLogicalDeviceQueueFreeInfos(context);
    vulkanLogicalDeviceSharedFree(context);

    context->device.vkLdDevice.vkLdQueueSharedSetup.index = 0;
    context->device.vkLdDevice.vkLdQueueSharedSetup.indexCount = 0;
}

#endif
#endif