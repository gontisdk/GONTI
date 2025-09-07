#ifndef VULKANUTIL_H
#define VULKANUTIL_H

#ifdef __cplusplus
    extern "C" {
#endif
    
    #include "../../../__GONTI_INCLUDES.h"

    #ifdef USE_VULKAN

            #include "../../../Core/RendererSubsystem/Types/Vulkan/VulkanContextTypes.inl"
            #include "../../../Core/RendererSubsystem/Renderer/Backend/Vulkan/VulkanRendererBackend.h"
            
            KAPI b8 vulkan_result_is_success(VkResult result);

            KAPI const char* vulkan_result_string(VkResult result, b8 getExtended);

    #endif

#ifdef __cplusplus
    }
#endif

#endif