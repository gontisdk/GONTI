#include "VulkanDebugger.h"

#if GONTI_USE_VULKAN && _DEBUG

#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Logging/Logger.h>

VKAPI_ATTR VkBool32 VKAPI_CALL __vkDebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
    VkDebugUtilsMessageTypeFlagsEXT messageTypes, 
    const VkDebugUtilsMessengerCallbackDataEXT* callbackData, 
    void* userData
) {
    switch (messageSeverity) {
        default:
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            KERROR("##[{VK DEBUGGER}]##: %s", callbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            KWARN(callbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            KINFO(callbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            KTRACE(callbackData->pMessage);
            break;
    }
    return VK_FALSE;
}

#endif