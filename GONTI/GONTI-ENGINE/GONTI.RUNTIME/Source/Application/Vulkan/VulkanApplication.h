#ifndef VULKANAPPLICATION_H
#define VULKANAPPLICATION_H

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI/GONTI-ENGINE/GONTI.RENDER/Source/Config/SetupGraphicBackend/UserSetGraphicBackend.h>

        #if GONTI_USE_VULKAN

            #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/Defines.h>
            #include "../../EntryPoint/Vulkan/VulkanEntry.h"

            /* B8 */
            KAPI b8 gontiVkApplicationCreate(GontiVulkanEntry* entryInst);
            KAPI b8 gontiVkApplicationRun(u64 fpsLimit, b8 isFpsLimit);

            /* VOID */
            KAPI void gontiVkApplicationShutDown();
            KAPI void gontiVkApplicationPause();
            KAPI void gontiVkApplicationResume();
            KAPI void gontiVkApplicationGetFramebufferSizePtr(u32* width, u32* height);
            KAPI void gontiVkApplicationGetWidthPtr(u32* width);
            KAPI void gontiVkApplicationGetHeightPtr(u32* height);
            KAPI void gontiVkApplicationWidthSet(u32 width);
            KAPI void gontiVkApplicationHeightSet(u32 height);
            KAPI void gontiVkApplicationOnResized(u32 width, u32 height);

            /* i16 */
            KAPI i16 gontiVkApplicationGetWidth();
            KAPI i16 gontiVkApplicationGetHeight();

            /* BOOL/B8 */
            KAPI b8 gontiVkApplicationIsPaused();

        #endif

#ifdef __cplusplus
    }
#endif

#endif