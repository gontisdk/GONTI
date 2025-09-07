#include "Inputs.h"

#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Memory/Memory.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Logging/Logger.h>
#include "../Events.h"

typedef struct GontiKeyboardState {
    b8 keys[256];
}GontiKeyboardState;

typedef struct GontiMouseState {
    i16 x;
    i16 y;
    b8 buttons[BUTTON_MAX_GONTI_BUTTONS];
}GontiMouseState;

typedef struct GontiInputState {
    GontiKeyboardState keyboardCurrent;
    GontiKeyboardState keyboardPrevious;
    GontiMouseState mouseCurrent;
    GontiMouseState mousePrevious;
}GontiInputState;

static b8 initialized;
static GontiInputState state;

void gontiInputInitialize() {
    k_zeroMemory(&state, sizeof(GontiInputState));
    initialized = true;
    KINFO("Input subsystem initialized");
}
void gontiInputShutdown() {
    initialized = false;
}
void gontiInputUpdate(f64 deltaTime) {
    if (!initialized) return;

    k_copyMemory(&state.keyboardPrevious, &state.keyboardCurrent, sizeof(GontiKeyboardState));
    k_copyMemory(&state.mousePrevious, &state.mouseCurrent, sizeof(GontiMouseState));
}
void gontiInputProcessKey(GontiInputKeyboardKeys key, b8 pressed) {
    if (state.keyboardCurrent.keys[key] != pressed) {
        state.keyboardCurrent.keys[key] = pressed;

        GontiEventContext context;
        context.data.u16[0] = key;
        gontiEventFire(pressed ? GONTI_EVENT_CODE_KEY_PRESSED : GONTI_EVENT_CODE_KEY_RELEASED, 0, context);
    }
}
void gontiInputProcessButton(GontiInputMouseButtons button, b8 pressed) {
    if (state.mouseCurrent.buttons[button] != pressed) {
        state.mouseCurrent.buttons[button] = pressed;

        GontiEventContext context;
        context.data.u16[0] = button;
        gontiEventFire(pressed ? GONTI_EVENT_CODE_BUTTON_PRESSED : GONTI_EVENT_CODE_BUTTON_RELEASED, 0, context);
    }
}
void gontiInputProcessMouseMove(i16 x, i16 y) {
    if (state.mouseCurrent.x != x || state.mouseCurrent.y != y) {
        state.mouseCurrent.x = x;
        state.mouseCurrent.y = y;

        GontiEventContext context;
        context.data.u16[0] = x;
        context.data.u16[1] = y;
        gontiEventFire(GONTI_EVENT_CODE_MOUSE_MOVED, 0, context);
    }
}
void gontiInputProcessMouseWheel(i8 zDelta) {
    GontiEventContext context;
    context.data.u8[0] = zDelta;
    gontiEventFire(GONTI_EVENT_CODE_MOUSE_WHEEL, 0, context);
}
void gontiVkInputGetMousePosition(i32* x, i32* y) {
    if (!initialized) {
        *x = 0;
        *y = 0;
        return;
    }

    *x = state.mouseCurrent.x;
    *y = state.mouseCurrent.y;
}
void gontiInputGetPreviousMousePosition(i32* x, i32* y) {
    if (!initialized) {
        *x = 0;
        *y = 0;
        return;
    }

    *x = state.mousePrevious.x;
    *y = state.mousePrevious.y;
}

b8 gontiInputIsKeyDown(GontiInputKeyboardKeys key) {
    if (!initialized) return false;
    return state.keyboardCurrent.keys[key] == true;
}
b8 gontiInputIsKeyUp(GontiInputKeyboardKeys key) {
    if (!initialized) return true;
    return state.keyboardCurrent.keys[key] == false;
}
b8 gontiInputWasKeyDown(GontiInputKeyboardKeys key) {
    if (!initialized) return false;
    return state.keyboardPrevious.keys[key] == true;
}
b8 gontiInputWasKeyUp(GontiInputKeyboardKeys key) {
    if (!initialized) return true;
    return state.keyboardPrevious.keys[key] == false;
}
b8 gontiInputIsButtonDown(GontiInputMouseButtons button) {
    if (!initialized) return false;
    return state.mouseCurrent.buttons[button] == true;
}
b8 gontiInputIsButtonUp(GontiInputMouseButtons button) {
    if (!initialized) return true;
    return state.mouseCurrent.buttons[button] == false;
}
b8 gontiInputWasButtonDown(GontiInputMouseButtons button) {
    if (!initialized) return false;
    return state.mousePrevious.buttons[button] == true;
}
b8 gontiInputWasButtonUp(GontiInputMouseButtons button) {
    if (!initialized) return true;
    return state.mousePrevious.buttons[button] == false;
}