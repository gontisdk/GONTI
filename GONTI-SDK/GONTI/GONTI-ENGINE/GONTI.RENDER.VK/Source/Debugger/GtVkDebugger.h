#ifndef GtVkDebuggerH
#define GtVkDebuggerH

#ifdef __cplusplus
    extern "C" {
#endif

        #if _DEBUG

            #include <vulkan/vulkan.h>
            #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/GtDefines.h>

            VKAPI_ATTR VkBool32 VKAPI_CALL __vkDebugCallback(
                VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
                VkDebugUtilsMessageTypeFlagsEXT messageTypes, 
                const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
                void* userData
            );

        #endif

#ifdef __cplusplus
    }
#endif

#endif