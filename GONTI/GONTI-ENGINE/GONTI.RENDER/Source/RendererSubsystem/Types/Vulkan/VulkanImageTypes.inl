#ifndef VULKANIMAGETYPES_INL
#define VULKANIMAGETYPES_INL

#ifdef __cplusplus 
    extern "C" {
#endif

        #include "../../../Config/SetupGraphicBackend/UserSetGraphicBackend.h"

        #ifdef GONTI_USE_VULKAN

            #include <vulkan/vulkan.h>
            #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/Defines.h>

            typedef struct GontiVulkanImage {
                VkImage handle;
                VkDeviceMemory memory;
                VkImageView view;
                u32 width;
                u32 height;
            }GontiVulkanImage;

        #endif

#ifdef __cplusplus 
    }
#endif

#endif