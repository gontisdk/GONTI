#ifndef GtEntryTypesH
#define GtEntryTypesH

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI-CORE/GONTI.CORE/Source/Defines/GtDefines.inl>

        typedef struct GtEntryWindowConfig {
            char* windowName;
            char* className;

            GtI16 startPosX;
            GtI16 startPosY;
            GtI16 startWidth;
            GtI16 startHeight;
        } GtEntryWindowConfig;

        typedef struct GtEntry {
            GtEntryWindowConfig windowConfig;
            GtB8 disableSystemEvents;

            GtB8 (*GtInitializeFN) (struct GtEntry* entryInst);
            GtB8 (*GtUpdateFN) (struct GtEntry* entryInst, GtF32 deltaTime);
            GtB8 (*GtFixedUpdateFN) (struct GtEntry* entryInst, GtF32 fixedDelta);
            GtB8 (*GtRenderFN) (struct GtEntry* entryInst, GtF32 deltaTime, GtF32 alpha);

            void (*GtOnResizeFN) (struct GtEntry* entryInst, GtU32 width, GtU32 height);
            void (*GtShutdownFN) (struct GtEntry* entryInst);

            void* GtState;
            void* GtAppState;
        } GtEntry;

#ifdef __cplusplus
    }
#endif

#endif