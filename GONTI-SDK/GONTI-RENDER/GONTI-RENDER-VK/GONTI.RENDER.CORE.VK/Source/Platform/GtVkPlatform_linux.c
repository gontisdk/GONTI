#include <Platform/GtVkPlatform.h>

#if GTPLATFORM_LINUX

#include <GONTI-CORE/GONTI.CORE/Source/Memory/GtMemory.h>
#include <GONTI-CORE/GONTI.CORE/Source/Logging/GtLogger.h>
#include <GONTI-CORE/GONTI.CONTAINERS/Source/DynamicArray/GtDArray.h>
#include <GONTI-CORE/GONTI.CORE/Source/Platform/GtPlatform.h>
#include <GONTI-CORE/GONTI.CORE/Source/CStringTools/GtCStrTools.h>

#include <xcb/xcb.h>
#include <X11/XKBlib.h> // sudo apt-get install libx11-dev
#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h> // sudo apt-get install libxkbcommon-x11-dev
#include <sys/time.h>

#define VK_USE_PLATFORM_XCB_KHR
#include <vulkan/vulkan_xcb.h>
#include <vulkan/vulkan.h>

#if _POSIX_C_SOURCE >= 199309L
    #include <time.h>
#else
    #include <unistd.h>
#endif

#include <stdlib.h>
#include <stdio.h>

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
    state->gontiVkInternalStateLinux.display = XOpenDisplay(NULL);

    XAutoRepeatOff(state->gontiVkInternalStateLinux.display);

    GtB32 screenP = 0;
    state->gontiVkInternalStateLinux.connection = XGetXCBConnection(state->gontiVkInternalStateLinux.display);

    if (xcb_connection_has_error(state->gontiVkInternalStateLinux.connection)) {
        GTFATAL("Failed to connect to X server via XCB");
        return GtFalse;
    }

    const struct xcb_setup_t* setup = xcb_get_setup(state->gontiVkInternalStateLinux.connection);
    xcb_screen_iterator_t it = xcb_setup_roots_iterator(setup);

    for (GtI32 s = screenP; s > 0; s--) {
        xcb_screen_next(&it);
    }

    state->gontiVkInternalStateLinux.screen = it.data;
    state->gontiVkInternalStateLinux.window = xcb_generate_id(state->gontiVkInternalStateLinux.connection);

    GtU32 eventMask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    GtU32 eventValues = XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE | 
                        XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE | 
                        XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_POINTER_MOTION | 
                        XCB_EVENT_MASK_STRUCTURE_NOTIFY;

    GtU32 valueList[] = {state->gontiVkInternalStateLinux.screen->black_pixel, eventValues};

    xcb_void_cookie_t cookie = xcb_create_window(
        state->gontiVkInternalStateLinux.connection,
        XCB_COPY_FROM_PARENT,
        state->gontiVkInternalStateLinux.window,
        state->gontiVkInternalStateLinux.screen->root,
        x,
        y,
        width,
        height,
        0,
        XCB_WINDOW_CLASS_INPUT_OUTPUT,
        state->gontiVkInternalStateLinux.screen->root_visual,
        eventMask,
        valueList
    );
    (void)cookie; 

    xcb_change_property(
        state->gontiVkInternalStateLinux.connection,
        XCB_PROP_MODE_REPLACE,
        state->gontiVkInternalStateLinux.window,
        XCB_ATOM_WM_NAME,
        XCB_ATOM_STRING,
        8,
        gontiStringLength(windowName),
        windowName
    );

    xcb_intern_atom_cookie_t wmDeleteCookie = xcb_intern_atom(
        state->gontiVkInternalStateLinux.connection,
        0,
        gontiStringLength("WM_DELETE_WINDOW"),
        "WM_DELETE_WINDOW"
    );

    xcb_intern_atom_cookie_t wmProtocolsCookie = xcb_intern_atom(
        state->gontiVkInternalStateLinux.connection,
        0,
        gontiStringLength("WM_PROTOCOLS"),
        "WM_PROTOCOLS"
    );

    xcb_intern_atom_reply_t* wmDeleteReply = xcb_intern_atom_reply(
        state->gontiVkInternalStateLinux.connection,
        wmDeleteCookie,
        NULL
    );

    xcb_intern_atom_reply_t* wmProtocolsReply = xcb_intern_atom_reply(
        state->gontiVkInternalStateLinux.connection,
        wmProtocolsCookie,
        NULL
    );

    state->gontiVkInternalStateLinux.wmDeleteWin = wmDeleteReply->atom;
    state->gontiVkInternalStateLinux.wmProtocols = wmProtocolsReply->atom;

    xcb_change_property(
        state->gontiVkInternalStateLinux.connection,
        XCB_PROP_MODE_REPLACE,
        state->gontiVkInternalStateLinux.window,
        wmProtocolsReply->atom,
        4,
        32,
        1,
        &wmDeleteReply->atom
    );

    xcb_map_window(state->gontiVkInternalStateLinux.connection, state->gontiVkInternalStateLinux.window);

    GtI32 streamResult = xcb_flush(state->gontiVkInternalStateLinux.connection);
    if (streamResult <= 0) {
        GTFATAL("An error occurred when flushing the stream: %d", streamResult);
        return GtFalse;
    }

    free(wmDeleteReply);
    free(wmProtocolsReply);
    return GtTrue;
}
GtB8 gontiVkPlatformPumpMessage(void* platState) {
    GtVkPlatformState* state = (GtVkPlatformState*)platState;

    if (state->gontiVkInternalStateLinux.linux_process_message == NULL) {
        GTFATAL("linux_process_message must be set.");
        gontiVkPlatformShutdown(platState);
        return GtFalse;
    }

    GtB8 quitFlagged = GtFalse;

    while ((state->gontiVkInternalStateLinux.event = xcb_poll_for_event(state->gontiVkInternalStateLinux.connection)) != NULL) {
        if ((state->gontiVkInternalStateLinux.event->response_type & ~0x80) == XCB_CLIENT_MESSAGE) {
            state->gontiVkInternalStateLinux.clientMessage = (xcb_client_message_event_t*)state->gontiVkInternalStateLinux.event;
        } else {
            state->gontiVkInternalStateLinux.clientMessage = NULL;
        }

        if (!state->gontiVkInternalStateLinux.linux_process_message(platState)) {
            quitFlagged = GtTrue;
        }

        free(state->gontiVkInternalStateLinux.event);
    }

    return !quitFlagged;
}

GtB8 gontiVkPlatformCreateVulkanSurface(GtVkPlatformState* platState, GtVkContext* context) {
    VkXcbSurfaceCreateInfoKHR createInfo = {VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR};
    createInfo.connection = platState->gontiVkInternalStateLinux.connection;
    createInfo.window = platState->gontiVkInternalStateLinux.window;

    VkResult result = vkCreateXcbSurfaceKHR(
        context->instance,
        &createInfo,
        context->allocator,
        &platState->vkInternalState.surface
    ); if (result != VK_SUCCESS) {
        GTFATAL("Vulkan surface creation failed!");
        return GtFalse;
    }

    context->surface = platState->vkInternalState.surface;
    return GtTrue;
}

void gontiVkPlatformShutdown(void* platState) {
    if (!platState) return;
    GtVkPlatformState* state = (GtVkPlatformState*)platState;

    if (state->gontiVkInternalStateLinux.display) {
        if (state->gontiVkInternalStateLinux.connection && state->gontiVkInternalStateLinux.window) {
            xcb_destroy_window(state->gontiVkInternalStateLinux.connection, state->gontiVkInternalStateLinux.window);
            xcb_flush(state->gontiVkInternalStateLinux.connection);
        }

        XAutoRepeatOn(state->gontiVkInternalStateLinux.display);
        XCloseDisplay(state->gontiVkInternalStateLinux.display);

        state->gontiVkInternalStateLinux.display = NULL;
        state->gontiVkInternalStateLinux.connection = NULL;
    }

    GTINFO("Platform shutdown finished.");
}

// void gontiVkPlatformShutdown(void* platState) {
//     GtVkPlatformState* state = (GtVkPlatformState*)platState;

//     XAutoRepeatOn(state->gontiVkInternalStateLinux.display);
//     xcb_destroy_window(state->gontiVkInternalStateLinux.connection, state->gontiVkInternalStateLinux.window);
// }

void gontiVkPlatformGetRequiredExtensionNames(const char*** namesDarray) {
    const char* ext = "VK_KHR_xcb_surface"; // "VK_KHR_xlib_surface" or "VK_KHR_xcb_surface"?
    gontiDarrayPush(*namesDarray, ext);
}

#endif