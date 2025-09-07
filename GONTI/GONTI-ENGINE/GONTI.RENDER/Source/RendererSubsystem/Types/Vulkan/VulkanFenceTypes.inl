#ifndef VULKANFENCETYPES_INL
#define VULKANFENCETYPES_INL

#ifdef __cplusplus 
    extern "C" {
#endif

        #include "../../../Config/SetupGraphicBackend/UserSetGraphicBackend.h"

        #ifdef GONTI_USE_VULKAN

            #include <vulkan/vulkan.h>
            #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/Defines.h>

            typedef struct GontiVulkanFence {
                VkFence handle;
                b8 isSignaled;
            }GontiVulkanFence;

        #endif

#ifdef __cplusplus 
    }
#endif

#endif