#ifndef VULKANPLATFORM_H
#define VULKANPLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

    #include "../../../__GONTI_INCLUDES.h"

    #ifdef PlatformVulkan_ON
    #ifdef USE_VULKAN

        #include "../../Hardware/Device/Vulkan/VulkanDevice.h"

        typedef struct gontiPlatformState {
            void* internalState;
        }gontiPlatformState;

        KAPI b8 platformStartup(
            gontiPlatformState* platState,
            const char* className,
            const char* appName,
            i32 x,
            i32 y,
            i32 width,
            i32 height
        );

        KAPI b8 platformPumpMessage(gontiPlatformState* platState);
        KAPI b8 platformCreateVulkanSurface(struct gontiPlatformState* platState, struct gontiVulkanContext* context);
        
        KAPI f64 platformGetAbsoluteTime();

        KAPI void platformShutdown(gontiPlatformState* platState);
        KAPI void platformGetRequiredExtensionNames(const char*** namesDarray);

    #endif
    #endif

#ifdef __cplusplus
}
#endif

#endif