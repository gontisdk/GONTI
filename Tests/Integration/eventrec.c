#include "eventrec.h"

#if defined(USE_VULKAN)

static bool isPaused = false;

b8 applicationOnEvent(u16 code, void* sender, void* listenerInst, EventContext context) {
    switch (code) {
        case EVENT_CODE_APPLICATION_QUIT: {
            KINFO("EVENT_CODE_APPLICATION_QUIT recived, shutting down.\n");
            applicationShutDown();

            return true;
        }
        case EVENT_CODE_APPLICATION_PAUSE: {
            KINFO("EVENT_CODE_APPLICATION_PAUSE recived, paused\n");
            applicationPause();

            return true;
        }
        case EVENT_CODE_APPLICATION_RESUME: {
            KINFO("EVENT_CODE_APPLICATION_PAUSE recived, resumed\n");
            applicationResume();

            return true;
        }
    }

    return false;
}
b8 applicationOnKey(u16 code, void* sender, void* listenerInst, EventContext context) {
    if (code == EVENT_CODE_KEY_PRESSED) {
        u16 keyCode = context.data.u16[0];

        if (keyCode == KEY_ESCAPE) {
            if (!isPaused) {
                EventContext data = {};
                eventFire(EVENT_CODE_APPLICATION_PAUSE, 0, data);
                isPaused = true;
            } else if (isPaused) {
                EventContext data = {};
                eventFire(EVENT_CODE_APPLICATION_RESUME, 0, data);
                isPaused = false;
            }
            
            return true;
        } else {
            KDEBUG("%c KEY PRESSED", keyCode);
        }
    } else if (code == EVENT_CODE_KEY_RELEASED) {
        u16 keyCode = context.data.u16[0];

        KDEBUG("%c KEY RELEASED", keyCode);
    }

    return false;
}
b8 applicationOnButton(u16 code, void* sender, void* listenerInst, EventContext context) {
    if (code == EVENT_CODE_BUTTON_PRESSED) {
        u16 buttonCode = context.data.u16[0];

        if (buttonCode == BUTTON_LEFT) {
            KDEBUG("LEFT BUTTON PRESSED");
        } else if (buttonCode == BUTTON_MIDDLE) {
            KDEBUG("MIDDLE BUTTON PRESSED");
        } else if (buttonCode == BUTTON_RIGHT) {
            KDEBUG("RIGHT BUTTON PRESSED");
        }
    } else if (code == EVENT_CODE_BUTTON_RELEASED) {
        u16 buttonCode = context.data.u16[0];

        if (buttonCode == BUTTON_LEFT) {
            KDEBUG("LEFT BUTTON RELEASED");
        } else if (buttonCode == BUTTON_MIDDLE) {
            KDEBUG("MIDDLE BUTTON RELEASED");
        } else if (buttonCode == BUTTON_RIGHT) {
            KDEBUG("RIGHT BUTTON RELEASED");
        }
    }

    return false;
}
b8 applicationOnMouseWheel(u16 code, void* sender, void* listenerInst, EventContext context) {
    if (code == EVENT_CODE_MOUSE_WHEEL) {
        u16 wheelCode = context.data.u16[0];

        if (wheelCode == 1) {
            KDEBUG("WHEEL MOVED UP");
        } else if (wheelCode == 255) {
            KDEBUG("WHEEL MOVED DOWN");
        }
    }

    return false;
}
b8 applicationOnMouseMove(u16 code, void* sender, void* listenerInst, EventContext context) {
    if (code == EVENT_CODE_MOUSE_MOVED) {
        u16 mousePosX = context.data.u16[0];
        u16 mousePosY = context.data.u16[1];

        KDEBUG("Mouse position X: %i Y: %i", mousePosX, mousePosY);
    }

    return false;
}

#endif
#if defined(USE_DIRECTX)
// Add here callback implementations for DirectX
#endif
#if defined(USE_OPENGL)
// Add here callback implementations for OpenGL
#endif