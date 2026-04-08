#ifndef GtVkAppH
#define GtVkAppH

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI/GONTI-ENGINE/GONTI.RENDER/Source/Config/SetupGraphicBackend/GtUsrSetGraphBackend.h>

        #if GONTI_USE_VULKAN

            #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/GtDefines.h>
            #include "../../EntryPoint/GtEntry.h"

            /* B8 */
            GTAPI GtB8 gontiVkApplicationCreate(GtEntry* entryInst);
            GTAPI GtB8 gontiVkApplicationRun(GtU64 fpsLimit, GtB8 isFpsLimit);

            /* VOID */
            GTAPI void gontiVkApplicationShutdown();
            GTAPI void gontiVkApplicationPause();
            GTAPI void gontiVkApplicationResume();
            GTAPI void gontiVkApplicationSuspend();
            GTAPI void gontiVkApplicationUnSuspend();
            GTAPI void gontiVkApplicationGetFramebufferSizePtr(GtU32* width, GtU32* height);
            GTAPI void gontiVkApplicationGetWidthPtr(GtU32* width);
            GTAPI void gontiVkApplicationGetHeightPtr(GtU32* height);
            GTAPI void gontiVkApplicationSetWidth(GtU32 width);
            GTAPI void gontiVkApplicationSetHeight(GtU32 height);
            GTAPI void gontiVkApplicationOnResized(GtU32 width, GtU32 height);

            /* GtI16 */
            GTAPI GtI16 gontiVkApplicationGetWidth();
            GTAPI GtI16 gontiVkApplicationGetHeight();

            /* BOOL/B8 */
            GTAPI GtB8 gontiVkApplicationIsSuspended();
            GTAPI GtB8 gontiVkApplicationIsPaused();

        #endif

#ifdef __cplusplus
    }
#endif

#endif