#include "PlatformEvents.h"

#if GONTI_USE_VULKAN && KPLATFORM_WINDOWS

#include "../InputSystem/Inputs.h"
#include "../Events.h"

LRESULT CALLBACK WndProc(HWND hwnd, u32 msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
            case WM_ERASEBKGND: {
                return 1;
            } // NOTE: Do not have to add break because you have return 1
            case WM_CLOSE: {
                GontiEventContext data = {};
                gontiEventFire(GONTI_EVENT_CODE_APPLICATION_QUIT, 0, data);
                return 0;
            } // NOTE: Do not have to add break because you have return 0
            case WM_DESTROY: {
                PostQuitMessage(0);
                return 0;
            } // NOTE: Do not have to add break because you have return 0
            case WM_SIZE: {
                RECT r;
                GetClientRect(hwnd, &r);
                u32 width = r.right - r.left;
                u32 height = r.bottom - r.top;

                GontiEventContext context;
                context.data.u16[0] = (u16)width;
                context.data.u16[1] = (u16)height;
                gontiEventFire(GONTI_EVENT_CODE_RESIZED, 0, context);
            }break;
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
            case WM_KEYUP:
            case WM_SYSKEYUP: {
                b8 pressed; //= (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
                if (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN) pressed = true;
                else pressed = false;

                GontiInputKeyboardKeys key = (u16)wParam;
                gontiInputProcessKey(key, pressed);
            }break;
            case WM_MOUSEMOVE: {
                i32 xPosition = GET_X_LPARAM(lParam);
                i32 yPosition = GET_Y_LPARAM(lParam);

                gontiInputProcessMouseMove(xPosition, yPosition);
            }break;
            case WM_MOUSEWHEEL: {
                i32 zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
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
                b8 pressed = (msg == WM_LBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_RBUTTONDOWN);
                GontiInputMouseButtons mButton = BUTTON_MAX_GONTI_BUTTONS;

                switch (msg) {
                    case WM_LBUTTONDOWN:
                    case WM_LBUTTONUP: {
                        mButton = GONTI_BUTTON_LEFT;
                    }break;
                    case WM_MBUTTONDOWN:
                    case WM_MBUTTONUP: {
                        mButton = GONTI_BUTTON_MIDDLE;
                    }break;
                    case WM_RBUTTONDOWN:
                    case WM_RBUTTONUP: {
                        mButton = GONTI_BUTTON_RIGHT;
                    }break;
                }

                if (mButton != BUTTON_MAX_GONTI_BUTTONS) {
                    gontiInputProcessButton(mButton, pressed);
                }
            }break;
            default: break;
        }

        return DefWindowProcA(hwnd, msg, wParam, lParam);
}

#endif