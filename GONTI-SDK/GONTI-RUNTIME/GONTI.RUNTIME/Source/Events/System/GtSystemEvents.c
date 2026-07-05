#include <Events/System/GtSystemEvents.h>

#include <Inputs/GtInputs.h>
#include <Application/GtApp.h>
#include <GONTI-CORE/GONTI.CORE/Source/Logging/GtLogger.h>
#include <GONTI-RENDER/GONTI.RENDER/Source/Renderer/Frontend/GtRFrontend.h>

GtB8 gontiSystemEventsOnKey(GtU16 code, void* sender, void* listenerInst, GtEventContext context) {
    if (code == GT_EVENT_CODE_KEY_PRESSED) {
        GtU16 keyCode = context.data.GtU16[0];
        GTDEBUG("\"%c\" KEY PRESSED", keyCode);

        return GtTrue;
    } else if (code == GT_EVENT_CODE_KEY_RELEASED) {
        GtU16 keyCode = context.data.GtU16[0];
        GTDEBUG("\"%c\" KEY RELEASED", keyCode);

        return GtTrue;
    }

    return GtFalse;
}
GtB8 gontiSystemEventsOnButton(GtU16 code, void* sender, void* listenerInst, GtEventContext context) {
    if (code == GT_EVENT_CODE_BUTTON_PRESSED) {
        GtU16 buttonCode = context.data.GtU16[0];

        if (buttonCode == GT_BTN_LEFT) {
            GTDEBUG("LEFT BUTTON PRESSED");
        } else if (buttonCode == GT_BTN_MIDDLE) {
            GTDEBUG("MIDDLE BUTTON PRESSED");
        } else if (buttonCode == GT_BTN_RIGHT) {
            GTDEBUG("RIGHT BUTTON PRESSED");
        }
    } else if (code == GT_EVENT_CODE_BUTTON_RELEASED) {
        GtU16 buttonCode = context.data.GtU16[0];

        if (buttonCode == GT_BTN_LEFT) {
            GTDEBUG("LEFT BUTTON RELEASED");
        } else if (buttonCode == GT_BTN_MIDDLE) {
            GTDEBUG("MIDDLE BUTTON RELEASED");
        } else if (buttonCode == GT_BTN_RIGHT) {
            GTDEBUG("RIGHT BUTTON RELEASED");
        }
    }

    return GtFalse;
}
GtB8 gontiSystemEventsOnMouseWheel(GtU16 code, void* sender, void* listenerInst, GtEventContext context) {
    if (code == GT_EVENT_CODE_MOUSE_WHEEL) {
        GtU16 wheelCode = context.data.GtU16[0];

        if (wheelCode == 1) {
            GTDEBUG("WHEEL MOVED UP");
        } else if (wheelCode == 255) {
            GTDEBUG("WHEEL MOVED DOWN");
        }
    }

    return GtFalse;
}
GtB8 gontiSystemEventsOnMouseMove(GtU16 code, void* sender, void* listenerInst, GtEventContext context) {
    if (code == GT_EVENT_CODE_MOUSE_MOVED) {
        GtU16 mousePosX = context.data.GtU16[0];
        GtU16 mousePosY = context.data.GtU16[1];

        GTDEBUG("Mouse position X: %i Y: %i", mousePosX, mousePosY);
    }

    return GtFalse;
}
GtB8 gontiSystemEventsOnResized(GtU16 code, void* sender, void* listenerInst, GtEventContext context) {
    if (code == GT_EVENT_CODE_RESIZED) {
        GtU16 width = context.data.GtU16[0];
        GtU16 height = context.data.GtU16[1];

        if (width != gontiApplicationGetWidth() || height != gontiApplicationGetHeight()) {
            gontiApplicationSetWidth(width);
            gontiApplicationSetHeight(height);

            GTDEBUG("Window resized %i:%i", width, height);

            if (width == 0 || height == 0) {
                GTDEBUG("Window minimalized, suspending application");
                gontiApplicationSuspend();

                return GtTrue;
            } else {
                if (gontiApplicationIsSuspended()) {
                    GTDEBUG("Window restored, resuming application");
                    gontiApplicationUnSuspend();
                }

                gontiApplicationOnResized(width, height);
                gontiRendererOnResized(width, height);
            }
        }
    }

    return GtFalse;
}