#ifndef VULKANSYNCOBJECTS_H
#define VULKANSYNCOBJECTS_H

    #ifdef __cplusplus
        extern "C" {
    #endif

            #include "../../../Config/SetupGraphicBackend/UserSetGraphicBackend.h"
            
            #ifdef GONTI_USE_VULKAN

                #include "../../../Types/Vulkan/VulkanContextTypes.inl"

                KAPI void gontiVkSyncObjectsCreate(GontiVulkanContext* context);
                KAPI void gontiVkSyncObjectsDestroy(GontiVulkanContext* context);

            #endif

    #ifdef __cplusplus
        }
    #endif

#endif