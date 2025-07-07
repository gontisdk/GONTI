#include "Events.h"

#ifdef Events_ON

#include "../Core/MemorySubsystem/MemorySubsystem.h"
#include "../Containers/DynamicArray/DynamicArray.h"
#include "../Utilities/Logging/Scripts/Logger.h"

typedef struct RegisteredEvent {
    void* listener;
    PFN_onEvent callback;
}RegisteredEvent;

typedef struct EventCodeEntry {
    RegisteredEvent* events;
}EventCodeEntry;

#define MAX_MESSAGE_CODES 16384

typedef struct EventSystemState {
    EventCodeEntry registered[MAX_EVENT_CODE];
}EventSystemState;

static b8 isInitialized = false;
static EventSystemState state;

/*BOOL*/
b8 eventInitialize() {
    if (isInitialized == true) return false;

    isInitialized = false;
    k_zeroMemory(&state, sizeof(EventSystemState));

    isInitialized = true;

    KINFO("Event system initialized");
    return true;
}
b8 eventRegister(u16 code, void* listener, PFN_onEvent onEvent) {
    if (isInitialized == false) return false;

    if (state.registered[code].events == 0) state.registered[code].events = darrayCreate(RegisteredEvent);

    u64 registeredCount = darrayLength(state.registered[code].events);

    for (u64 i = 0; i < registeredCount; i++) {
        if (state.registered[code].events[i].listener == listener) {
            KWARN("TODO: WARN MESSAGE HERE");
            return false;
        }
    }

    RegisteredEvent event;
    event.listener = listener;
    event.callback = onEvent;
    darrayPush(state.registered[code].events, event);

    return true;
}
b8 eventUnregister(u16 code, void* listener, PFN_onEvent onEvent) {
    if (isInitialized == false) return false;

    if (state.registered[code].events == 0) {
        KWARN("TODO: WARN MESSAGE HERE");
        return false;
    }

    u64 registeredCount = darrayLength(state.registered[code].events);

    for (u64 i = 0; i < registeredCount; i++) {
        RegisteredEvent e = state.registered[code].events[i];

        if (e.listener == listener && e.callback == onEvent) {
            RegisteredEvent poppedEvent;
            darrayPopAt(state.registered[code].events, i, &poppedEvent);

            return true;
        }
    }

    return false;
}
b8 eventFire(u16 code, void* sender, EventContext context) {
    if (isInitialized == false) return false;

    if (state.registered[code].events == 0) return false;

    u64 registeredCount = darrayLength(state.registered[code].events);

    for (u64 i = 0; i < registeredCount; i++) {
        RegisteredEvent e = state.registered[code].events[i];

        if (e.callback == NULL) continue;

        if (e.callback(code, sender, e.listener, context)) {
            return true;
        }
    }

    return false;
}

/*VOID*/
void eventShutdown() {
    for (u16 i = 0; i < MAX_EVENT_CODE; i++) {
        if (state.registered[i].events != NULL) {
            darrayDestroy(state.registered[i].events);
            state.registered[i].events = NULL;
        }
    }

    isInitialized = false;
}

#endif