#ifndef VULKANIMAGETYPES_INL
#define VULKANIMAGETYPES_INL

#ifdef __cplusplus 
    extern "C" {
#endif

        #include "../../../../__GONTI_INCLUDES.h"

        #ifdef USE_VULKAN
        #ifdef Image_ON

            #include <vulkan/vulkan.h>

            typedef struct gontiVulkanImage {
                VkImage handle;
                VkDeviceMemory memory;
                VkImageView view;
                u32 width;
                u32 height;
            }gontiVulkanImage;

        #endif
        #endif

#ifdef __cplusplus 
    }
#endif

#endif