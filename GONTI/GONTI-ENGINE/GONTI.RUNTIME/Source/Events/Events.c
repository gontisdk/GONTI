#include "Events.h"

#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Memory/Memory.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Containers/DynamicArray/DynamicArray.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Logging/Logger.h>

typedef struct GontiRegisteredEvent {
    void* listener;
    PFN_gontiOnEvent callback;
}GontiRegisteredEvent;

typedef struct GontiEventCodeEntry {
    GontiRegisteredEvent* events;
}GontiEventCodeEntry;

#define MAX_MESSAGE_CODES 16384

typedef struct GontiEventSystemState {
    GontiEventCodeEntry registered[MAX_GONTI_EVENT_CODE];
}GontiEventSystemState;

static b8 isInitialized = false;
static GontiEventSystemState state;

/*BOOL*/
b8 gontiEventInitialize() {
    if (isInitialized == true) return false;

    isInitialized = false;
    k_zeroMemory(&state, sizeof(GontiEventSystemState));

    isInitialized = true;

    KINFO("Event system initialized");
    return true;
}
b8 gontiEventRegister(u16 code, void* listener, PFN_gontiOnEvent onEvent) {
    if (isInitialized == false) return false;

    if (state.registered[code].events == 0) state.registered[code].events = darrayCreate(GontiRegisteredEvent);

    u64 registeredCount = darrayLength(state.registered[code].events);

    for (u64 i = 0; i < registeredCount; i++) {
        if (state.registered[code].events[i].listener == listener) {
            KWARN("TODO: WARN MESSAGE HERE");
            return false;
        }
    }

    GontiRegisteredEvent event;
    event.listener = listener;
    event.callback = onEvent;
    darrayPush(state.registered[code].events, event);

    return true;
}
b8 gontiEventUnregister(u16 code, void* listener, PFN_gontiOnEvent onEvent) {
    if (isInitialized == false) return false;

    if (state.registered[code].events == 0) {
        KWARN("TODO: WARN MESSAGE HERE");
        return false;
    }

    u64 registeredCount = darrayLength(state.registered[code].events);

    for (u64 i = 0; i < registeredCount; i++) {
        GontiRegisteredEvent e = state.registered[code].events[i];

        if (e.listener == listener && e.callback == onEvent) {
            GontiRegisteredEvent poppedEvent;
            darrayPopAt(state.registered[code].events, i, &poppedEvent);

            return true;
        }
    }

    return false;
}
b8 gontiEventFire(u16 code, void* sender, GontiEventContext context) {
    if (isInitialized == false) return false;

    if (state.registered[code].events == 0) return false;

    u64 registeredCount = darrayLength(state.registered[code].events);

    for (u64 i = 0; i < registeredCount; i++) {
        GontiRegisteredEvent e = state.registered[code].events[i];

        if (e.callback == NULL) continue;

        if (e.callback(code, sender, e.listener, context)) {
            return true;
        }
    }

    return false;
}

/*VOID*/
void gontiEventShutdown() {
    for (u16 i = 0; i < MAX_GONTI_EVENT_CODE; i++) {
        if (state.registered[i].events != NULL) {
            darrayDestroy(state.registered[i].events);
            state.registered[i].events = NULL;
        }
    }

    isInitialized = false;
}