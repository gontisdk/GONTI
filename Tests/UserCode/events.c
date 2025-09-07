#include "events.h"

static bool isPaused = false;

b8 applicationOnEvent(u16 code, void* sender, void* listenerInst, GontiEventContext context) {
    switch (code) {
        case GONTI_EVENT_CODE_APPLICATION_QUIT: {
            KINFO("EVENT_CODE_APPLICATION_QUIT recived, shutting down.\n");
            gontiVkApplicationShutDown();

            return true;
        }
        case GONTI_EVENT_CODE_APPLICATION_PAUSE: {
            KINFO("EVENT_CODE_APPLICATION_PAUSE recived, paused\n");
            gontiVkApplicationPause();

            return true;
        }
        case GONTI_EVENT_CODE_APPLICATION_RESUME: {
            KINFO("EVENT_CODE_APPLICATION_PAUSE recived, resumed\n");
            gontiVkApplicationResume();

            return true;
        }
    }

    return false;
}
b8 applicationOnKey(u16 code, void* sender, void* listenerInst, GontiEventContext context) {
    if (code == GONTI_EVENT_CODE_KEY_PRESSED) {
        u16 keyCode = context.data.u16[0];

        if (keyCode == GONTI_KEY_ESCAPE) {
            if (!isPaused) {
                GontiEventContext data = {};
                gontiEventFire(GONTI_EVENT_CODE_APPLICATION_PAUSE, 0, data);
                isPaused = true;
            } else if (isPaused) {
                GontiEventContext data = {};
                gontiEventFire(GONTI_EVENT_CODE_APPLICATION_RESUME, 0, data);
                isPaused = false;
            }
            
            return true;
        } else {
            KDEBUG("\"%c\" KEY PRESSED", keyCode);
        }
    } else if (code == GONTI_EVENT_CODE_KEY_RELEASED) {
        u16 keyCode = context.data.u16[0];

        KDEBUG("\"%c\" KEY RELEASED", keyCode);
    }

    return false;
}
b8 applicationOnButton(u16 code, void* sender, void* listenerInst, GontiEventContext context) {
    if (code == GONTI_EVENT_CODE_BUTTON_PRESSED) {
        u16 buttonCode = context.data.u16[0];

        if (buttonCode == GONTI_BUTTON_LEFT) {
            KDEBUG("LEFT BUTTON PRESSED");
        } else if (buttonCode == GONTI_BUTTON_MIDDLE) {
            KDEBUG("MIDDLE BUTTON PRESSED");
        } else if (buttonCode == GONTI_BUTTON_RIGHT) {
            KDEBUG("RIGHT BUTTON PRESSED");
        }
    } else if (code == GONTI_EVENT_CODE_BUTTON_RELEASED) {
        u16 buttonCode = context.data.u16[0];

        if (buttonCode == GONTI_BUTTON_LEFT) {
            KDEBUG("LEFT BUTTON RELEASED");
        } else if (buttonCode == GONTI_BUTTON_MIDDLE) {
            KDEBUG("MIDDLE BUTTON RELEASED");
        } else if (buttonCode == GONTI_BUTTON_RIGHT) {
            KDEBUG("RIGHT BUTTON RELEASED");
        }
    }

    return false;
}
b8 applicationOnMouseWheel(u16 code, void* sender, void* listenerInst, GontiEventContext context) {
    if (code == GONTI_EVENT_CODE_MOUSE_WHEEL) {
        u16 wheelCode = context.data.u16[0];

        if (wheelCode == 1) {
            KDEBUG("WHEEL MOVED UP");
        } else if (wheelCode == 255) {
            KDEBUG("WHEEL MOVED DOWN");
        }
    }

    return false;
}
b8 applicationOnMouseMove(u16 code, void* sender, void* listenerInst, GontiEventContext context) {
    if (code == GONTI_EVENT_CODE_MOUSE_MOVED) {
        u16 mousePosX = context.data.u16[0];
        u16 mousePosY = context.data.u16[1];

        KDEBUG("Mouse position X: %i Y: %i", mousePosX, mousePosY);
    }

    return false;
}
b8 applicationOnResized(u16 code, void* sender, void* listenerInst, GontiEventContext context) {
    if (code == GONTI_EVENT_CODE_RESIZED) {
        u16 width = context.data.u16[0];
        u16 height = context.data.u16[1];

        if (width != gontiVkApplicationGetWidth() || height != gontiVkApplicationGetHeight()) {
            gontiVkApplicationWidthSet(width);
            gontiVkApplicationHeightSet(height);

            KDEBUG("Window resized %i:%i", width, height);

            if (width == 0 || height == 0) {
                KDEBUG("Window minimalized, suspending application");
                gontiVkApplicationPause();

                return true;
            } else {
                if (gontiVkApplicationIsPaused()) {
                    KDEBUG("Window restored, resuming application");
                    gontiVkApplicationResume();
                }

                gontiVkApplicationOnResized(width, height);
                gontiRendererOnResized(width, height);
            }
        }
    }

    return false;
}