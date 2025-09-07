#ifndef VULKANSYNCOBJECTS_H
#define VULKANSYNCOBJECTS_H

    #ifdef __cplusplus
        extern "C" {
    #endif

            #include "../../../../__GONTI_INCLUDES.h"
            
            #ifdef USE_VULKAN
            #ifdef SyncObjects_ON

                #include "../../Types/Vulkan/VulkanContextTypes.inl"

                KAPI void vulkanSyncObjectsCreate(gontiVulkanContext* context);
                KAPI void vulkanSyncObjectsDestroy(gontiVulkanContext* context);

            #endif
            #endif

    #ifdef __cplusplus
        }
    #endif

#endif