#ifndef GtSystemEventsH
#define GtSystemEventsH

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI-CORE/GONTI.CORE/Source/Defines/GtDefines.inl>
        #include <Events/GtEvents.h>

        GTAPI GtB8 gontiSystemEventsOnKey(GtU16 code, void* sender, void* listenerInst, GtEventContext context);
        GTAPI GtB8 gontiSystemEventsOnButton(GtU16 code, void* sender, void* listenerInst, GtEventContext context);
        GTAPI GtB8 gontiSystemEventsOnMouseWheel(GtU16 code, void* sender, void* listenerInst, GtEventContext context);
        GTAPI GtB8 gontiSystemEventsOnMouseMove(GtU16 code, void* sender, void* listenerInst, GtEventContext context);
        GTAPI GtB8 gontiSystemEventsOnResized(GtU16 code, void* sender, void* listenerInst, GtEventContext context);

#ifdef __cplusplus
    }
#endif

#endif