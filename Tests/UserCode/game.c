#include "game.h"
#include "events.h"

// SAMPLE USER GAME INIT FUNC
b8 gameInitialize(GontiVulkanEntry* gameInst) {
    KDEBUG("gameInitialize() called!");

    gontiEventRegister(GONTI_EVENT_CODE_APPLICATION_QUIT, 0, applicationOnEvent);
    gontiEventRegister(GONTI_EVENT_CODE_APPLICATION_PAUSE, 0, applicationOnEvent);
    gontiEventRegister(GONTI_EVENT_CODE_APPLICATION_RESUME, 0, applicationOnEvent);
    gontiEventRegister(GONTI_EVENT_CODE_KEY_PRESSED, 0, applicationOnKey);
    gontiEventRegister(GONTI_EVENT_CODE_KEY_RELEASED, 0, applicationOnKey);
    gontiEventRegister(GONTI_EVENT_CODE_BUTTON_PRESSED, 0, applicationOnButton);
    gontiEventRegister(GONTI_EVENT_CODE_BUTTON_RELEASED, 0, applicationOnButton);
    gontiEventRegister(GONTI_EVENT_CODE_MOUSE_WHEEL, 0, applicationOnMouseWheel);
    gontiEventRegister(GONTI_EVENT_CODE_MOUSE_MOVED, 0, applicationOnMouseMove);
    gontiEventRegister(GONTI_EVENT_CODE_RESIZED, 0, applicationOnResized);

    return true;
}
// SAMPLE USER UPDATE FUNC
b8 gameUpdate(GontiVulkanEntry* gameInst, f32 deltaTime) {
    return true;
}
// SAMPLE USER RENDER FUNC
b8 gameRender(GontiVulkanEntry* gameInst, f32 deltaTime) {
    return true;
}

// SAMPLE USER ONRESIZE FUNC
void gameOnResize(GontiVulkanEntry* gameInst, u32 width, u32 height) {

}

// SAMPLE USER SHUTDOWN FUNC
void gameShutdown(GontiVulkanEntry* gameInst) {
    gontiEventUnregister(GONTI_EVENT_CODE_APPLICATION_QUIT, 0, applicationOnEvent);
    gontiEventUnregister(GONTI_EVENT_CODE_APPLICATION_PAUSE, 0, applicationOnEvent);
    gontiEventUnregister(GONTI_EVENT_CODE_APPLICATION_RESUME, 0, applicationOnEvent);
    gontiEventUnregister(GONTI_EVENT_CODE_KEY_PRESSED, 0, applicationOnKey);
    gontiEventUnregister(GONTI_EVENT_CODE_KEY_RELEASED, 0, applicationOnKey);
    gontiEventUnregister(GONTI_EVENT_CODE_BUTTON_PRESSED, 0, applicationOnButton);
    gontiEventUnregister(GONTI_EVENT_CODE_BUTTON_RELEASED, 0, applicationOnButton);
    gontiEventUnregister(GONTI_EVENT_CODE_MOUSE_WHEEL, 0, applicationOnMouseWheel);
    gontiEventUnregister(GONTI_EVENT_CODE_MOUSE_MOVED, 0, applicationOnMouseMove);
    gontiEventUnregister(GONTI_EVENT_CODE_RESIZED, 0, applicationOnResized);
}