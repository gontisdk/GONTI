#include <Debugger/GtVkDebugger.h>

#if _DEBUG

#include <GONTI-ENGINE/GONTI.CORE/Source/Logging/GtLogger.h>

VKAPI_ATTR VkBool32 VKAPI_CALL __vkDebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
    VkDebugUtilsMessageTypeFlagsEXT messageTypes, 
    const VkDebugUtilsMessengerCallbackDataEXT* callbackData, 
    void* userData
) {
    switch (messageSeverity) {
        default:
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            GTERROR("##[{VK DEBUGGER}]##: %s", callbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            GTWARN(callbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            GTINFO(callbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            GTTRACE(callbackData->pMessage);
            break;
    }
    return VK_FALSE;
}

#endif