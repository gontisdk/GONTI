#include "VulkanPhysicalDevice.h"

#ifdef USE_VULKAN
#ifdef RendererSubsystem_ON

#include "../../../../Utilities/Logging/Scripts/Logger.h"
#include "../../../../Libs/StringLib/StringLib.h"
#include "../../../../Core/MemorySubsystem/MemorySubsystem.h"
#include "../../../../Containers/DynamicArray/DynamicArray.h"
#include "../../Device/Vulkan/VulkanDevice.h"

b8 vulkanSelectPhysicalDevice(gontiVulkanContext* context) {
    if(!vulkanFindPhysicalDevices(context)) return false;
    vulkanNumeratePhysicalDevices(context);

    return true;
}
b8 vulkanFindPhysicalDevices(gontiVulkanContext* context) {
    context->device.vkDevices = ZERO_DEVICES;

    VK_CHECK(vkEnumeratePhysicalDevices(context->instance, &context->device.vkDevices.count, 0));

    if (context->device.vkDevices.count == 0) {
        KFATAL("No devices witch support Vulkan were found.");
        return false;
    }

    //context->vkDevices.devices[context->vkDevices.count];
    context->device.vkDevices.devices = k_allocate(context->device.vkDevices.count * sizeof(VkPhysicalDevice), MEMORY_TAG_RENDERER);
    VK_CHECK(vkEnumeratePhysicalDevices(context->instance, &context->device.vkDevices.count, context->device.vkDevices.devices));

    return true;
}
b8 vulkanPhysicalDeviceMeetsRequirements(
    VkPhysicalDevice device,
    VkSurfaceKHR surface,
    const VkPhysicalDeviceProperties* properties,
    const VkPhysicalDeviceFeatures* features,
    const VulkanPhysicalDeviceRequirements* requirements,
    VulkanPhysicalDeviceQueueFamilyInfo* outQueueInfo,
    gontiVulkanSwapchainSupportInfo* outSwapchainSupport
) {
    outQueueInfo->graphicsFamilyIndex = -1;
    outQueueInfo->presentFamilyIndex = -1;
    outQueueInfo->computeFamilyIndex = -1;
    outQueueInfo->transferFamilyIndex = -1;

    if (requirements->discreteGpu) {
        if (properties->deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            KINFO("Device is not a discrete GPU, and one is required. Skipping...");
            return false;
        }
    }

    u32 queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, 0);

    VkQueueFamilyProperties queueFamilies[queueFamilyCount];
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

    KINFO(" Graphics | Present  | Compute | Transfer | Name");
    u8 minTransferScore = 255;

    for (u32 i = 0; i < queueFamilyCount; i++) {
        u8 currentTransferScore = 0;

        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            outQueueInfo->graphicsFamilyIndex = i;
            currentTransferScore++;
        }

        if (queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
            outQueueInfo->computeFamilyIndex = i;
            currentTransferScore++;
        }

        if (queueFamilies[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
            if (currentTransferScore <= minTransferScore) {
                minTransferScore = currentTransferScore;
                outQueueInfo->transferFamilyIndex = i;
            }
        }

        VkBool32 supportsPresent = VK_FALSE;
        VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &supportsPresent));

        if (supportsPresent) {
            outQueueInfo->presentFamilyIndex = i;
        }
    }

    KINFO("        %d |        %d |       %d |        %d | %s",
        outQueueInfo->graphicsFamilyIndex != -1,
        outQueueInfo->presentFamilyIndex != -1,
        outQueueInfo->computeFamilyIndex != -1,
        outQueueInfo->transferFamilyIndex != -1,
        properties->deviceName    
    );

    if (
        (!requirements->graphics || (requirements->graphics && outQueueInfo->graphicsFamilyIndex != -1)) &&
        (!requirements->present || (requirements->present && outQueueInfo->presentFamilyIndex != -1)) &&
        (!requirements->compute || (requirements->compute && outQueueInfo->computeFamilyIndex != -1)) &&
        (!requirements->transfer || (requirements->transfer && outQueueInfo->transferFamilyIndex != -1))
    ) {
        KINFO("Device meets queue requirements.");
        KTRACE("Graphics Family Index: %i", outQueueInfo->graphicsFamilyIndex);
        KTRACE("Present Family Index: %i", outQueueInfo->presentFamilyIndex);
        KTRACE("Transfer Family Index: %i", outQueueInfo->transferFamilyIndex);
        KTRACE("Compute Family Index: %i", outQueueInfo->computeFamilyIndex);
        
        vulkanDeviceQuerySwapchainSupport(device, surface, outSwapchainSupport);

        if (outSwapchainSupport->formatCount < 1 || outSwapchainSupport->presentModeCount < 1) {
            if (outSwapchainSupport->formats) {
                k_free(outSwapchainSupport->formats, outSwapchainSupport->formatCount * sizeof(VkSurfaceFormatKHR), MEMORY_TAG_RENDERER);
            }

            if (outSwapchainSupport->presentMode) {
                k_free(outSwapchainSupport->presentMode, outSwapchainSupport->presentModeCount * sizeof(VkPresentModeKHR), MEMORY_TAG_RENDERER);
            }

            KINFO("Required swapchain support not present. Skipping device...");
            return false;
        }

        if (requirements->deviceExtensionNames) {
            u32 avanibleExtensionCount = 0;
            VkExtensionProperties* avanibleExtensions = 0;

            VK_CHECK(vkEnumerateDeviceExtensionProperties(device, 0, &avanibleExtensionCount, 0));

            if (avanibleExtensionCount != 0) {
                avanibleExtensions = k_allocate(avanibleExtensionCount * sizeof(VkExtensionProperties), MEMORY_TAG_RENDERER);
                VK_CHECK(vkEnumerateDeviceExtensionProperties(device, 0, &avanibleExtensionCount, avanibleExtensions));

                u32 requiredExtensionCount = darrayLength(requirements->deviceExtensionNames);
                
                for (u32 i = 0; i < requiredExtensionCount; i++) {
                    b8 found = false;

                    for (u32 j = 0; j < avanibleExtensionCount; j++) {
                        if (stringsEqual(requirements->deviceExtensionNames[i], avanibleExtensions[j].extensionName)) {
                            found = true;
                            break;
                        }
                    }

                    if (!found) {
                        KINFO("Required extension not found: %s. Skipping device...", requirements->deviceExtensionNames[i]);
                        k_free(avanibleExtensions, avanibleExtensionCount * sizeof(VkExtensionProperties), MEMORY_TAG_RENDERER);
                        return false;
                    }
                }
            }

            k_free(avanibleExtensions, avanibleExtensionCount * sizeof(VkExtensionProperties), MEMORY_TAG_RENDERER);
        }

        if (requirements->samplerAnisotropy && !features->samplerAnisotropy) {
            KINFO("Device not support samplerAnisotropy. Skipping...");
            return false;
        }

        return true;
    }

    return false;
}
b8 vulkanNumeratePhysicalDevices(gontiVulkanContext* context) {
    context->device.vkDeviceInfo = k_allocate(context->device.vkDevices.count * sizeof(gontiVulkanDeviceInfo), MEMORY_TAG_RENDERER);

    for (u32 i = 0; i < context->device.vkDevices.count; i++) {
        vkGetPhysicalDeviceProperties(context->device.vkDevices.devices[i], &context->device.vkDeviceInfo[i].properties);
        vkGetPhysicalDeviceFeatures(context->device.vkDevices.devices[i], &context->device.vkDeviceInfo[i].features);
        vkGetPhysicalDeviceMemoryProperties(context->device.vkDevices.devices[i], &context->device.vkDeviceInfo[i].memory);

        VulkanPhysicalDeviceRequirements requirements = {};
        requirements.graphics = true;
        requirements.present = true;
        requirements.transfer = true;
        //requirements.compute = true; // NOTE: Enable this if compute will be required
            
        requirements.samplerAnisotropy = true;
        requirements.discreteGpu = true;
            
        requirements.deviceExtensionNames = darrayCreate(const char*);
        darrayPush(requirements.deviceExtensionNames, &VK_KHR_SWAPCHAIN_EXTENSION_NAME);
            
        VulkanPhysicalDeviceQueueFamilyInfo queueInfo = {};

        b8 result = vulkanPhysicalDeviceMeetsRequirements(
            context->device.vkDevices.devices[i],
            context->surface,
            &context->device.vkDeviceInfo[i].properties,
            &context->device.vkDeviceInfo[i].features,
            &requirements,
            &queueInfo,
            &context->device.swapchainSupport
        );

        if (result) {
            KINFO("Selected device: '%s'.", context->device.vkDeviceInfo[i].properties.deviceName);

            switch (context->device.vkDeviceInfo[i].properties.deviceType) {
                default:
                case VK_PHYSICAL_DEVICE_TYPE_OTHER:
                    KINFO("GPU type is unknown");
                    break;
                case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                    KINFO("GPU type is integrated");
                    break;
                case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                    KINFO("GPU type is discrete");
                    break;
                case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                    KINFO("GPU type is virtual");
                    break;
                case VK_PHYSICAL_DEVICE_TYPE_CPU:
                    KINFO("GPU type is CPU");
                    break;
            }

            KINFO("GPU Driver version %d.%d.%d",
                VK_VERSION_MAJOR(context->device.vkDeviceInfo[i].properties.driverVersion),
                VK_VERSION_MAJOR(context->device.vkDeviceInfo[i].properties.driverVersion),
                VK_VERSION_PATCH(context->device.vkDeviceInfo[i].properties.driverVersion)
            );

            KINFO("Vulkan API version %d.%d.%d",
                VK_VERSION_MAJOR(context->device.vkDeviceInfo[i].properties.apiVersion),
                VK_VERSION_MAJOR(context->device.vkDeviceInfo[i].properties.apiVersion),
                VK_VERSION_PATCH(context->device.vkDeviceInfo[i].properties.apiVersion)
            );

            for (u32 j = 0; j < context->device.vkDeviceInfo[i].memory.memoryHeapCount; j++) {
                f32 memorySizeGib = (((f32)context->device.vkDeviceInfo[i].memory.memoryHeaps[j].size) / 1024.0f / 1024.0f / 1024.0f);

                if (context->device.vkDeviceInfo[i].memory.memoryHeaps[j].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
                    KINFO("Local GPU memory: %.2f GiB", memorySizeGib);
                } else {
                    KINFO("Shared system memory: %.2f GiB", memorySizeGib);
                }
            }

            context->device.physicalDevice = context->device.vkDevices.devices[i];
            context->device.graphicsQueueIndex = queueInfo.graphicsFamilyIndex;
            context->device.presentQueueIndex = queueInfo.presentFamilyIndex;
            context->device.transferQueueIndex = queueInfo.transferFamilyIndex;
            //context->device.computeQueueIndex = queueInfo.computeFamilyIndex; // NOTE: Uncoment if you needed

            context->device.properties = context->device.vkDeviceInfo->properties;
            context->device.features = context->device.vkDeviceInfo->features;
            context->device.memory = context->device.vkDeviceInfo->memory;

            break;
        }
    }

    if (!context->device.physicalDevice) {
        KERROR("No physical devices were found witch meet the requirements.");
        return false;
    }

    KINFO("Physical device selected");
    return true;
}

void vulkanPhysicalDeviceRelease(struct gontiVulkanContext* context) {
    KINFO("Release physical device resources...");
    
    if (context->device.vkDevices.devices) {
        k_free(context->device.vkDevices.devices, context->device.vkDevices.count * sizeof(VkPhysicalDevice), MEMORY_TAG_RENDERER);

        context->device.vkDevices = ZERO_DEVICES;
        context->device.vkDevices.devices = 0;
        context->device.vkDevices.count = 0;
    }

    if (context->device.vkDeviceInfo) {
        k_free(context->device.vkDeviceInfo, context->device.vkDevices.count * sizeof(gontiVulkanDeviceInfo), MEMORY_TAG_RENDERER);

        context->device.vkDeviceInfo = 0;
    }

    k_zeroMemory(&context->device.swapchainSupport.capabilities, sizeof(context->device.swapchainSupport.capabilities));

    context->device.graphicsQueueIndex = -1;
    context->device.presentQueueIndex = -1;
    context->device.computeQueueIndex = -1;
    context->device.transferQueueIndex = -1;
    
    vkDestroySurfaceKHR(context->instance, context->surface, context->allocator);
    context->surface = 0;

    context->device.physicalDevice = 0;
    context->allocator = 0;
    
    // TODO: Release it

    //context->device.properties =
    //context->device.features =
    //context->device.memory =
    
    //context->device.vkDeviceInfo->properties =
    //context->device.vkDeviceInfo->features =
    //context->device.vkDeviceInfo->memory =
}

#endif
#endif