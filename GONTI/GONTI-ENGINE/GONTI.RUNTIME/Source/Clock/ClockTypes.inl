#ifndef CLOCKTYPES_INL
#define CLOCKTYPES_INL

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/Defines.h>

        typedef struct Clock {
            f64 startTime;
            f64 elapsed;
        }Clock;

#ifdef __cplusplus
    }
#endif

#endif