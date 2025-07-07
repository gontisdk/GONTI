#ifndef CLOCK_H
#define CLOCK_H

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../../../__GONTI_INCLUDES.h"
        #ifdef USE_VULKAN
        #ifdef DeltaTime_ON

            typedef struct Clock {
                f64 startTime;
                f64 elapsed;
            }Clock;

            KAPI void clockUpdate(Clock* clock);
            KAPI void clockStart(Clock* clock);
            KAPI void clockStop(Clock* clock);

        #endif
        #endif

#ifdef __cplusplus
    }
#endif

#endif