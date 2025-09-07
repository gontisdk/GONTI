#ifndef EVENTS_H
#define EVENTS_H

#ifdef __cplusplus
    extern "C" {
#endif

        #include "EventTypes.inl"

        /*BOOL*/
        KAPI b8 gontiEventInitialize();
        KAPI b8 gontiEventRegister(u16 code, void* listener, PFN_gontiOnEvent onEvent);
        KAPI b8 gontiEventUnregister(u16 code, void* listener, PFN_gontiOnEvent onEvent);
        KAPI b8 gontiEventFire(u16 code, void* sender, GontiEventContext context);

        /*VOID*/
        KAPI void gontiEventShutdown();

#ifdef __cplusplus
    }
#endif

#endif