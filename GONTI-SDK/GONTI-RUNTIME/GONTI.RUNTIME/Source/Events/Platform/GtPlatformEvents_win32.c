#include <Events/Platform/GtPlatformEvents.h>

#if GTPLATFORM_WINDOWS

#include <windowsx.h>
#include <Inputs/GtInputs.h>
#include <Events/GtEvents.h>

LRESULT CALLBACK WndProc(HWND hwnd, GtU32 msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
            case WM_ERASEBKGND: {
                return 1;
            } // NOTE: Do not have to add break because you have return 1
            case WM_CLOSE: {
                GtEventContext data = {};
                gontiEventFire(GT_EVENT_CODE_APPLICATION_QUIT, 0, data);
                return 0;
            } // NOTE: Do not have to add break because you have return 0
            case WM_DESTROY: {
                PostQuitMessage(0);
                return 0;
            } // NOTE: Do not have to add break because you have return 0
            case WM_SIZE: {
                RECT r;
                GetClientRect(hwnd, &r);
                GtU32 width = r.right - r.left;
                GtU32 height = r.bottom - r.top;

                GtEventContext context;
                context.data.GtU16[0] = (GtU16)width;
                context.data.GtU16[1] = (GtU16)height;
                gontiEventFire(GT_EVENT_CODE_RESIZED, 0, context);
            }break;
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
            case WM_KEYUP:
            case WM_SYSKEYUP: {
                GtB8 pressed;
                if (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN) pressed = GtTrue;
                else pressed = GtFalse;

                GtInputKeyboardKeys key = (GtU16)wParam;
                GtB8 isExtended = (HIWORD(lParam) & KF_EXTENDED) == KF_EXTENDED;

                if (wParam == VK_MENU) key = isExtended ? GT_KEY_RALT : GT_KEY_LALT;
                else if (wParam == VK_SHIFT) {
                    GtU32 lShift = MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC);
                    GtU32 scancode = ((lParam & (0xFF << 16)) >> 16);
                    key = scancode == lShift ? GT_KEY_LSHIFT : GT_KEY_RSHIFT;
                } else if (wParam == VK_CONTROL) key = isExtended ? GT_KEY_RCONTROL : GT_KEY_LCONTROL;

                gontiInputProcessKey(key, pressed);
                return 0;
            }break;
            case WM_MOUSEMOVE: {
                GtI32 xPosition = GET_X_LPARAM(lParam);
                GtI32 yPosition = GET_Y_LPARAM(lParam);

                gontiInputProcessMouseMove(xPosition, yPosition);
            }break;
            case WM_MOUSEWHEEL: {
                GtI32 zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
                if (zDelta != 0) {
                    zDelta = (zDelta < 0) ? -1: 1;
                    gontiInputProcessMouseWheel(zDelta);
                }
            }break;
            case WM_LBUTTONDOWN:
            case WM_MBUTTONDOWN:
            case WM_RBUTTONDOWN:
            case WM_LBUTTONUP:
            case WM_MBUTTONUP:
            case WM_RBUTTONUP: {
                GtB8 pressed = (msg == WM_LBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_RBUTTONDOWN);
                GtInputMouseBtns mButton = BUTTON_MAX_GT_BTNS;

                switch (msg) {
                    case WM_LBUTTONDOWN:
                    case WM_LBUTTONUP: {
                        mButton = GT_BTN_LEFT;
                    }break;
                    case WM_MBUTTONDOWN:
                    case WM_MBUTTONUP: {
                        mButton = GT_BTN_MIDDLE;
                    }break;
                    case WM_RBUTTONDOWN:
                    case WM_RBUTTONUP: {
                        mButton = GT_BTN_RIGHT;
                    }break;
                }

                if (mButton != BUTTON_MAX_GT_BTNS) {
                    gontiInputProcessButton(mButton, pressed);
                }
            }break;
            default: break;
        }

        return DefWindowProcA(hwnd, msg, wParam, lParam);
}

#endif