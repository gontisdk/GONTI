#include "VulkanPlatform.h"

#if KPLATFORM_WINDOWS
#ifdef PlatformVulkan_ON
#ifdef USE_VULKAN

    #include <Windows.h>
    #include <windowsx.h>
    #include <string.h>
    #include <vulkan/vulkan.h>
    #include <vulkan/vulkan_win32.h>

    #include "../../../Utilities/Logging/Scripts/Logger.h"
    #include "../Platform.h"
    #include "../../../Events/InputSystem/Vulkan/VulkanInputs.h"
    #include "../../../Containers/DynamicArray/DynamicArray.h"
    #include "../../MemorySubsystem/MemorySubsystem.h"
    #include "../../RendererSubsystem/Types/Vulkan/VulkanContextTypes.inl"
    #include "../../../Events/Events.h"

    typedef struct InternalState {
        HINSTANCE hInstance;
        HWND hwnd;
        VkSurfaceKHR surface;
    }InternalState;

    static f64 clockFrequency;
    static LARGE_INTEGER startTime; 

    LRESULT CALLBACK win32_process_message(HWND hwnd, u32 msg, WPARAM wParam, LPARAM lParam);

    b8 platformStartup(gontiPlatformState* platState, const char* className, const char* appName, i32 x, i32 y, i32 width, i32 height) {
        // MessageBoxA title
        const char* msgbxsTitle = "Vulkan Window Error";

        platState->internalState = malloc(sizeof(InternalState));
        InternalState* state = (InternalState*)platState->internalState;

        state->hInstance = GetModuleHandleA(0);

        HICON icon = LoadIcon(state->hInstance, IDI_APPLICATION);
        WNDCLASSA wc;
        k_setMemory(&wc, 0, sizeof(wc));
        wc.style = CS_DBLCLKS;
        wc.lpfnWndProc = win32_process_message;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = state->hInstance;
        wc.hIcon = icon;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = NULL; // Transparent
        wc.lpszClassName = className;

        if (!RegisterClassA(&wc)) {
            MessageBoxA(0, "Window Registration Failed", msgbxsTitle, MB_ICONERROR | MB_OK);
            return false;
        }

        u32 clientX = x;
        u32 clientY = y;
        u32 clientWidth = width;
        u32 clientHeight = height;

        u32 windowX = clientX;
        u32 windowY = clientY;
        u32 windowWidth = clientWidth;
        u32 windowHeight = clientHeight;

        u32 windowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
        u32 windowExStyle = WS_EX_APPWINDOW;

        windowStyle |= WS_MAXIMIZEBOX;
        windowStyle |= WS_MINIMIZEBOX;
        windowStyle |= WS_THICKFRAME;

        RECT borderRect = {0, 0, 0, 0};
        AdjustWindowRectEx(&borderRect, windowStyle, 0, windowExStyle);

        windowX += borderRect.left;
        windowY += borderRect.top;
        windowWidth += borderRect.right - borderRect.left;
        windowHeight += borderRect.bottom - borderRect.top;

        HWND handle = CreateWindowExA(
            windowExStyle,
            className,
            appName,
            windowStyle,
            windowX,
            windowY,
            windowWidth,
            windowHeight,
            0,
            0,
            state->hInstance,
            0
        );

        if (handle == 0) {
            MessageBoxA(NULL, "Window Creation Failed", msgbxsTitle, MB_ICONERROR | MB_OK);
            KFATAL("Window creation failed");
            return false;
        }

        state->hwnd = handle;

        b32 shouldActive = 1;
        i32 showWindowCommandFlags = shouldActive ? SW_SHOW : SW_SHOWNOACTIVATE;

        ShowWindow(state->hwnd, showWindowCommandFlags);

        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        clockFrequency = 1.0 / (f64)frequency.QuadPart;
        QueryPerformanceCounter(&startTime);

        return true;
    }

    b8 platformPumpMessage(gontiPlatformState* platState) {
        MSG message;
        while (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }

        return true;
    }

    b8 platformCreateVulkanSurface(gontiPlatformState* platState, gontiVulkanContext* context) {
        InternalState* state = (InternalState*)platState->internalState;

        VkWin32SurfaceCreateInfoKHR createInfo = {VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR};
        createInfo.hinstance = state->hInstance;
        createInfo.hwnd = state->hwnd;

        VkResult result = vkCreateWin32SurfaceKHR(context->instance, &createInfo, context->allocator, &state->surface);
        if (result != VK_SUCCESS) {
            KFATAL("Vulkan surface creation failed!");
            return false;
        }

        context->surface = state->surface;
        return true;
    }

    f64 platformGetAbsoluteTime() {
        LARGE_INTEGER nowTime;
        QueryPerformanceCounter(&nowTime);
        return (f64)nowTime.QuadPart * clockFrequency;
    }

    void platformShutdown(gontiPlatformState* platState) {
        InternalState* state = (InternalState*)platState->internalState;

        if (state->hwnd) {
            DestroyWindow(state->hwnd);
            state->hwnd = 0;
        }
    }

    void platformGetRequiredExtensionNames(const char*** namesDarray) {
        darrayPush(*namesDarray, &"VK_KHR_win32_surface");
    }

    LRESULT CALLBACK win32_process_message(HWND hwnd, u32 msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
            case WM_ERASEBKGND: {
                return 1;
            } // NOTE: Do not have to add break because you have return 1
            case WM_CLOSE: {
                EventContext data = {};
                eventFire(EVENT_CODE_APPLICATION_QUIT, 0, data);
                return 0;
            } // NOTE: Do not have to add break because you have return 0
            case WM_DESTROY: {
                PostQuitMessage(0);
                return 0;
            } // NOTE: Do not have to add break because you have return 0
            case WM_SIZE: {
                //RECT r;
                //GetClientRect(hwnd, &r);
                //u32 width = r.right - r.left;
                //u32 height = r.bottom - r.top;

                // TODO: Fire an event for window resieze
            }break;
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
            case WM_KEYUP:
            case WM_SYSKEYUP: {
                b8 pressed; //= (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
                if (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN) pressed = true;
                else pressed = false;

                InputKeyboardKeys key = (u16)wParam;
                inputProcessKey(key, pressed);
            }break;
            case WM_MOUSEMOVE: {
                i32 xPosition = GET_X_LPARAM(lParam);
                i32 yPosition = GET_Y_LPARAM(lParam);

                inputProcessMouseMove(xPosition, yPosition);
            }break;
            case WM_MOUSEWHEEL: {
                i32 zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
                if (zDelta != 0) {
                    zDelta = (zDelta < 0) ? -1: 1;
                    inputProcessMouseWheel(zDelta);
                }
            }break;
            case WM_LBUTTONDOWN:
            case WM_MBUTTONDOWN:
            case WM_RBUTTONDOWN:
            case WM_LBUTTONUP:
            case WM_MBUTTONUP:
            case WM_RBUTTONUP: {
                b8 pressed = (msg == WM_LBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_RBUTTONDOWN);
                InputMouseButtons mButton = BUTTON_MAX_BUTTONS;

                switch (msg) {
                    case WM_LBUTTONDOWN:
                    case WM_LBUTTONUP: {
                        mButton = BUTTON_LEFT;
                    }break;
                    case WM_MBUTTONDOWN:
                    case WM_MBUTTONUP: {
                        mButton = BUTTON_MIDDLE;
                    }break;
                    case WM_RBUTTONDOWN:
                    case WM_RBUTTONUP: {
                        mButton = BUTTON_RIGHT;
                    }break;
                }

                if (mButton != BUTTON_MAX_BUTTONS) {
                    inputProcessButton(mButton, pressed);
                }
            }break;
            default: break;
        }

        return DefWindowProcA(hwnd, msg, wParam, lParam);
    }

#endif
#endif
#endif