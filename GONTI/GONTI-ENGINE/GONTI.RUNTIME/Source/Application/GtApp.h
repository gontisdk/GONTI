#ifndef GtAppH
#define GtAppH

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI-ENGINE/GONTI.CORE/Source/Defines/GtDefines.inl>
        #include <EntryPoint/GtEntry.h>

        /* B8 */
        GTAPI GtB8 gontiApplicationCreate(GtEntry* entry);
        GTAPI GtB8 gontiApplicationRun(GtU64 fpsLimit, GtB8 isFpsLimit);

        /* VOID */
        GTAPI void gontiApplicationShutdown(void);
        GTAPI void gontiApplicationPause(void);
        GTAPI void gontiApplicationResume(void);
        GTAPI void gontiApplicationSuspend(void);
        GTAPI void gontiApplicationUnSuspend(void);
        GTAPI void gontiApplicationSetWidth(GtU32 width);
        GTAPI void gontiApplicationSetHeight(GtU32 height);
        GTAPI void gontiApplicationOnResized(GtU32 width, GtU32 height);

        /* GtI16 */
        GTAPI GtI16 gontiApplicationGetWidth(void);
        GTAPI GtI16 gontiApplicationGetHeight(void);

        /* BOOL/B8 */
        GTAPI GtB8 gontiApplicationIsSuspended(void);
        GTAPI GtB8 gontiApplicationIsPaused(void);

#ifdef __cplusplus
    }
#endif

#endif