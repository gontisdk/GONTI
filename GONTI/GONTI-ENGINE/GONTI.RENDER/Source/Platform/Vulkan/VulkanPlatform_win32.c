#include "VulkanPlatform.h"

#if GONTI_USE_VULKAN && KPLATFORM_WINDOWS

#include <vulkan/vulkan_win32.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/String/String.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Platform/Platform.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Logging/Logger.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Containers/DynamicArray/DynamicArray.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Memory/Memory.h>

static f64 clockFrequency;
static LARGE_INTEGER startTime;

b8 gontiVkPlatformStartup(
    GontiVulkanPlatformState* platState,
    const char* windowName,
    const char* windowClass,
    i32 x,
    i32 y,
    i32 width,
    i32 height
) {    
    platState->internalState = k_allocate(sizeof(GontiVulkanInternalStateWindows), GONTI_MEMORY_TAG_WINDOW);
    platState->internalState = &platState->gontiVkInternalStateWin32;
    GontiVulkanInternalStateWindows* state = (GontiVulkanInternalStateWindows*)platState->internalState;
    state->hInstance = GetModuleHandleA(0);

    platState->vkInternalState = k_allocate(sizeof(GontiVulkanInternalStateVK), GONTI_MEMORY_TAG_RENDERER);

    if (state->win32_process_message == NULL) {
        KFATAL("win32_process_message must match WndProc signature. Without it, message handling will fail.");
        gontiVkPlatformShutdown(platState);
        return false;
    }

    HICON icon = LoadIcon(state->hInstance, IDI_APPLICATION);
    WNDCLASSA wc;
    k_setMemory(&wc, 0, sizeof(wc));
    wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = state->win32_process_message;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = state->hInstance;
    wc.hIcon = icon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL; // Transparent
    wc.lpszClassName = windowClass;

    if (!RegisterClassA(&wc)) {
        MessageBoxA(0, "Window Registration Failed", "Failed create window", MB_ICONERROR | MB_OK);
        gontiVkPlatformShutdown(platState);
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
        windowClass,
        windowName,
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
        MessageBoxA(NULL, "Window failed to create", "Failed create window", MB_ICONERROR | MB_OK);
        KFATAL("Window creation failed");
        gontiVkPlatformShutdown(platState);
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

b8 gontiVkPlatformPumpMessage(GontiVulkanPlatformState* platState) {
    MSG message;
    while (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }

    return true;
}

b8 gontiVkPlatformCreateVulkanSurface(GontiVulkanPlatformState* platState, GontiVulkanContext* context) {
    GontiVulkanInternalStateWindows* state = (GontiVulkanInternalStateWindows*)platState->internalState;

    VkWin32SurfaceCreateInfoKHR createInfo = {VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR};
    createInfo.hinstance = state->hInstance;
    createInfo.hwnd = state->hwnd;

    VkResult result = vkCreateWin32SurfaceKHR(context->instance, &createInfo, context->allocator, &platState->vkInternalState->surface);
    if (result != VK_SUCCESS) {
        KFATAL("Vulkan surface creation failed!");
        return false;
    }

    context->surface = platState->vkInternalState->surface;
    return true;
}

f64 gontiVkPlatformGetAbsoluteTime() {
    LARGE_INTEGER nowTime;
    QueryPerformanceCounter(&nowTime);
    return (f64)nowTime.QuadPart * clockFrequency;
}

void gontiVkPlatformShutdown(GontiVulkanPlatformState* platState) {
    GontiVulkanInternalStateWindows* state = (GontiVulkanInternalStateWindows*)platState->internalState;
    
    if (state->hwnd) {
        DestroyWindow(state->hwnd);
        state->hwnd = 0;
    }
    
    if (platState->internalState) 
        k_free(platState->internalState, sizeof(GontiVulkanInternalStateWindows), GONTI_MEMORY_TAG_WINDOW);

    if(platState->vkInternalState) {
        k_free(platState->vkInternalState, sizeof(GontiVulkanInternalStateVK), GONTI_MEMORY_TAG_RENDERER);
    }
}

void gontiVkPlatformGetRequiredExtensionNames(const char*** namesDarray) {
    darrayPush(*namesDarray, &"VK_KHR_win32_surface");
}

#endif