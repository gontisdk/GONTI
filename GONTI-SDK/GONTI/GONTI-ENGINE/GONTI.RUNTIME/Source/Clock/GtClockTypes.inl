#ifndef GtClockTypesINL
#define GtClockTypesINL

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/GtDefines.h>

        typedef struct GtClock {
            GtF64 startTime;
            GtF64 elapsed;
        }GtClock;

#ifdef __cplusplus
    }
#endif

#endif