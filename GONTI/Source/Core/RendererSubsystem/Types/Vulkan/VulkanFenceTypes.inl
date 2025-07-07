#ifndef VULKANFENCETYPES_INL
#define VULKANFENCETYPES_INL

#ifdef __cplusplus 
    extern "C" {
#endif

        #include "../../../../__GONTI_INCLUDES.h"

        #ifdef USE_VULKAN
        #ifdef Fence_ON

            #include <vulkan/vulkan.h>

            typedef struct gontiVulkanFence {
                VkFence handle;
                b8 isSignaled;
            }gontiVulkanFence;

        #endif
        #endif

#ifdef __cplusplus 
    }
#endif

#endif