#include "VulkanInputs.h"

#ifdef USE_VULKAN
#ifdef InputSystemVulkan_ON

#include "../../Events.h"
#include "../../../Core/MemorySubsystem/MemorySubsystem.h"
#include "../../../Utilities/Logging/Scripts/Logger.h"

typedef struct KeyboardState {
    b8 keys[256];
}KeyboardState;

typedef struct MouseState {
    i16 x;
    i16 y;
    b8 buttons[BUTTON_MAX_BUTTONS];
}MouseState;

typedef struct InputState {
    KeyboardState keyboardCurrent;
    KeyboardState keyboardPrevious;
    MouseState mouseCurrent;
    MouseState mousePrevious;
}InputState;

static b8 initialized;
static InputState state;

void inputInitialize() {
    k_zeroMemory(&state, sizeof(InputState));
    initialized = true;
    KINFO("Input subsystem initialized");
}
void inputShutdown() {
    initialized = false;
}
void inputUpdate(f64 deltaTime) {
    if (!initialized) return;

    k_copyMemory(&state.keyboardPrevious, &state.keyboardCurrent, sizeof(KeyboardState));
    k_copyMemory(&state.mousePrevious, &state.mouseCurrent, sizeof(MouseState));
}
void inputProcessKey(InputKeyboardKeys key, b8 pressed) {
    if (state.keyboardCurrent.keys[key] != pressed) {
        state.keyboardCurrent.keys[key] = pressed;

        EventContext context;
        context.data.u16[0] = key;
        //KDEBUG("Key: %d Action: %d", key, pressed);
        eventFire(pressed ? EVENT_CODE_KEY_PRESSED : EVENT_CODE_KEY_RELEASED, 0, context);
    }
}
void inputProcessButton(InputMouseButtons button, b8 pressed) {
    if (state.mouseCurrent.buttons[button] != pressed) {
        state.mouseCurrent.buttons[button] = pressed;

        EventContext context;
        context.data.u16[0] = button;
        //KDEBUG("Button: %d Action: %d", button, pressed);
        eventFire(pressed ? EVENT_CODE_BUTTON_PRESSED : EVENT_CODE_BUTTON_RELEASED, 0, context);
    }
}
void inputProcessMouseMove(i16 x, i16 y) {
    if (state.mouseCurrent.x != x || state.mouseCurrent.y != y) {
        state.mouseCurrent.x = x;
        state.mouseCurrent.y = y;

        EventContext context;
        context.data.u16[0] = x;
        context.data.u16[1] = y;
        //KDEBUG("Mouse pos: %i, %i!", x, y);
        eventFire(EVENT_CODE_MOUSE_MOVED, 0, context);
    }
}
void inputProcessMouseWheel(i8 zDelta) {
    EventContext context;
    context.data.u8[0] = zDelta;
    eventFire(EVENT_CODE_MOUSE_WHEEL, 0, context);
}
void inputGetMousePosition(i32* x, i32* y) {
    if (!initialized) {
        *x = 0;
        *y = 0;
        return;
    }

    *x = state.mouseCurrent.x;
    *y = state.mouseCurrent.y;
}
void inputGetPreviousMousePosition(i32* x, i32* y) {
    if (!initialized) {
        *x = 0;
        *y = 0;
        return;
    }

    *x = state.mousePrevious.x;
    *y = state.mousePrevious.y;
}

b8 inputIsKeyDown(InputKeyboardKeys key) {
    if (!initialized) return false;
    return state.keyboardCurrent.keys[key] == true;
}
b8 inputISKeyUp(InputKeyboardKeys key) {
    if (!initialized) return true;
    return state.keyboardCurrent.keys[key] == false;
}
b8 inputWasKeyDown(InputKeyboardKeys key) {
    if (!initialized) return false;
    return state.keyboardPrevious.keys[key] == true;
}
b8 inputWasKeyUp(InputKeyboardKeys key) {
    if (!initialized) return true;
    return state.keyboardPrevious.keys[key] == false;
}
b8 inputIsButtonDown(InputMouseButtons button) {
    if (!initialized) return false;
    return state.mouseCurrent.buttons[button] == true;
}
b8 inputIsButtonUp(InputMouseButtons button) {
    if (!initialized) return true;
    return state.mouseCurrent.buttons[button] == false;
}
b8 inputWasButtonDown(InputMouseButtons button) {
    if (!initialized) return false;
    return state.mousePrevious.buttons[button] == true;
}
b8 inputWasButtonUp(InputMouseButtons button) {
    if (!initialized) return true;
    return state.mousePrevious.buttons[button] == false;
}

#endif
#endif