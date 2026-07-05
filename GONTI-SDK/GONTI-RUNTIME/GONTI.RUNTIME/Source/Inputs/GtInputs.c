#include <Inputs/GtInputs.h>

#include <GONTI-CORE/GONTI.CORE/Source/Memory/GtMemory.h>
#include <GONTI-CORE/GONTI.CORE/Source/Logging/GtLogger.h>
#include <Events/GtEvents.h>

typedef struct GtKeyboardState {
    GtB8 keys[256];
}GtKeyboardState;

typedef struct GtMouseState {
    GtI16 x;
    GtI16 y;
    GtB8 buttons[BUTTON_MAX_GT_BTNS];
}GtMouseState;

typedef struct GtInputState {
    GtKeyboardState keyboardCurrent;
    GtKeyboardState keyboardPrevious;
    GtMouseState mouseCurrent;
    GtMouseState mousePrevious;
    GtB8 initialized;
}GtInputState;

static GtInputState* statePtr;

void gontiInputInitialize(GtU64* memoryRequirement, void* state) {
    *memoryRequirement = sizeof(GtInputState);
    if (!state) return;

    statePtr = state;
    gt_zeroMemory(statePtr, sizeof(GtInputState));
    statePtr->initialized = GtTrue;
    GTINFO("Input subsystem initialized");
}
void gontiInputShutdown(void* state) {
    statePtr->initialized = GtFalse;
    statePtr = 0;
}
void gontiInputUpdate(GtF64 deltaTime) {
    if (!statePtr ||!statePtr->initialized) return;

    gt_copyMemory(&statePtr->keyboardPrevious, &statePtr->keyboardCurrent, sizeof(GtKeyboardState));
    gt_copyMemory(&statePtr->mousePrevious, &statePtr->mouseCurrent, sizeof(GtMouseState));
}
void gontiInputProcessKey(GtInputKeyboardKeys key, GtB8 pressed) {
    if (!statePtr || statePtr->keyboardCurrent.keys[key] != pressed) {
        statePtr->keyboardCurrent.keys[key] = pressed;
        
        GtEventContext context;
        context.data.GtU16[0] = key;
        gontiEventFire(pressed ? GT_EVENT_CODE_KEY_PRESSED : GT_EVENT_CODE_KEY_RELEASED, 0, context);
    }
}
void gontiInputProcessButton(GtInputMouseBtns button, GtB8 pressed) {
    if (!statePtr || statePtr->mouseCurrent.buttons[button] != pressed) {
        statePtr->mouseCurrent.buttons[button] = pressed;

        GtEventContext context;
        context.data.GtU16[0] = button;
        gontiEventFire(pressed ? GT_EVENT_CODE_BUTTON_PRESSED : GT_EVENT_CODE_BUTTON_RELEASED, 0, context);
    }
}
void gontiInputProcessMouseMove(GtI16 x, GtI16 y) {
    if (!statePtr || statePtr->mouseCurrent.x != x || statePtr->mouseCurrent.y != y) {
        statePtr->mouseCurrent.x = x;
        statePtr->mouseCurrent.y = y;

        GtEventContext context;
        context.data.GtU16[0] = x;
        context.data.GtU16[1] = y;
        gontiEventFire(GT_EVENT_CODE_MOUSE_MOVED, 0, context);
    }
}
void gontiInputProcessMouseWheel(GtI8 zDelta) {
    GtEventContext context;
    context.data.GtU8[0] = zDelta;
    gontiEventFire(GT_EVENT_CODE_MOUSE_WHEEL, 0, context);
}
void gontiVkInputGetMousePosition(GtI32* x, GtI32* y) {
    if (!statePtr || !statePtr->initialized) {
        *x = 0;
        *y = 0;
        return;
    }

    *x = statePtr->mouseCurrent.x;
    *y = statePtr->mouseCurrent.y;
}
void gontiInputGetPreviousMousePosition(GtI32* x, GtI32* y) {
    if (!statePtr || !statePtr->initialized) {
        *x = 0;
        *y = 0;
        return;
    }

    *x = statePtr->mousePrevious.x;
    *y = statePtr->mousePrevious.y;
}

GtB8 gontiInputIsKeyDown(GtInputKeyboardKeys key) {
    if (!statePtr || !statePtr->initialized) return GtFalse;
    return statePtr->keyboardCurrent.keys[key] == GtTrue;
}
GtB8 gontiInputIsKeyUp(GtInputKeyboardKeys key) {
    if (!statePtr || !statePtr->initialized) return GtTrue;
    return statePtr->keyboardCurrent.keys[key] == GtFalse;
}
GtB8 gontiInputWasKeyDown(GtInputKeyboardKeys key) {
    if (!statePtr || !statePtr->initialized) return GtFalse;
    return statePtr->keyboardPrevious.keys[key] == GtTrue;
}
GtB8 gontiInputWasKeyUp(GtInputKeyboardKeys key) {
    if (!statePtr || !statePtr->initialized) return GtTrue;
    return statePtr->keyboardPrevious.keys[key] == GtFalse;
}
GtB8 gontiInputIsButtonDown(GtInputMouseBtns button) {
    if (!statePtr || !statePtr->initialized) return GtFalse;
    return statePtr->mouseCurrent.buttons[button] == GtTrue;
}
GtB8 gontiInputIsButtonUp(GtInputMouseBtns button) {
    if (!statePtr || !statePtr->initialized) return GtTrue;
    return statePtr->mouseCurrent.buttons[button] == GtFalse;
}
GtB8 gontiInputWasButtonDown(GtInputMouseBtns button) {
    if (!statePtr || !statePtr->initialized) return GtFalse;
    return statePtr->mousePrevious.buttons[button] == GtTrue;
}
GtB8 gontiInputWasButtonUp(GtInputMouseBtns button) {
    if (!statePtr || !statePtr->initialized) return GtTrue;
    return statePtr->mousePrevious.buttons[button] == GtFalse;
}