#ifndef VULKANENTRY_H
#define VULKANENTRY_H

    #ifdef __cplusplus
        extern "C" {
    #endif

            #include "../../../__GONTI_INCLUDES.h"

            #ifdef USE_VULKAN
            #ifdef EntryPointVulkan_ON

                #include "../../../Environments/Vulkan/Application/VulkanApplication.h"
                #include "../../../Utilities/Logging/Scripts/Logger.h"
                #include "VulkanEntryTypes.h"

                extern b8 gontiVkEntryPoint(gontiVkEntry* outEntry);

            #endif
            #endif

    #ifdef __cplusplus
        }
    #endif

#endif