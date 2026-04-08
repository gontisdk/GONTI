#include "GtPlatformEvents.h"

#if GONTI_USE_VULKAN && GTPLATFORM_LINUX

#include <X11/keysym.h>
#include <xcb/xproto.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <GONTI/GONTI-ENGINE/GONTI.RENDER.VK/Source/Platform/GtVkPlatform.h>
#include "../../Inputs/GtInputs.h"
#include "../GtEvents.h"

GtB8 LinuxProc(void* platState) {
    GtVkPlatformState* state = (GtVkPlatformState*)platState;

    xcb_generic_event_t* event = state->gontiVkInternalStateLinux.event;
    xcb_client_message_event_t* cm = state->gontiVkInternalStateLinux.clientMessage;

    switch (event->response_type & ~0x80) {
        case XCB_KEY_PRESS:
        case XCB_KEY_RELEASE: {
            xcb_key_press_event_t* kbEvent = (xcb_key_press_event_t*)event;
            GtB8 pressed = event->response_type == XCB_KEY_PRESS;
            xcb_keycode_t code = kbEvent->detail;
            KeySym keySym = XkbKeycodeToKeysym(
                state->gontiVkInternalStateLinux.display,
                (KeyCode)code,
                0,
                code & ShiftMask ? 1 : 0
            );

            GtInputKeyboardKeys key = translateKeycode(keySym);
            gontiInputProcessKey(key, pressed);
        }break;
        case XCB_BUTTON_PRESS:
        case XCB_BUTTON_RELEASE: {
            xcb_button_press_event_t* mouseEvent = (xcb_button_press_event_t*)event;
            GtB8 pressed = event->response_type == XCB_BUTTON_PRESS;
            GtInputMouseBtns mButton = BUTTON_MAX_GT_BTNS;

            switch (mouseEvent->detail) {
                case XCB_BUTTON_INDEX_1:
                    mButton = GT_BTN_LEFT;
                    break;
                case XCB_BUTTON_INDEX_2:
                    mButton = GT_BTN_MIDDLE;
                    break;
                case XCB_BUTTON_INDEX_3:
                    mButton = GT_BTN_RIGHT;
                    break;
            }

            if (mButton != BUTTON_MAX_GT_BTNS) {
                gontiInputProcessButton(mButton, pressed);
            }
        }break;
        case XCB_MOTION_NOTIFY: {
            xcb_motion_notify_event_t* moveEvent = (xcb_motion_notify_event_t*)event;
            gontiInputProcessMouseMove(moveEvent->event_x, moveEvent->event_y);
        }break;
        case XCB_CONFIGURE_NOTIFY: {
            xcb_configure_notify_event_t* configureEvent = (xcb_configure_notify_event_t*)event;

            GtEventContext context;
            context.data.GtU16[0] = configureEvent->width;
            context.data.GtU16[1] = configureEvent->height;
            gontiEventFire(GT_EVENT_CODE_RESIZED, 0, context);
        }break;
        case XCB_CLIENT_MESSAGE: {
            cm = (xcb_client_message_event_t*)event;

            if (cm->data.data32[0] == state->gontiVkInternalStateLinux.wmDeleteWin) return GtFalse;
        }break;
        default: break;
    }

    return GtTrue;
}

GtInputKeyboardKeys translateKeycode(GtU32 xKeyCode) {
    switch (xKeyCode) {
        case XK_BackSpace:
            return GT_KEY_BACKSPACE;
        case XK_Return:
            return GT_KEY_ENTER;
        case XK_Tab:
            return GT_KEY_TAB;
            // case XK_Shift: return GT_KEY_SHIFT;
            // case XK_Control: return GT_KEY_CONTROL;

        case XK_Pause:
            return GT_KEY_PAUSE;
        case XK_Caps_Lock:
            return GT_KEY_CAPITAL;

        case XK_Escape:
            return GT_KEY_ESCAPE;

            // Not supported
            // case : return GT_KEY_CONVERT;
            // case : return GT_KEY_NONCONVERT;
            // case : return GT_KEY_ACCEPT;

        case XK_Mode_switch:
            return GT_KEY_MODECHANGE;

        case XK_space:
            return GT_KEY_SPACE;
        case XK_Prior:
            return GT_KEY_PAGEUP;
        case XK_Next:
            return GT_KEY_PAGEDOWN;
        case XK_End:
            return GT_KEY_END;
        case XK_Home:
            return GT_KEY_HOME;
        case XK_Left:
            return GT_KEY_LEFT;
        case XK_Up:
            return GT_KEY_UP;
        case XK_Right:
            return GT_KEY_RIGHT;
        case XK_Down:
            return GT_KEY_DOWN;
        case XK_Select:
            return GT_KEY_SELECT;
        case XK_Print:
            return GT_KEY_PRINT;
        case XK_Execute:
            return GT_KEY_EXECUTE;
        // case XK_snapshot: return GT_KEY_SNAPSHOT; // not supported
        case XK_Insert:
            return GT_KEY_INSERT;
        case XK_Delete:
            return GT_KEY_DELETE;
        case XK_Help:
            return GT_KEY_HELP;

        case XK_Meta_L:
        case XK_Super_L:
            // Treat the "meta" key (if mapped) as super
            return GT_KEY_LSUPER;
        case XK_Meta_R:
        case XK_Super_R:
            // Treat the "meta" key (if mapped) as super
            return GT_KEY_RSUPER;
            // case XK_apps: return GT_KEY_APPS; // not supported

            // case XK_sleep: return GT_KEY_SLEEP; //not supported

        case XK_KP_0:
            return GT_KEY_NUMPAD0;
        case XK_KP_1:
            return GT_KEY_NUMPAD1;
        case XK_KP_2:
            return GT_KEY_NUMPAD2;
        case XK_KP_3:
            return GT_KEY_NUMPAD3;
        case XK_KP_4:
            return GT_KEY_NUMPAD4;
        case XK_KP_5:
            return GT_KEY_NUMPAD5;
        case XK_KP_6:
            return GT_KEY_NUMPAD6;
        case XK_KP_7:
            return GT_KEY_NUMPAD7;
        case XK_KP_8:
            return GT_KEY_NUMPAD8;
        case XK_KP_9:
            return GT_KEY_NUMPAD9;
        case XK_multiply:
            return GT_KEY_MULTIPLY;
        case XK_KP_Add:
            return GT_KEY_ADD;
        case XK_KP_Separator:
            return GT_KEY_SEPARATOR;
        case XK_KP_Subtract:
            return GT_KEY_SUBTRACT;
        case XK_KP_Decimal:
            return GT_KEY_DECIMAL;
        case XK_KP_Divide:
            return GT_KEY_DIVIDE;
        case XK_F1:
            return GT_KEY_F1;
        case XK_F2:
            return GT_KEY_F2;
        case XK_F3:
            return GT_KEY_F3;
        case XK_F4:
            return GT_KEY_F4;
        case XK_F5:
            return GT_KEY_F5;
        case XK_F6:
            return GT_KEY_F6;
        case XK_F7:
            return GT_KEY_F7;
        case XK_F8:
            return GT_KEY_F8;
        case XK_F9:
            return GT_KEY_F9;
        case XK_F10:
            return GT_KEY_F10;
        case XK_F11:
            return GT_KEY_F11;
        case XK_F12:
            return GT_KEY_F12;
        case XK_F13:
            return GT_KEY_F13;
        case XK_F14:
            return GT_KEY_F14;
        case XK_F15:
            return GT_KEY_F15;
        case XK_F16:
            return GT_KEY_F16;
        case XK_F17:
            return GT_KEY_F17;
        case XK_F18:
            return GT_KEY_F18;
        case XK_F19:
            return GT_KEY_F19;
        case XK_F20:
            return GT_KEY_F20;
        case XK_F21:
            return GT_KEY_F21;
        case XK_F22:
            return GT_KEY_F22;
        case XK_F23:
            return GT_KEY_F23;
        case XK_F24:
            return GT_KEY_F24;

        case XK_Num_Lock:
            return GT_KEY_NUMLOCK;
        case XK_Scroll_Lock:
            return GT_KEY_SCROLL;

        case XK_KP_Equal:
            return GT_KEY_NUMPAD_EQUAL;

        case XK_Shift_L:
            return GT_KEY_LSHIFT;
        case XK_Shift_R:
            return GT_KEY_RSHIFT;
        case XK_Control_L:
            return GT_KEY_LCONTROL;
        case XK_Control_R:
            return GT_KEY_RCONTROL;
        case XK_Alt_L:
            return GT_KEY_LALT;
        case XK_Alt_R:
            return GT_KEY_RALT;

        case XK_semicolon:
            return GT_KEY_SEMICOLON;
        case XK_plus:
            return GT_KEY_EQUAL;
        case XK_comma:
            return GT_KEY_COMMA;
        case XK_minus:
            return GT_KEY_MINUS;
        case XK_period:
            return GT_KEY_PERIOD;
        case XK_slash:
            return GT_KEY_SLASH;
        case XK_grave:
            return GT_KEY_GRAVE;

        case XK_0:
            return GT_KEY_0;
        case XK_1:
            return GT_KEY_1;
        case XK_2:
            return GT_KEY_2;
        case XK_3:
            return GT_KEY_3;
        case XK_4:
            return GT_KEY_4;
        case XK_5:
            return GT_KEY_5;
        case XK_6:
            return GT_KEY_6;
        case XK_7:
            return GT_KEY_7;
        case XK_8:
            return GT_KEY_8;
        case XK_9:
            return GT_KEY_9;

        case XK_a:
        case XK_A:
            return GT_KEY_A;
        case XK_b:
        case XK_B:
            return GT_KEY_B;
        case XK_c:
        case XK_C:
            return GT_KEY_C;
        case XK_d:
        case XK_D:
            return GT_KEY_D;
        case XK_e:
        case XK_E:
            return GT_KEY_E;
        case XK_f:
        case XK_F:
            return GT_KEY_F;
        case XK_g:
        case XK_G:
            return GT_KEY_G;
        case XK_h:
        case XK_H:
            return GT_KEY_H;
        case XK_i:
        case XK_I:
            return GT_KEY_I;
        case XK_j:
        case XK_J:
            return GT_KEY_J;
        case XK_k:
        case XK_K:
            return GT_KEY_K;
        case XK_l:
        case XK_L:
            return GT_KEY_L;
        case XK_m:
        case XK_M:
            return GT_KEY_M;
        case XK_n:
        case XK_N:
            return GT_KEY_N;
        case XK_o:
        case XK_O:
            return GT_KEY_O;
        case XK_p:
        case XK_P:
            return GT_KEY_P;
        case XK_q:
        case XK_Q:
            return GT_KEY_Q;
        case XK_r:
        case XK_R:
            return GT_KEY_R;
        case XK_s:
        case XK_S:
            return GT_KEY_S;
        case XK_t:
        case XK_T:
            return GT_KEY_T;
        case XK_u:
        case XK_U:
            return GT_KEY_U;
        case XK_v:
        case XK_V:
            return GT_KEY_V;
        case XK_w:
        case XK_W:
            return GT_KEY_W;
        case XK_x:
        case XK_X:
            return GT_KEY_X;
        case XK_y:
        case XK_Y:
            return GT_KEY_Y;
        case XK_z:
        case XK_Z:
            return GT_KEY_Z;

        default:
            return 0;
        }
}

#endif