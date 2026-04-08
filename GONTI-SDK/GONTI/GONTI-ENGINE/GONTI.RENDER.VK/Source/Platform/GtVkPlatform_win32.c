#include "GtVkPlatform.h"

#if GTPLATFORM_WINDOWS

#include <vulkan/vulkan_win32.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/CStringTools/GtCStrTools.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Platform/GtPlatform.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Logging/GtLogger.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Containers/DynamicArray/GtDArray.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Memory/GtMemory.h>

static LARGE_INTEGER startTime;

GtB8 gontiVkPlatformStartup(
    GtU64* memoryRequirement, 
    void* platState,
    const char* windowName,
    const char* windowClass,
    GtI32 x,
    GtI32 y,
    GtI32 width,
    GtI32 height
) {
    *memoryRequirement = sizeof(GtVkPlatformState);
    if (!platState) return GtTrue;

    GtVkPlatformState* state = (GtVkPlatformState*)platState;
    state->gontiVkInternalStateWin32.hInstance = GetModuleHandleA(0);

    if (state->gontiVkInternalStateWin32.win32_process_message == NULL) {
        GTFATAL("win32_process_message must match WndProc signature. Without it, message handling will fail.");
        gontiVkPlatformShutdown(platState);
        return GtFalse;

    }

    HICON icon = LoadIcon(state->gontiVkInternalStateWin32.hInstance, IDI_APPLICATION);
    WNDCLASSA wc;
    gt_setMemory(&wc, 0, sizeof(wc));
    wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = state->gontiVkInternalStateWin32.win32_process_message;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = state->gontiVkInternalStateWin32.hInstance;
    wc.hIcon = icon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL; // Transparent
    wc.lpszClassName = windowClass;

    if (!RegisterClassA(&wc)) {
        MessageBoxA(0, "Window Registration Failed", "Failed create window", MB_ICONERROR | MB_OK);
        gontiVkPlatformShutdown(platState);
        return GtFalse;
    }

    GtU32 clientX = x;
    GtU32 clientY = y;
    GtU32 clientWidth = width;
    GtU32 clientHeight = height;

    GtU32 windowX = clientX;
    GtU32 windowY = clientY;
    GtU32 windowWidth = clientWidth;
    GtU32 windowHeight = clientHeight;

    GtU32 windowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
    GtU32 windowExStyle = WS_EX_APPWINDOW;

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
        windowClass,
        windowName,
        windowStyle,
        windowX,
        windowY,
        windowWidth,
        windowHeight,
        0,
        0,
        state->gontiVkInternalStateWin32.hInstance,
        0
    );

    if (handle == 0) {
        MessageBoxA(NULL, "Window failed to create", "Failed create window", MB_ICONERROR | MB_OK);
        GTFATAL("Window creation failed");
        gontiVkPlatformShutdown(platState);
        return GtFalse;
    }

    state->gontiVkInternalStateWin32.hwnd = handle;

    GtB32 shouldActive = 1;
    GtI32 showWindowCommandFlags = shouldActive ? SW_SHOW : SW_SHOWNOACTIVATE;

    ShowWindow(state->gontiVkInternalStateWin32.hwnd, showWindowCommandFlags);

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    gontiPlatformSetClockFrequency(1.0 / (GtF64)frequency.QuadPart);
    QueryPerformanceCounter(&startTime);

    return GtTrue;
}

GtB8 gontiVkPlatformPumpMessage(void* platState) {
    MSG message;
    while (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }

    return GtTrue;
}

GtB8 gontiVkPlatformCreateVulkanSurface(GtVkPlatformState* platState, GtVkContext* context) {
    GtVkInternalStateWIN* state = &platState->gontiVkInternalStateWin32;

    VkWin32SurfaceCreateInfoKHR createInfo = {VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR};
    createInfo.hinstance = state->hInstance;
    createInfo.hwnd = state->hwnd;

    VkResult result = vkCreateWin32SurfaceKHR(context->instance, &createInfo, context->allocator, &platState->vkInternalState.surface);
    if (result != VK_SUCCESS) {
        GTFATAL("Vulkan surface creation failed!");
        return GtFalse;
    }

    context->surface = platState->vkInternalState.surface;
    return GtTrue;
}

void gontiVkPlatformShutdown(void* platState) {
    GtVkPlatformState* state = (GtVkPlatformState*)platState;
    
    if (state->gontiVkInternalStateWin32.hwnd) {
        DestroyWindow(state->gontiVkInternalStateWin32.hwnd);
        state->gontiVkInternalStateWin32.hwnd = 0;
    }
    
    if (state->internalState) {
        state->internalState = 0;
    }
}

void gontiVkPlatformGetRequiredExtensionNames(const char*** namesDarray) {
    gontiDarrayPush(*namesDarray, &"VK_KHR_win32_surface");
}

#endif