#ifndef VULKANDEBUGGER_H
#define VULKANDEBUGGER_H

#ifdef __cplusplus
     extern "C" {
#endif

    #include "../../../../__GONTI_INCLUDES.h"

    #ifdef USE_VULKAN
    #ifdef _DEBUG

            #include "../../../../Core/RendererSubsystem/Types/Vulkan/VulkanContextTypes.inl"
            #include "../../../../Core/RendererSubsystem/Renderer/Backend/Vulkan/VulkanRendererBackend.h"

            VKAPI_ATTR VkBool32 VKAPI_CALL __vkDebugCallback(
                VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
                VkDebugUtilsMessageTypeFlagsEXT messageTypes, 
                const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
                void* userData
            );
        
    #endif
    #endif

#ifdef __cplusplus
    }
#endif

#endif