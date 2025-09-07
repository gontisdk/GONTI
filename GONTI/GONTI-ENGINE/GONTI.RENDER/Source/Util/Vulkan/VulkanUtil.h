#ifndef VULKANUTIL_H
#define VULKANUTIL_H

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../../Config/SetupGraphicBackend/UserSetGraphicBackend.h"

        #if GONTI_USE_VULKAN

            #include <vulkan/vulkan.h>
            #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/Defines.h>

            KAPI const char* gontiVkUtilResultToString(VkResult result, b8 getExtended);
            
            KAPI b8 gontiVkUtilResultIsSuccess(VkResult result);

        #endif

#ifdef __cplusplus
    }
#endif

#endif