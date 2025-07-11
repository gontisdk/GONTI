#include "VulkanRendererBackend.h"

#ifdef USE_VULKAN
#ifdef RendererBackend_ON

#include "../../../Types/Vulkan/VulkanContextTypes.inl"
#include "../../../../../Utilities/Logging/Scripts/Logger.h"
#include "../../../../../Libs/StringLib/StringLib.h"
#include "../../../../../Containers/DynamicArray/DynamicArray.h"
#include "../../../../Platform/Platform.h"
#include "../../../../Platform/Vulkan/VulkanPlatform.h"
#include "../../../../../Utilities/Debug/BackendDebugger/Vulkan/VulkanDebugger.h"
#include "../../../../Hardware/Device/Vulkan/VulkanDevice.h"
#include "../../../Swapchain/Vulkan/VulkanSwapchain.h"
#include "../../../Renderpass/Vulkan/VulkanRenderpass.h"
#include "../../../../MemorySubsystem/MemorySubsystem.h"
#include "../../../CommandBuffer/Vulkan/VulkanCommandBuffer.h"
#include "../../../FrameBuffer/Vulkan/VulkanFramebuffer.h"
#include "../../../../../Environments/Vulkan/Application/VulkanApplication.h"
#include "../../../SyncObjects/Vulkan/VulkanSyncObjects.h"
#include "../../../Fence/Vulkan/VulkanFence.h"
#include "../../../../../Utilities/Util/Vulkan/VulkanUtil.h"

static gontiVulkanContext context;

/* === [{ VV }]==[TEMP VERBS]==[{ VV }] === */

static u32 cachedFramebufferWidth = 0;
static u32 cachedFramebufferHeight = 0;

/* === [{ ^^ }]==[TEMP VERBS]==[{ ^^ }] === */

/*PRIVATE FUNCS*/

i32 findMemoryIndex(u32 typeFilter, u32 propertyFlags) {
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(context.device.physicalDevice, &memoryProperties);

    for (u32 i = 0; i < memoryProperties.memoryTypeCount; i++) {
        if (typeFilter & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & propertyFlags) == propertyFlags) {
            return i;
        }
    }

    KWARN("Unable to find suitable memory type!");
    return -1;
}

void createCommandBuffers(gontiRendererBackend* backend) {
    if (!context.graphicsCommandBuffers) {
        context.graphicsCommandBuffers = darrayReserve(gontiVulkanCommandBuffer, context.swapchain.imageCount);

        for (u32 i = 0; i < context.swapchain.imageCount; i++) {
            k_zeroMemory(&context.graphicsCommandBuffers[i], sizeof(gontiVulkanCommandBuffer));
        }
    }

    for (u32 i = 0; i < context.swapchain.imageCount; i++) {
        if (context.graphicsCommandBuffers[i].handle) {
            vulkanCommandBufferFree(
                &context,
                context.device.graphicsCommandPool,
                &context.graphicsCommandBuffers[i]
            );
            k_zeroMemory(&context.graphicsCommandBuffers[i], sizeof(gontiVulkanCommandBuffer));
            vulkanCommandBufferAllocate(
                &context,
                context.device.graphicsCommandPool,
                true,
                &context.graphicsCommandBuffers[i]
            );
        }
    }

    KINFO("Vulkan command buffers created");
}

void regenerateFramebuffers(gontiRendererBackend* backend, gontiVulkanSwapchain* swapchain, gontiVulkanRenderpass* renderpass) {
    for (u32 i = 0; i < swapchain->imageCount; i++) {
        KINFO("Vulkan creating framebuffers...");

        // TODO: make this dynamic based on the currently configured attachments
        u32 attachmentsCount = 2;
        VkImageView attachments[] = {
            swapchain->views[i],
            swapchain->depthAttachment.view
        };

        vulkanFramebufferCreate(
            &context,
            renderpass,
            context.framebufferWidth,
            context.framebufferHeight,
            attachmentsCount,
            attachments,
            &context.swapchain.framebuffers[i]
        );
    }
}

b8 recreateSwapchain(gontiRendererBackend* backend) {
    if (context.swapchain.recreatingSwapchain) {
        KDEBUG("recreateSwapchain called when already recreating. Booting...");
        return false;
    } 

    if (context.framebufferWidth == 0 || context.framebufferHeight == 0) {
        KDEBUG("recreateSwapchain called when window < 1 in a dimension. Booting...");
        return false;
    }

    context.swapchain.recreatingSwapchain = true;

    vkDeviceWaitIdle(context.device.logicalDevice);

    for (u32 i = 0; i < context.swapchain.imageCount; i++) {
        context.imagesInFlight[i] = 0;
    }

    vulkanDeviceQuerySwapchainSupport(
        context.device.physicalDevice,
        context.surface,
        &context.device.swapchainSupport
    );
    vulkanDeviceDetectDepthFormat(&context);
    vulkanSwapchainRecreate(
        &context,
        cachedFramebufferWidth,
        cachedFramebufferHeight,
        &context.swapchain
    );

    context.framebufferWidth = cachedFramebufferWidth;
    context.framebufferHeight = cachedFramebufferHeight;
    context.mainRenderpass.w = context.framebufferWidth;
    context.mainRenderpass.y = context.framebufferHeight;
    cachedFramebufferWidth = 0;
    cachedFramebufferHeight = 0;

    context.framebufferSizeLastGeneration = context.framebufferSizeGeneration;

    for (u32 i = 0; i < context.swapchain.imageCount; i++) {
        vulkanCommandBufferFree(&context, context.device.graphicsCommandPool, &context.graphicsCommandBuffers[i]);
    }

    for (u32 i = 0; i < context.swapchain.imageCount; i++) {
        vulkanFramebufferDestroy(&context, &context.swapchain.framebuffers[i]);
    }

    context.mainRenderpass.x = 0;
    context.mainRenderpass.y = 0;
    context.mainRenderpass.w = context.framebufferWidth;
    context.mainRenderpass.h = context.framebufferHeight;

    regenerateFramebuffers(backend, &context.swapchain, &context.mainRenderpass);
    createCommandBuffers(backend);

    context.swapchain.recreatingSwapchain = false;

    return true;
}

/*PUBLIC FUNCS*/

b8 vulkanRendererBackendInitialize(gontiRendererBackend* backend, const char* appName, struct gontiPlatformState* platState) {
    context.findMemoryIndex = findMemoryIndex;
    
    context.allocator = 0; // TODO: custom allocator

    applicationGetFramebufferSize(&cachedFramebufferWidth, &cachedFramebufferWidth);
    context.framebufferWidth = (cachedFramebufferWidth != 0) ? cachedFramebufferWidth : 800;
    context.framebufferHeight = (cachedFramebufferHeight != 0) ? cachedFramebufferHeight : 600;
    cachedFramebufferWidth = 0;
    cachedFramebufferHeight = 0;

    VkApplicationInfo appInfo = {VK_STRUCTURE_TYPE_APPLICATION_INFO};
    appInfo.apiVersion = VK_API_VERSION_1_2;
    appInfo.pApplicationName = appName;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "GONTI Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

    VkInstanceCreateInfo createInfo = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
    createInfo.pApplicationInfo = &appInfo;

    const char** requiredExtensions = darrayCreate(const char*);
    darrayPush(requiredExtensions, &VK_KHR_SURFACE_EXTENSION_NAME);
    platformGetRequiredExtensionNames(&requiredExtensions);

    #if defined(_DEBUG)
        darrayPush(requiredExtensions, &VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        KDEBUG("Required extensions:");
        u32 length = darrayLength(requiredExtensions);
        for (u32 i = 0; i < length; i++) {
            KDEBUG(requiredExtensions[i]);
        }
    #endif

    createInfo.enabledExtensionCount = darrayLength(requiredExtensions);
    createInfo.ppEnabledExtensionNames = requiredExtensions;

    const char** requiredValidationLayerNames = 0;
    u32 requiredValidationLayerCount = 0;

    #if defined(_DEBUG)
        KDEBUG("Validation layers enabled. Enumerating...");

        requiredValidationLayerNames = darrayCreate(const char*);
        darrayPush(requiredValidationLayerNames, &"VK_LAYER_KHRONOS_validation");
        requiredValidationLayerCount = darrayLength(requiredValidationLayerNames);

        u32 availableLayerCount = 0;
        VK_CHECK(vkEnumerateInstanceLayerProperties(&availableLayerCount, 0));
        VkLayerProperties* availableLayers = darrayReserve(VkLayerProperties, availableLayerCount);
        VK_CHECK(vkEnumerateInstanceLayerProperties(&availableLayerCount, availableLayers));

        for (u32 i = 0; i < requiredValidationLayerCount; i++) {
            KDEBUG("Searching for layer: %s...", requiredValidationLayerNames[i]);
            b8 found = false;

            for (u32 j = 0; j < availableLayerCount; j++) {
                if (stringsEqual(requiredValidationLayerNames[i], availableLayers[j].layerName)) {
                    found = true;
                    KDEBUG("Found Successfully");
                    break;
                }
            }

            if (!found) {
                KFATAL("Required validation layers is missing: %s", requiredValidationLayerNames[i]);
                return false;
            }
        }

        KDEBUG("All requred validation layers are present.");
    #endif

    createInfo.enabledLayerCount = requiredValidationLayerCount;
    createInfo.ppEnabledLayerNames = requiredValidationLayerNames;

    VK_CHECK(vkCreateInstance(&createInfo, context.allocator, &context.instance));
    KINFO("Vulkan Instance created");

    #if defined(_DEBUG)
        KDEBUG("Creating Vulkan debugger...");

        u32 logSecurity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT 
                        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
                        //| VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
                        //| VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};
        debugCreateInfo.messageSeverity = logSecurity;
        debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                                    | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                                    | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debugCreateInfo.pfnUserCallback = __vkDebugCallback;
        debugCreateInfo.pUserData = 0;

        PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(context.instance, "vkCreateDebugUtilsMessengerEXT");
        KASSERT_MSG(func, "Failed to create debug messenger!");
        VK_CHECK(func(context.instance, &debugCreateInfo, context.allocator, &context.debugMessenger));

        KDEBUG("Vulkan debugger created.");
    #endif

    KINFO("Creating Vulkan surface...");
    if (!platformCreateVulkanSurface(platState, &context)) {
        KERROR("Failed to create Vulkan surface!");
        return false;
    } KINFO("Vulkan surface created.");

    if (!vulkanDeviceCreate(&context)) {
        KERROR("Failed to create device!");
        return false;
    }

    KINFO("Creating Vulkan swapchain...");
    vulkanSwapchainCreate(&context, context.framebufferWidth, context.framebufferHeight, &context.swapchain);

    KINFO("Creating Vulkan renderpass...");
    vulkanRenderpassCreate(
        &context, &context.mainRenderpass,
        0, 0, context.framebufferWidth, context.framebufferHeight,
        0.0f, 0.0f, 0.2f, 1.0f, 
        1.0f, 
        0
    );

    context.swapchain.framebuffers = darrayReserve(gontiVulkanFramebuffer, context.swapchain.imageCount);
    regenerateFramebuffers(backend, &context.swapchain, &context.mainRenderpass);

    KINFO("Creating Vulkan command buffers...");
    createCommandBuffers(backend);

    KINFO("Creating Vulkan Sync Objects...");
    vulkanSyncObjectsCreate(&context);    

    KINFO("Vulkan renderer initialized successfully");
    return true;
}
b8 vulkanRendererBackendBeginFrame(gontiRendererBackend* backend, f32 deltaTime) {
    gontiVulkanDevice* device = &context.device;

    if (context.swapchain.recreatingSwapchain) {
        VkResult result = vkDeviceWaitIdle(device->logicalDevice);

        if (!vulkan_result_is_success(result)) {
            KERROR("In func vulkanRendererBackendBeginFrame(), vkDeviceWaitIdle (1) failed: '%s'", vulkan_result_string(result, true));
            return false;
        }

        KINFO("Recreating swapchain, booting.");
        return false;
    }

    if (context.framebufferSizeGeneration != context.framebufferSizeLastGeneration) {
        VkResult result = vkDeviceWaitIdle(device->logicalDevice);
        
        if (!vulkan_result_is_success(result)) {
            KERROR("In func vulkanRendererBackendBeginFrame(), vkDeviceWaitIdle (2) failed: '%s'", vulkan_result_string(result, true));
            return false;
        }

        if (!recreateSwapchain(backend)) {
            KERROR("Recreating swapchain failed.");
            return false;
        }

        KINFO("Resized, booting...");
        return false;
    }

    if (!vulkanFenceWait(
        &context,
        &context.inFlightFences[context.currentFrame],
        UINT64_MAX
    )) {
        KWARN("In-flight fence wait failure!");
        return false;
    }

    if (vulkanSwapchainAcquireNextImageIndex(
        &context,
        &context.swapchain,
        UINT64_MAX,
        context.imageAvailableSemaphores[context.currentFrame],
        0,
        &context.imageIndex
    )) {
        KERROR("Vulkan Swapchain Acquire Next Image Index failed.");
        return false;
    }

    gontiVulkanCommandBuffer* commandBuffer = &context.graphicsCommandBuffers[context.imageIndex];

    vulkanCommandBufferReset(commandBuffer);
    vulkanCommandBufferBegin(commandBuffer, false, false, false);

    VkViewport viewport;
    viewport.x = 0.0f;
    viewport.y = (f32)context.framebufferHeight;
    viewport.width = (f32)context.framebufferWidth;
    viewport.height = -(f32)context.framebufferHeight;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor;
    scissor.offset.x = scissor.offset.y = 0;
    scissor.extent.width = context.framebufferWidth;
    scissor.extent.height = context.framebufferHeight;

    vkCmdSetViewport(commandBuffer->handle, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer->handle, 0, 1, &scissor);

    context.mainRenderpass.w = context.framebufferWidth;
    context.mainRenderpass.h = context.framebufferHeight;

    vulkanRenderpassBegin(
        commandBuffer,
        &context.mainRenderpass,
        context.swapchain.framebuffers[context.imageIndex].handle
    );

    return true;
}
b8 vulkanRendererBackendEndFrame(gontiRendererBackend* backend, f32 deltaTime) {
    gontiVulkanCommandBuffer* commandBuffer = &context.graphicsCommandBuffers[context.imageIndex];

    vulkanRenderpassEnd(commandBuffer, &context.mainRenderpass);
    vulkanCommandBufferEnd(commandBuffer);

    if (context.imagesInFlight[context.imageIndex]->handle != VK_NULL_HANDLE) {
        vulkanFenceWait(
            &context,
            context.imagesInFlight[context.imageIndex],
            UINT64_MAX
        );
    }

    context.imagesInFlight[context.imageIndex] = &context.inFlightFences[context.currentFrame];
    
    vulkanFenceReset(&context, &context.inFlightFences[context.currentFrame]);

    VkSubmitInfo submitInfo = {VK_STRUCTURE_TYPE_SUBMIT_INFO};
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer->handle;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &context.queueCompleteSemaphore[context.currentFrame];
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &context.imageAvailableSemaphores[context.currentFrame];

    VkPipelineStageFlags flags[1] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.pWaitDstStageMask = flags;

    VkResult result = vkQueueSubmit(
        context.device.graphicsQueue,
        1,
        &submitInfo,
        context.inFlightFences[context.currentFrame].handle
    );

    if (result != VK_SUCCESS) {
        KERROR("vkQueueSubmit failed with result: %s", vulkan_result_string(result, true));
        return false;
    }

    vulkanCommandBufferUpdateSubmitted(commandBuffer);
    vulkanSwapchainPresent(
        &context,
        &context.swapchain,
        context.device.graphicsQueue,
        context.device.presentQueue,
        context.queueCompleteSemaphore[context.currentFrame],
        context.imageIndex
    );

    return true;
}

void vulkanRendererBackendShutdown(gontiRendererBackend* backend) {
    #if defined(_DEBUG)
    KDEBUG("Destroying Vulkan debugger...");
    if (context.debugMessenger) {
        PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(context.instance, "vkDestroyDebugUtilsMessengerEXT");
        func(context.instance, context.debugMessenger, context.allocator);
    }
    #endif

    KINFO("Destroying Vulkan Sync Objects...");
    vulkanSyncObjectsDestroy(&context);

    KINFO("Destroying Vulkan command buffers...");
    for (u32 i = 0; i < context.swapchain.imageCount; i++) {
        if (context.graphicsCommandBuffers[i].handle) {
            vulkanCommandBufferFree(
                &context,
                context.device.graphicsCommandPool,
                &context.graphicsCommandBuffers[i]
            );
            context.graphicsCommandBuffers[i].handle = 0;
        }
    } darrayDestroy(context.graphicsCommandBuffers);
    context.graphicsCommandBuffers = 0;

    KINFO("Destroying Vulkan framebuffers...");
    for (u32 i = 0; i < context.swapchain.imageCount; i++) {
        vulkanFramebufferDestroy(&context, &context.swapchain.framebuffers[i]);
    }

    KINFO("Destroying Vulkan renderpass...");
    vulkanRenderpassDestroy(&context, &context.mainRenderpass);

    KINFO("Destroying Vulkan swapchain...");
    vulkanSwapchainDestroy(&context, &context.swapchain);
    
    KINFO("Destroying handle to device...");
    vulkanDeviceRelease(&context);
    
    KINFO("Destroying Vulkan surface...");
    if (context.surface) {
        vkDestroySurfaceKHR(context.instance, context.surface, context.allocator);
        context.surface = 0;
    }
    
    KINFO("Destroying Vulkan Instance...");
    vkDestroyInstance(context.instance, context.allocator);
    context.instance = 0;

    KINFO("All destroyed successfully");
}
void vulkanRendererBackendOnResized(gontiRendererBackend* backend, u16 width, u16 height) {
    cachedFramebufferWidth = width;
    cachedFramebufferHeight = height;
    context.framebufferSizeGeneration++;

    KINFO("Vulkan renderer backend->resized. Width: %i, Height: %i, Gen: %llu", width, height, context.framebufferSizeGeneration);


}

#endif
#endif