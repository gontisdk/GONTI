#ifndef EVENTS_H
#define EVENTS_H

#ifdef __cplusplus
    extern "C" {
#endif

        #include "GtEventTypes.inl"

        /*BOOL*/
        GTAPI GtB8 gontiEventInitialize(GtU64* memoryRequirement, void* state);
        GTAPI GtB8 gontiEventRegister(GtU16 code, void* listener, GtOnEventFN onEvent);
        GTAPI GtB8 gontiEventUnregister(GtU16 code, void* listener, GtOnEventFN onEvent);
        GTAPI GtB8 gontiEventFire(GtU16 code, void* sender, GtEventContext context);

        /*VOID*/
        GTAPI void gontiEventShutdown(void* state);

#ifdef __cplusplus
    }
#endif

#endif