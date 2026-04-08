#include "GtVkPDevice.h"

#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Logging/GtLogger.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/CStringTools/GtCStrTools.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Memory/GtMemory.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Containers/DynamicArray/GtDArray.h>

#include "../Device/GtVkDevice.h"

const GtVkPDevicesList ZERO_PHYSICAL_DEVICES = { VK_NULL_HANDLE, 0 };

GtB8 gontiVkSelectPhysicalDevice(GtVkContext* context) {
    if (!gontiVkFindPhysicalDevices(context)) return GtFalse;
    gontiVkNumeratePhysicalDevices(context);

    return GtTrue;
}
GtB8 gontiVkFindPhysicalDevices(GtVkContext* context) {
    context->device.vkDevices = ZERO_PHYSICAL_DEVICES;

    GTVK_CHECK(vkEnumeratePhysicalDevices(context->instance, &context->device.vkDevices.count, 0));

    if (context->device.vkDevices.count == 0) {
        GTFATAL("No devices witch support Vulkan were found.");
        return GtFalse;
    }

    context->device.vkDevices.devices = gt_allocate(context->device.vkDevices.count * sizeof(VkPhysicalDevice), GT_MEM_TAG_RENDERER);
    GTVK_CHECK(vkEnumeratePhysicalDevices(context->instance, &context->device.vkDevices.count, context->device.vkDevices.devices));

    return GtTrue;
}
GtB8 gontiVkPhysicalDeviceMeetsRequirements(
    VkPhysicalDevice device,
    VkSurfaceKHR surface,
    const VkPhysicalDeviceProperties* properties,
    const VkPhysicalDeviceFeatures* features,
    const GtVkPDeviceRequirements* requirements,
    GtVkPDeviceQueueFamilyInfo* outQueueInfo,
    GtVkSwapchainInfo* outSwapchainSupport
) {
    outQueueInfo->graphicsFamilyIndex = -1;
    outQueueInfo->presentFamilyIndex = -1;
    outQueueInfo->computeFamilyIndex = -1;
    outQueueInfo->transferFamilyIndex = -1;

    if (requirements->discreteGpu) {
        if (properties->deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            GTINFO("Device is not a discrete GPU, and one is required. Skipping...");
            return GtFalse;
        }
    }

    GtU32 queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, 0);

    VkQueueFamilyProperties queueFamilies[queueFamilyCount];
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

    GTINFO(" Graphics | Present  | Compute | Transfer | Name");
    GtU8 minTransferScore = 255;

    for (GtU32 i = 0; i < queueFamilyCount; i++) {
        GtU8 currentTransferScore = 0;

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
        GTVK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &supportsPresent));

        if (supportsPresent) {
            outQueueInfo->presentFamilyIndex = i;
        }
    }

    GTINFO("        %d |        %d |       %d |        %d | %s",
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
        GTINFO("Device meets queue requirements.");
        GTTRACE("Graphics Family Index: %i", outQueueInfo->graphicsFamilyIndex);
        GTTRACE("Present Family Index: %i", outQueueInfo->presentFamilyIndex);
        GTTRACE("Transfer Family Index: %i", outQueueInfo->transferFamilyIndex);
        GTTRACE("Compute Family Index: %i", outQueueInfo->computeFamilyIndex);
        
        gontiVkDeviceQuerySwapchainSupport(device, surface, outSwapchainSupport);

        if (outSwapchainSupport->formatCount < 1 || outSwapchainSupport->presentModeCount < 1) {
            if (outSwapchainSupport->formats) {
                gt_free(outSwapchainSupport->formats);
            }

            if (outSwapchainSupport->presentMode) {
                gt_free(outSwapchainSupport->presentMode);
            }

            GTINFO("Required swapchain support not present. Skipping device...");
            return GtFalse;
        }

        if (requirements->deviceExtensionNames) {
            GtU32 avanibleExtensionCount = 0;
            VkExtensionProperties* avanibleExtensions = 0;

            GTVK_CHECK(vkEnumerateDeviceExtensionProperties(device, 0, &avanibleExtensionCount, 0));

            if (avanibleExtensionCount != 0) {
                avanibleExtensions = gt_allocate(avanibleExtensionCount * sizeof(VkExtensionProperties), GT_MEM_TAG_RENDERER);
                GTVK_CHECK(vkEnumerateDeviceExtensionProperties(device, 0, &avanibleExtensionCount, avanibleExtensions));

                GtU32 requiredExtensionCount = gontiDarrayLength(requirements->deviceExtensionNames);
                
                for (GtU32 i = 0; i < requiredExtensionCount; i++) {
                    GtB8 found = GtFalse;

                    for (GtU32 j = 0; j < avanibleExtensionCount; j++) {
                        if (gontiStringEqual(requirements->deviceExtensionNames[i], avanibleExtensions[j].extensionName)) {
                            found = GtTrue;
                            break;
                        }
                    }

                    if (!found) {
                        GTINFO("Required extension not found: %s. Skipping device...", requirements->deviceExtensionNames[i]);
                        gt_free(avanibleExtensions);
                        return GtFalse;
                    }
                }
            }

            gt_free(avanibleExtensions);
        }

        if (requirements->samplerAnisotropy && !features->samplerAnisotropy) {
            GTINFO("Device not support samplerAnisotropy. Skipping...");
            return GtFalse;
        }

        return GtTrue;
    }

    return GtFalse;
}
GtB8 gontiVkNumeratePhysicalDevices(GtVkContext* context) {
    context->device.vkDeviceInfo = gt_allocate(context->device.vkDevices.count * sizeof(GtVkDeviceInfo), GT_MEM_TAG_RENDERER);

    for (GtU32 i = 0; i < context->device.vkDevices.count; i++) {
        vkGetPhysicalDeviceProperties(context->device.vkDevices.devices[i], &context->device.vkDeviceInfo[i].properties);
        vkGetPhysicalDeviceFeatures(context->device.vkDevices.devices[i], &context->device.vkDeviceInfo[i].features);
        vkGetPhysicalDeviceMemoryProperties(context->device.vkDevices.devices[i], &context->device.vkDeviceInfo[i].memory);

        GtVkPDeviceRequirements requirements = {};
        requirements.graphics = GtTrue;
        requirements.present = GtTrue;
        requirements.transfer = GtTrue;
        requirements.compute = GtTrue; // NOTE: Enable this if compute will be required
            
        requirements.samplerAnisotropy = GtTrue;
        requirements.discreteGpu = GtTrue;
            
        requirements.deviceExtensionNames = gontiDarrayCreate(const char*);
        gontiDarrayPush(requirements.deviceExtensionNames, &VK_KHR_SWAPCHAIN_EXTENSION_NAME);
            
        GtVkPDeviceQueueFamilyInfo queueInfo = {};

        GtB8 result = gontiVkPhysicalDeviceMeetsRequirements(
            context->device.vkDevices.devices[i],
            context->surface,
            &context->device.vkDeviceInfo[i].properties,
            &context->device.vkDeviceInfo[i].features,
            &requirements,
            &queueInfo,
            &context->device.swapchainSupport
        );

        gontiDarrayDestroy(requirements.deviceExtensionNames);

        if (result) {
            GTINFO("Selected device: '%s'.", context->device.vkDeviceInfo[i].properties.deviceName);

            switch (context->device.vkDeviceInfo[i].properties.deviceType) {
                default:
                case VK_PHYSICAL_DEVICE_TYPE_OTHER:
                    GTINFO("GPU type is unknown");
                    break;
                case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                    GTINFO("GPU type is integrated");
                    break;
                case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                    GTINFO("GPU type is discrete");
                    break;
                case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                    GTINFO("GPU type is virtual");
                    break;
                case VK_PHYSICAL_DEVICE_TYPE_CPU:
                    GTINFO("GPU type is CPU");
                    break;
            }

            GTINFO("GPU Driver version %d.%d.%d",
                VK_VERSION_MAJOR(context->device.vkDeviceInfo[i].properties.driverVersion),
                VK_VERSION_MAJOR(context->device.vkDeviceInfo[i].properties.driverVersion),
                VK_VERSION_PATCH(context->device.vkDeviceInfo[i].properties.driverVersion)
            );

            GTINFO("Vulkan API version %d.%d.%d",
                VK_VERSION_MAJOR(context->device.vkDeviceInfo[i].properties.apiVersion),
                VK_VERSION_MAJOR(context->device.vkDeviceInfo[i].properties.apiVersion),
                VK_VERSION_PATCH(context->device.vkDeviceInfo[i].properties.apiVersion)
            );

            for (GtU32 j = 0; j < context->device.vkDeviceInfo[i].memory.memoryHeapCount; j++) {
                GtF32 memorySizeGib = (((GtF32)context->device.vkDeviceInfo[i].memory.memoryHeaps[j].size) / 1024.0f / 1024.0f / 1024.0f);

                if (context->device.vkDeviceInfo[i].memory.memoryHeaps[j].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
                    GTINFO("Local GPU memory: %.2f GiB", memorySizeGib);
                } else {
                    GTINFO("Shared system memory: %.2f GiB", memorySizeGib);
                }
            }

            context->device.physicalDevice = context->device.vkDevices.devices[i];
            context->device.graphicsQueueIndex = queueInfo.graphicsFamilyIndex;
            context->device.presentQueueIndex = queueInfo.presentFamilyIndex;
            context->device.transferQueueIndex = queueInfo.transferFamilyIndex;
            context->device.computeQueueIndex = queueInfo.computeFamilyIndex;

            context->device.properties = context->device.vkDeviceInfo[i].properties;
            context->device.features = context->device.vkDeviceInfo[i].features;
            context->device.memory = context->device.vkDeviceInfo[i].memory;

            break;
        }
    }

    if (!context->device.physicalDevice) {
        GTERROR("No physical devices were found witch meet the requirements.");
        return GtFalse;
    }

    GTINFO("Physical device selected");
    return GtTrue;
}
void gontiVkPhysicalDeviceRelease(struct GtVkContext* context) {
    GTINFO("Release physical device resources...");
    
    if (context->device.vkDevices.devices) {
        gt_free(context->device.vkDevices.devices);

        context->device.vkDevices = ZERO_PHYSICAL_DEVICES;
        context->device.vkDevices.devices = 0;
        context->device.vkDevices.count = 0;
    }

    if (context->device.vkDeviceInfo) {
        gt_free(context->device.vkDeviceInfo);
        context->device.vkDeviceInfo = 0;
    }

    gt_zeroMemory(&context->device.swapchainSupport.capabilities, sizeof(context->device.swapchainSupport.capabilities));

    context->device.graphicsQueueIndex = -1;
    context->device.presentQueueIndex = -1;
    context->device.computeQueueIndex = -1;
    context->device.transferQueueIndex = -1;
    
    vkDestroySurfaceKHR(context->instance, context->surface, context->allocator);
    context->surface = 0;

    context->device.physicalDevice = 0;
    context->allocator = 0;
}
