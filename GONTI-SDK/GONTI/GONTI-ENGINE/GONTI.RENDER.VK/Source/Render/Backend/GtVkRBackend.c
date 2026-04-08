#include "GtVkRBackend.h"

#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Logging/GtLogger.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/CStringTools/GtCStrTools.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Containers/DynamicArray/GtDArray.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Platform/GtPlatform.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Memory/GtMemory.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Math/Geometry/Vertices/GtVertexTypes.inl>
#include "../../Types/GtVkContextTypes.inl"
#include "../../Platform/GtVkPlatform.h"
#include "../../Debugger/GtVkDebugger.h"
#include "../../Hardware/Device/GtVkDevice.h"
#include "../../Subsystems/Swapchain/GtVkSwapchain.h"
#include "../../Subsystems/Renderpass/GtVkRenderpass.h"
#include "../../Subsystems/CommandBuffer/GtVkCmdBuffer.h"
#include "../../Subsystems/FrameBuffer/GtVkFrameBuffer.h"
#include "../../Subsystems/Sync/SyncObjects/GtVkSyncObjects.h"
#include "../../Subsystems/Sync/Fence/GtVkFence.h"
#include "../../Util/GtVkUtil.h"
#include "../../Shaders/ObjectShader/GtVkObjectShader.h"
#include "../../Resources/Buffer/GtVkBuffer.h"

static GtVkContext context;
static const char** requiredExtensions;
static const char** requiredValidationLayerNames;
static GtU32 cachedFramebufferWidth = 0;
static GtU32 cachedFramebufferHeight = 0;

/* TEMPORARY FUNCS */

void __tmp__upload_data_range(GtVkContext* context, VkCommandPool pool, VkFence fence, VkQueue queue, GtVkBuffer* buffer, GtU64 offset, GtU64 size, void* data) {
    VkBufferUsageFlags flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    GtVkBuffer staging;
    gontiVkBufferCreate(context, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, flags, GtTrue, &staging);
    gontiVkBufferLoadData(context, &staging, 0, size, 0, data);
    gontiVkBufferCopyTo(context, pool, fence, queue, staging.handle, 0, buffer->handle, offset, size);
    gontiVkBufferDestroy(context, &staging);
}

/* END OF TEMPORARY FUNCS */

/*PRIVATE FUNCS*/

GtI32 gontiVkFindMemoryIndex(GtU32 typeFilter, GtU32 propertyFlags) {
    if (!context.device.physicalDevice || !context.device.logicalDevice) {
        GTERROR("Physical or Logical Device is NULL in gontiVkFindMemoryIndex!");
        return -1;
    }

    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(context.device.physicalDevice, &memoryProperties);

    for (GtU32 i = 0; i < memoryProperties.memoryTypeCount; i++) {
        if (typeFilter & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & propertyFlags) == propertyFlags) {
            return i;
        }
    }

    GTWARN("Unable to find suitable memory type!");
    return -1;
}

/* END PRIVATE FUNC */

GtB8 gontiVkRendererBackendRecreateSwapchain(GtVkContext* context) {
    if (context->swapchain.recreatingSwapchain) {
        GTDEBUG("gontiVkRecreategontiVkRendererBackendRecreateSwapchain called when already recreating. Booting...");
        return GtFalse;
    } 

    if (context->framebufferWidth == 0 || context->framebufferHeight == 0) {
        GTDEBUG("gontiVkRendererBackendRecreateSwapchain called when window < 1 in a dimension. Booting...");
        return GtFalse;
    }

    context->swapchain.recreatingSwapchain = GtTrue;

    vkDeviceWaitIdle(context->device.logicalDevice);

    for (GtU32 i = 0; i < context->swapchain.imageCount; i++) {
        context->imagesInFlight[i] = 0;
    }

    gontiVkDeviceQuerySwapchainSupport(
        context->device.physicalDevice,
        context->surface,
        &context->device.swapchainSupport
    );
    gontiVkDeviceDetectDepthFormat(context);
    gontiVkSwapchainRecreate(
        context,
        cachedFramebufferWidth,
        cachedFramebufferHeight,
        &context->swapchain
    );

    context->framebufferWidth = cachedFramebufferWidth;
    context->framebufferHeight = cachedFramebufferHeight;
    context->mainRenderpass.w = context->framebufferWidth;
    context->mainRenderpass.h = context->framebufferHeight;
    cachedFramebufferWidth = 0;
    cachedFramebufferHeight = 0;

    context->framebufferSizeLastGeneration = context->framebufferSizeGeneration;

    for (GtU32 i = 0; i < context->swapchain.imageCount; i++) {
        gontiVkCommandBufferFree(context, context->device.graphicsCommandPool, &context->graphicsCommandBuffers[i]);
    }

    for (GtU32 i = 0; i < context->swapchain.imageCount; i++) {
        gontiVkFramebufferDestroy(context, &context->swapchain.framebuffers[i]);
    }

    context->mainRenderpass.x = 0;
    context->mainRenderpass.y = 0;
    context->mainRenderpass.w = context->framebufferWidth;
    context->mainRenderpass.h = context->framebufferHeight;

    gontiVkFramebuffersRegenerate(context, &context->swapchain, &context->mainRenderpass);
    gontiVkCommandBuffersCreate(context);

    context->swapchain.recreatingSwapchain = GtFalse;

    return GtTrue;
}
GtB8 gontiVkRendererBackendInitialize(const char* appName, struct GtVkPlatformState* platState) {
    gt_zeroMemory(&context, sizeof(GtVkContext));
    
    context.gontiVkFindMemoryIndex = gontiVkFindMemoryIndex;
    context.allocator = 0; // TODO: custom allocator

    if (!platState->get_frame_buffer_size_ptr) {
        GTFATAL("get_frame_buffer_size_ptr is NULL ptr");
        return GtFalse;
    }

    platState->get_frame_buffer_size_ptr(&cachedFramebufferWidth, &cachedFramebufferHeight);
    context.framebufferWidth = (cachedFramebufferWidth != 0) ? cachedFramebufferWidth : 800;
    context.framebufferHeight = (cachedFramebufferHeight != 0) ? cachedFramebufferHeight : 600;
    cachedFramebufferWidth = 0;
    cachedFramebufferHeight = 0;

    VkApplicationInfo appInfo = {VK_STRUCTURE_TYPE_APPLICATION_INFO};
    appInfo.apiVersion = VK_API_VERSION_1_2;
    appInfo.pApplicationName = appName;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "GONTI ENGINE RENDER";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

    VkInstanceCreateInfo createInfo = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
    createInfo.pApplicationInfo = &appInfo;

    requiredExtensions = gontiDarrayCreate(const char*);
    gontiDarrayPush(requiredExtensions, &VK_KHR_SURFACE_EXTENSION_NAME);
    gontiVkPlatformGetRequiredExtensionNames(&requiredExtensions);

    #if defined(_DEBUG)
        gontiDarrayPush(requiredExtensions, &VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        GTDEBUG("Required extensions:");
        GtU32 length = gontiDarrayLength(requiredExtensions);
        for (GtU32 i = 0; i < length; i++) {
            GTDEBUG(requiredExtensions[i]);
        }
    #endif

    createInfo.enabledExtensionCount = gontiDarrayLength(requiredExtensions);
    createInfo.ppEnabledExtensionNames = requiredExtensions;

    requiredValidationLayerNames = 0;
    GtU32 requiredValidationLayerCount = 0;

    #if defined(_DEBUG)
        GTDEBUG("Validation layers enabled. Enumerating...");

        requiredValidationLayerNames = gontiDarrayCreate(const char*);
        gontiDarrayPush(requiredValidationLayerNames, &"VK_LAYER_KHRONOS_validation");
        requiredValidationLayerCount = gontiDarrayLength(requiredValidationLayerNames);

        GtU32 availableLayerCount = 0;
        GTVK_CHECK(vkEnumerateInstanceLayerProperties(&availableLayerCount, 0));
        VkLayerProperties* availableLayers = gontiDarrayReserve(VkLayerProperties, availableLayerCount);
        GTVK_CHECK(vkEnumerateInstanceLayerProperties(&availableLayerCount, availableLayers));

        for (GtU32 i = 0; i < requiredValidationLayerCount; i++) {
            GTDEBUG("Searching for layer: %s...", requiredValidationLayerNames[i]);
            GtB8 found = GtFalse;

            for (GtU32 j = 0; j < availableLayerCount; j++) {
                if (gontiStringEqual(requiredValidationLayerNames[i], availableLayers[j].layerName)) {
                    found = GtTrue;
                    GTDEBUG("Found Successfully");
                    break;
                }
            }

            if (!found) {
                GTFATAL("Required validation layers is missing: %s", requiredValidationLayerNames[i]);
                return GtFalse;
            }
        }

        gontiDarrayDestroy(availableLayers);
        availableLayers = 0;

        GTDEBUG("All requred validation layers are present.");
    #endif

    createInfo.enabledLayerCount = requiredValidationLayerCount;
    createInfo.ppEnabledLayerNames = requiredValidationLayerNames;

    GTVK_CHECK(vkCreateInstance(&createInfo, context.allocator, &context.instance));
    GTINFO("Vulkan Instance created");

    #if defined(_DEBUG)
        GTDEBUG("Creating Vulkan debugger...");

        GtU32 logSecurity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT 
                        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
                        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};
        debugCreateInfo.messageSeverity = logSecurity;
        debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                                    | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                                    | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debugCreateInfo.pfnUserCallback = __vkDebugCallback;
        debugCreateInfo.pUserData = 0;

        PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(context.instance, "vkCreateDebugUtilsMessengerEXT");
        GTASSERT_MSG(func, "Failed to create debug messenger!");
        GTVK_CHECK(func(context.instance, &debugCreateInfo, context.allocator, &context.debugMessenger));

        GTDEBUG("Vulkan debugger created.");
    #endif

    GTINFO("Creating Vulkan Surface...");
    if (!gontiVkPlatformCreateVulkanSurface(platState, &context)) {
        GTERROR("Failed to create Vulkan surface!");
        return GtFalse;
    } GTINFO("Vulkan surface created.");

    GTINFO("Creating Vulkan Device...");
    if (!gontiVkDeviceCreate(&context)) {
        GTERROR("Failed to create device!");
        return GtFalse;
    }

    GTINFO("Creating Vulkan Swapchain...");
    gontiVkSwapchainCreate(&context, context.framebufferWidth, context.framebufferHeight, &context.swapchain);

    if (context.swapchain.maxFramesInFlight == 0) {
        GTFATAL("Swapchain maxFramesInFlight is 0 after creation!");
        return GtFalse;
    }
    
    GTINFO("Creating Vulkan Sync Objects...");
    gontiVkSyncObjectsCreate(&context);

    GTINFO("Creating Vulkan Renderpass...");
    gontiVkRenderpassCreate(
        &context, &context.mainRenderpass,
        0, 0, context.framebufferWidth, context.framebufferHeight,
        0.0f, 0.0f, 0.2f, 1.0f, 
        1.0f, 
        0
    );

    context.swapchain.framebuffers = gontiDarrayReserve(GtVkFramebuffer, context.swapchain.imageCount);
    gontiVkFramebuffersRegenerate(&context, &context.swapchain, &context.mainRenderpass);

    GTINFO("Creating Vulkan Command Buffers...");
    gontiVkCommandBuffersCreate(&context);

    GTINFO("Creating Vulkan Object Shader...");
    if (!gontiVkObjectShaderCreate(&context, &context.objectShader)) {
        GTERROR("Error loading built-in basic_lighting shader.");
        return GtFalse;
    }

    GTINFO("Creating Vulkan Buffers...");
    gontiVkBuffersCreate(&context);

    /*
    * TODO: TEMPORARY TEST CODE
    */ 
    const GtU32 vert_count = 4;
    GtVertex3D verts[vert_count];
    gt_zeroMemory(verts, sizeof(GtVertex3D) * vert_count); 

    verts[0].position.x = 0.0;
    verts[0].position.y = -0.5;

    verts[1].position.x = 0.5;
    verts[1].position.y = 0.5;

    verts[2].position.x = 0.0;
    verts[2].position.y = 0.5;

    verts[3].position.x = 0.5;
    verts[3].position.y = -0.5;

    const GtU32 index_count = 6;
    GtU32 indices[index_count];
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 0;
    indices[4] = 3;
    indices[5] = 1;

    __tmp__upload_data_range(
        &context, context.device.graphicsCommandPool, 0,
        context.device.graphicsQueue, &context.objectVertexBuffer, 0,
        sizeof(GtVertex3D) * vert_count, verts
    );
    __tmp__upload_data_range(
        &context, context.device.graphicsCommandPool, 0,
        context.device.graphicsQueue, &context.objectIndexBuffer, 0,
        sizeof(GtU32) * index_count, indices
    );
    /* 
    * TODO: END TEMPORARY CODE
    */

    // NOTE: temporaty removed for tests (looks like usless now)
    //context.currentFrame = 0;

    for (GtU8 i = 0; i < context.swapchain.maxFramesInFlight; i++) {
        if (context.inFlightFences[i].handle == VK_NULL_HANDLE) {
            GTFATAL("Failed to create in-flight fence %d", i);
            return GtFalse;
        }
        if (context.imageAvailableSemaphores[i] == VK_NULL_HANDLE) {
            GTFATAL("Failed to create image available semaphore %d", i);
            return GtFalse;
        }
        if (context.queueCompleteSemaphore[i] == VK_NULL_HANDLE) {
            GTFATAL("Failed to create queue complete semaphore %d", i);
            return GtFalse;
        }
        GTDEBUG("Sync objects %d created successfully - Fence: %p, ImageSem: %p, QueueSem: %p", 
            i, (void*)context.inFlightFences[i].handle, 
            (void*)context.imageAvailableSemaphores[i], 
            (void*)context.queueCompleteSemaphore[i]);
    }

    GTINFO("Vulkan renderer initialized successfully");
    return GtTrue;
}
GtB8 gontiVkRendererBackendBeginFrame(GtF32 deltaTime) {
    GtVkDevice* device = &context.device;

    if (context.currentFrame >= context.swapchain.maxFramesInFlight) {
        GTERROR("currentFrame (%d) is out of bounds (max: %d)", 
            context.currentFrame, context.swapchain.maxFramesInFlight);
        context.currentFrame = 0;
    }

    if (context.swapchain.recreatingSwapchain) {
        VkResult result = vkDeviceWaitIdle(device->logicalDevice);
        if (!gontiVkUtilResultIsSuccess(result)) {
            GTERROR("vkDeviceWaitIdle (1) failed: '%s'", gontiVkUtilResultToString(result, GtTrue));
            return GtFalse;
        }
        GTINFO("Recreating swapchain, booting.");
        return GtFalse;
    }

    if (context.framebufferSizeGeneration != context.framebufferSizeLastGeneration) {
        VkResult result = vkDeviceWaitIdle(device->logicalDevice);
        if (!gontiVkUtilResultIsSuccess(result)) {
            GTERROR("vkDeviceWaitIdle (2) failed: '%s'", gontiVkUtilResultToString(result, GtTrue));
            return GtFalse;
        }

        if (!gontiVkRendererBackendRecreateSwapchain(&context)) {
            GTERROR("Recreating swapchain failed.");
            return GtFalse;
        }
        GTINFO("Resized, booting...");
        return GtFalse;
    }

    GtVkFence* currentFence = &context.inFlightFences[context.currentFrame];
    if (!currentFence || currentFence->handle == VK_NULL_HANDLE) {
        GTERROR("Current fence (%d) is invalid!", context.currentFrame);
        return GtFalse;
    }

    GTDEBUG("BeginFrame: Using fence %d with handle %p", 
        context.currentFrame, (void*)currentFence->handle);

    if (!gontiVkFenceWait(&context, currentFence, 1000000000ULL)) {
        GTWARN("In-flight fence wait failure!");
        return GtFalse;
    }

    gontiVkFenceReset(&context, currentFence);

    VkResult acquireResult = vkAcquireNextImageKHR(
        context.device.logicalDevice,
        context.swapchain.handle,
        GT_U64_MAX,
        context.imageAvailableSemaphores[context.currentFrame],
        VK_NULL_HANDLE,
        &context.imageIndex
    );

    if (acquireResult == VK_ERROR_OUT_OF_DATE_KHR) {
        GTINFO("Swapchain out of date, recreating...");
        if (!gontiVkRendererBackendRecreateSwapchain(&context)) {
            GTERROR("Failed to recreate swapchain");
            return GtFalse;
        }
        return GtFalse;
    } else if (acquireResult != VK_SUCCESS && acquireResult != VK_SUBOPTIMAL_KHR) {
        GTERROR("Failed to acquire swapchain image! Result: %d", acquireResult);
        return GtFalse;
    }

    if (context.imageIndex >= context.swapchain.imageCount) {
        GTERROR("Invalid image index: %d (max: %d)", context.imageIndex, context.swapchain.imageCount);
        return GtFalse;
    }

    GTDEBUG("Acquired image index: %d for frame %d", context.imageIndex, context.currentFrame);

    if (context.imagesInFlight[context.imageIndex] != 0) {
        if (!gontiVkFenceWait(&context, context.imagesInFlight[context.imageIndex], 1000000000ULL)) {
            GTWARN("Image in flight fence wait failed");
        }
    }
    context.imagesInFlight[context.imageIndex] = currentFence;

    GtVkCmdBuffer* commandBuffer = &context.graphicsCommandBuffers[context.imageIndex];
    if (!commandBuffer || commandBuffer->handle == VK_NULL_HANDLE) {
        GTERROR("Invalid command buffer for image %d", context.imageIndex);
        return GtFalse;
    }

    gontiVkCommandBufferReset(commandBuffer);
    gontiVkCommandBufferBegin(commandBuffer, GtFalse, GtFalse, GtFalse);

    VkViewport viewport;
    viewport.x = 0.0f;
    viewport.y = (GtF32)context.framebufferHeight;
    viewport.width = (GtF32)context.framebufferWidth;
    viewport.height = -(GtF32)context.framebufferHeight;
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

    if (context.swapchain.framebuffers[context.imageIndex].handle == VK_NULL_HANDLE) {
        GTERROR("Invalid framebuffer for image %d", context.imageIndex);
        return GtFalse;
    }

    gontiVkRenderpassBegin(
        commandBuffer,
        &context.mainRenderpass,
        context.swapchain.framebuffers[context.imageIndex].handle
    );

    /*
    * TODO: TEMPORARY TEST CODE
    */ 
    gontiVkObjectShaderUse(&context, &context.objectShader);

    VkDeviceSize offsets[1] = {0};
    vkCmdBindVertexBuffers(commandBuffer->handle, 0, 1, &context.objectVertexBuffer.handle, (VkDeviceSize*)offsets);
    vkCmdBindIndexBuffer(commandBuffer->handle, context.objectIndexBuffer.handle, 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(commandBuffer->handle, 6, 1, 0, 0,0);
    /* 
    * TODO: END TEMPORARY CODE
    */

    return GtTrue;
}

GtB8 gontiVkRendererBackendEndFrame(GtF32 deltaTime) {
    if (context.imageIndex >= context.swapchain.imageCount) {
        GTERROR("Invalid image index in EndFrame: %d", context.imageIndex);
        return GtFalse;
    }

    GtVkCmdBuffer* commandBuffer = &context.graphicsCommandBuffers[context.imageIndex];
    if (!commandBuffer || commandBuffer->handle == VK_NULL_HANDLE) {
        GTERROR("Invalid command buffer in EndFrame");
        return GtFalse;
    }

    gontiVkRenderpassEnd(commandBuffer, &context.mainRenderpass);
    gontiVkCommandBufferEnd(commandBuffer);

    if (context.currentFrame >= context.swapchain.maxFramesInFlight) {
        GTERROR("Invalid currentFrame in EndFrame: %d", context.currentFrame);
        return GtFalse;
    }

    VkSubmitInfo submitInfo = {VK_STRUCTURE_TYPE_SUBMIT_INFO};
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer->handle;
    
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &context.imageAvailableSemaphores[context.currentFrame];
    
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &context.queueCompleteSemaphore[context.imageIndex];

    VkPipelineStageFlags flags[1] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.pWaitDstStageMask = flags;

    GtVkFence* currentFence = &context.inFlightFences[context.currentFrame];
    if (!currentFence || currentFence->handle == VK_NULL_HANDLE) {
        GTERROR("Invalid fence for submit in EndFrame");
        return GtFalse;
    }

    GTDEBUG("Submitting frame %d, image %d: wait_sem=%p, signal_sem=%p", 
        context.currentFrame, context.imageIndex,
        (void*)context.imageAvailableSemaphores[context.currentFrame],
        (void*)context.queueCompleteSemaphore[context.imageIndex]);

    VkResult result = vkQueueSubmit(
        context.device.graphicsQueue,
        1,
        &submitInfo,
        currentFence->handle
    );

    if (result != VK_SUCCESS) {
        GTERROR("vkQueueSubmit failed with result: %s", gontiVkUtilResultToString(result, GtTrue));
        return GtFalse;
    }

    gontiVkCommandBufferUpdateSubmitted(commandBuffer);
    
    VkResult presentResult = gontiVkSwapchainPresent(
        &context,
        &context.swapchain,
        context.device.graphicsQueue,
        context.device.presentQueue,
        context.queueCompleteSemaphore[context.imageIndex],
        context.imageIndex
    );

    context.currentFrame = (context.currentFrame + 1) % context.swapchain.maxFramesInFlight;

    return presentResult == VK_SUCCESS || presentResult == VK_SUBOPTIMAL_KHR;
}

void gontiVkRendererBackendShutdown() {
    vkDeviceWaitIdle(context.device.logicalDevice);

    if (requiredExtensions) {
        gontiDarrayDestroy(requiredExtensions);
        requiredExtensions = 0;
    }

    if (requiredValidationLayerNames) {
        gontiDarrayDestroy(requiredValidationLayerNames);
        requiredValidationLayerNames = 0;
    }

    #if defined(_DEBUG)
    GTDEBUG("Destroying Vulkan debugger...");
    if (context.debugMessenger) {
        PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(context.instance, "vkDestroyDebugUtilsMessengerEXT");
        func(context.instance, context.debugMessenger, context.allocator);
    }
    #endif

    GTINFO("Destroying vulkan buffers...");
    gontiVkBuffersDestroy(&context);

    GTINFO("Destroying vulkan object shader...");
    gontiVkObjectShaderDestroy(&context, &context.objectShader);

    GTINFO("Destroying Vulkan Sync Objects...");
    gontiVkSyncObjectsDestroy(&context);

    GTINFO("Destroying Vulkan command buffers...");
    for (GtU32 i = 0; i < context.swapchain.imageCount; i++) {
        if (context.graphicsCommandBuffers[i].handle) {
            gontiVkCommandBufferFree(
                &context,
                context.device.graphicsCommandPool,
                &context.graphicsCommandBuffers[i]
            );
            context.graphicsCommandBuffers[i].handle = 0;
        }
    } gontiDarrayDestroy(context.graphicsCommandBuffers);
    context.graphicsCommandBuffers = 0;

    GTINFO("Destroying Vulkan framebuffers...");
    for (GtU32 i = 0; i < context.swapchain.imageCount; i++) {
        gontiVkFramebufferDestroy(&context, &context.swapchain.framebuffers[i]);
    } gontiDarrayDestroy(context.swapchain.framebuffers);

    GTINFO("Destroying Vulkan renderpass...");
    gontiVkRenderpassDestroy(&context, &context.mainRenderpass);

    GTINFO("Destroying Vulkan swapchain...");
    gontiVkSwapchainDestroy(&context, &context.swapchain);
    gontiVkSwapchainDarraysDestroy(&context.swapchain);
    
    GTINFO("Destroying handle to device...");
    gontiVkDeviceRelease(&context);
    
    GTINFO("Destroying Vulkan surface...");
    if (context.surface) {
        vkDestroySurfaceKHR(context.instance, context.surface, context.allocator);
        context.surface = 0;
    }
    
    GTINFO("Destroying Vulkan Instance...");
    vkDestroyInstance(context.instance, context.allocator);
    context.instance = 0;

    GTINFO("All destroyed successfully");
}
void gontiVkRendererBackendOnResized(GtU16 width, GtU16 height) {
    cachedFramebufferWidth = width;
    cachedFramebufferHeight = height;
    context.framebufferSizeGeneration++;

    GTINFO("Vulkan renderer backend->resized. Width: %i, Height: %i, Gen: %llu", width, height, context.framebufferSizeGeneration);
}
