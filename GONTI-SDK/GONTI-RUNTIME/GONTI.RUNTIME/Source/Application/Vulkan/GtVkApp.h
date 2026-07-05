#ifndef GtVkAppH
#define GtVkAppH

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI-RENDER/GONTI.RENDER/Source/Config/SetupGraphicBackend/GtUsrSetGraphBackend.h>

        #if GONTI_USE_VULKAN

            #include <GONTI-CORE/GONTI.CORE/Source/Defines/GtDefines.inl>
            #include <EntryPoint/GtEntry.h>

            /* B8 */
            GTAPI GtB8 gontiVkApplicationCreate(GtEntry* entryInst);
            GTAPI GtB8 gontiVkApplicationRun(GtU64 fpsLimit, GtB8 isFpsLimit);

            /* VOID */
            GTAPI void gontiVkApplicationShutdown(void);
            GTAPI void gontiVkApplicationPause(void);
            GTAPI void gontiVkApplicationResume(void);
            GTAPI void gontiVkApplicationSuspend(void);
            GTAPI void gontiVkApplicationUnSuspend(void);
            GTAPI void gontiVkApplicationGetFramebufferSizePtr(GtU32* width, GtU32* height);
            GTAPI void gontiVkApplicationGetWidthPtr(GtU32* width);
            GTAPI void gontiVkApplicationGetHeightPtr(GtU32* height);
            GTAPI void gontiVkApplicationSetWidth(GtU32 width);
            GTAPI void gontiVkApplicationSetHeight(GtU32 height);
            GTAPI void gontiVkApplicationOnResized(GtU32 width, GtU32 height);

            /* GtI16 */
            GTAPI GtI16 gontiVkApplicationGetWidth(void);
            GTAPI GtI16 gontiVkApplicationGetHeight(void);

            /* BOOL/B8 */
            GTAPI GtB8 gontiVkApplicationIsSuspended(void);
            GTAPI GtB8 gontiVkApplicationIsPaused(void);

        #endif

#ifdef __cplusplus
    }
#endif

#endif