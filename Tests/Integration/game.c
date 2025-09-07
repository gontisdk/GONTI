#include "game.h"

#include "../../GONTI/Source/Utilities/Logging/Scripts/Logger.h"

#include "eventrec.h"

#if defined(USE_VULKAN)

// SAMPLE USER GAME INIT FUNC
b8 gameInitialize(gontiVkEntry* gameInst) {
    KDEBUG("gameInitialize() called!");
    return true;
}
// SAMPLE USER UPDATE FUNC
b8 gameUpdate(gontiVkEntry* gameInst, f32 deltaTime) {
    return true;
}
// SAMPLE USER RENDER FUNC
b8 gameRender(gontiVkEntry* gameInst, f32 deltaTime) {
    return true;
}


// SAMPLE USER ONRESIZE FUNC
void gameOnResize(gontiVkEntry* gameInst, u32 width, u32 height) {

}


void userEventsReg() {
    eventRegister(EVENT_CODE_APPLICATION_QUIT, 0, applicationOnEvent);
    eventRegister(EVENT_CODE_APPLICATION_PAUSE, 0, applicationOnEvent);
    eventRegister(EVENT_CODE_APPLICATION_RESUME, 0, applicationOnEvent);
    eventRegister(EVENT_CODE_KEY_PRESSED, 0, applicationOnKey);
    eventRegister(EVENT_CODE_KEY_RELEASED, 0, applicationOnKey);
    eventRegister(EVENT_CODE_BUTTON_PRESSED, 0, applicationOnButton);
    eventRegister(EVENT_CODE_BUTTON_RELEASED, 0, applicationOnButton);
    eventRegister(EVENT_CODE_MOUSE_WHEEL, 0, applicationOnMouseWheel);
    eventRegister(EVENT_CODE_MOUSE_MOVED, 0, applicationOnMouseMove);
}
void userEventUnreg() {
    eventUnregister(EVENT_CODE_APPLICATION_QUIT, 0, applicationOnEvent);
    eventUnregister(EVENT_CODE_APPLICATION_PAUSE, 0, applicationOnEvent);
    eventUnregister(EVENT_CODE_APPLICATION_RESUME, 0, applicationOnEvent);
    eventUnregister(EVENT_CODE_KEY_PRESSED, 0, applicationOnKey);
    eventUnregister(EVENT_CODE_KEY_RELEASED, 0, applicationOnKey);
    eventUnregister(EVENT_CODE_BUTTON_PRESSED, 0, applicationOnButton);
    eventUnregister(EVENT_CODE_BUTTON_RELEASED, 0, applicationOnButton);
    eventUnregister(EVENT_CODE_MOUSE_WHEEL, 0, applicationOnMouseWheel);
    eventUnregister(EVENT_CODE_MOUSE_MOVED, 0, applicationOnMouseMove);
}



// SAMPLE USER ENGINE INIT FUNC
// THIS FUNC CALL ENGINE FUNCTIONS
int initEngine(gontiVkEntry gameInst) {
    if (!gontiVkEntryPoint(&gameInst)) {
        KFATAL("Could not create game!");
        return -1;
    }

    if (!gameInst.render
        || !gameInst.update
        || !gameInst.initialize
        || !gameInst.onResize
        || !gameInst.registerEvents
        || !gameInst.unregisterEvents
        ) {
            KFATAL("The game function pointers must be assigned!");
            return -2;
    }

    if (!applicationCreate(&gameInst)) {
        KFATAL("Application failed to create!");
        return 1;
    }
       
    if (!applicationRun(30, true)) {
        KFATAL("Aplication did not shutdown gracefully!");
        return 2;
    }

    return 0;
}

#endif
#if defined(USE_DIRECTX)
// Add here code implementations for DirectX
#endif
#if defined(USE_OPENGL)
// Add here code implementations for OpenGL
#endif