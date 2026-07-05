#include <Events/GtEvents.h>

#include <GONTI-CORE/GONTI.CORE/Source/Memory/GtMemory.h>
#include <GONTI-CORE/GONTI.CONTAINERS/Source/DynamicArray/GtDArray.h>
#include <GONTI-CORE/GONTI.CORE/Source/Logging/GtLogger.h>

typedef struct GtRegisteredEvent {
    void* listener;
    GtOnEventFN callback;
}GtRegisteredEvent;

typedef struct GtEventCodeEntry {
    GtRegisteredEvent* events;
}GtEventCodeEntry;

#define MAX_MESSAGE_CODES 16384

typedef struct GtEventSystemState {
    GtEventCodeEntry registered[MAX_GT_EVENT_CODE];
    GtB8 isInitialized;
}GtEventSystemState;

static GtEventSystemState* statePtr;

/*BOOL*/
GtB8 gontiEventInitialize(GtU64* memoryRequirement, void* state) {
    *memoryRequirement = sizeof(GtEventSystemState);
    if (!state) return GtTrue;

    statePtr = state;
    if (statePtr->isInitialized == GtTrue) return GtFalse;

    statePtr->isInitialized = GtFalse;
    gt_zeroMemory(statePtr, sizeof(GtEventSystemState));

    statePtr->isInitialized = GtTrue;
    GTINFO("Event system initialized");
    return GtTrue;
}
GtB8 gontiEventRegister(GtU16 code, void* listener, GtOnEventFN onEvent) {
    if (!statePtr || statePtr->isInitialized == GtFalse) return GtFalse;

    if (statePtr->registered[code].events == 0) statePtr->registered[code].events = gontiDarrayCreate(GtRegisteredEvent);

    GtU64 registeredCount = gontiDarrayLength(statePtr->registered[code].events);

    for (GtU64 i = 0; i < registeredCount; i++) {
        if (statePtr->registered[code].events[i].listener == listener) {
            GTWARN("TODO: WARN MESSAGE HERE");
            return GtFalse;
        }
    }

    GtRegisteredEvent event;
    event.listener = listener;
    event.callback = onEvent;
    gontiDarrayPush(statePtr->registered[code].events, event);

    return GtTrue;
}
GtB8 gontiEventUnregister(GtU16 code, void* listener, GtOnEventFN onEvent) {
    if (!statePtr || statePtr->isInitialized == GtFalse) return GtFalse;

    if (statePtr->registered[code].events == 0) {
        GTWARN("TODO: WARN MESSAGE HERE");
        return GtFalse;
    }

    GtU64 registeredCount = gontiDarrayLength(statePtr->registered[code].events);

    for (GtU64 i = 0; i < registeredCount; i++) {
        GtRegisteredEvent e = statePtr->registered[code].events[i];

        if (e.listener == listener && e.callback == onEvent) {
            GtRegisteredEvent poppedEvent;
            gontiDarrayPopAt(statePtr->registered[code].events, i, &poppedEvent);

            return GtTrue;
        }
    }

    return GtFalse;
}
GtB8 gontiEventFire(GtU16 code, void* sender, GtEventContext context) {
    if (!statePtr || statePtr->isInitialized == GtFalse) return GtFalse;

    if (statePtr->registered[code].events == 0) return GtFalse;

    GtU64 registeredCount = gontiDarrayLength(statePtr->registered[code].events);

    for (GtU64 i = 0; i < registeredCount; i++) {
        GtRegisteredEvent e = statePtr->registered[code].events[i];

        if (e.callback == NULL) continue;

        if (e.callback(code, sender, e.listener, context)) {
            return GtTrue;
        }
    }

    return GtFalse;
}

/*VOID*/
void gontiEventShutdown(void* state) {
    for (GtU16 i = 0; i < MAX_GT_EVENT_CODE; i++) {
        if (statePtr->registered[i].events != NULL) {
            gontiDarrayDestroy(statePtr->registered[i].events);
            statePtr->registered[i].events = NULL;
        }
    }

    statePtr->isInitialized = GtFalse;
}