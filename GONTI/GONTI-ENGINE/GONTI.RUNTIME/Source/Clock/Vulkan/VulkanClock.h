#ifndef VULKANCLOCK_H
#define VULKANCLOCK_H

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI/GONTI-ENGINE/GONTI.RENDER/Source/Config/SetupGraphicBackend/UserSetGraphicBackend.h>
        #if GONTI_USE_VULKAN

            #include "../ClockTypes.inl"

            KAPI void gontiVkClockUpdate(Clock* clock);
            KAPI void gontiVkClockStart(Clock* clock);
            KAPI void gontiVkClockStop(Clock* clock);

        #endif

#ifdef __cplusplus
    }
#endif

#endif