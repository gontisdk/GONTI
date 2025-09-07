#include "VulkanPlatform.h"

#if GONTI_USE_VULKAN && KPLATFORM_LINUX

#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Memory/Memory.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Logging/Logger.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Containers/DynamicArray/DynamicArray.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Platform/Platform.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/String/String.h>

#include <xcb/xcb.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h> // sudo apt-get install libx11-dev
#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h> // sudo apt-get install libxkbcommon-x11-dev
#include <sys/time.h>
#include <vulkan/vulkan.h>

#if _POSIX_C_SOURCE >= 199309L
    #include <time.h>
#else
    #include <unistd.h>
#endif

#include <stdlib.h>
#include <stdio.h>

#define VK_USE_PLATFORM_XCB_KHR

b8 gontiVkPlatformStartup(GontiVulkanPlatformState* platState, const char* className, const char* appName, i32 x, i32 y, i32 width, i32 height) {
    platState->internalState = k_allocate(sizeof(GontiVulkanInternalStateLinux), GONTI_MEMORY_TAG_WINDOW);
    platState->internalState = &platState->gontiVkInternalStateLinux;

    GontiVulkanInternalStateLinux* state = (GontiVulkanInternalStateLinux*)platState->internalState;
    state->display = XOpenDisplay(NULL);

    XAutoRepeatOff(state->display);

    int screenP = 0;
    state->connection = XGetXCBConnection(state->display);

    if (xcb_connection_has_error(state->connection)) {
        KFATAL("Failed to connect to X server via XCB");
        return false;
    }

    const struct xcb_setup_t* setup = xcb_get_setup(state->connection);
    xcb_screen_iterator_t it = xcb_setup_roots_iterator(setup);

    for (i32 s = screenP; s > 0; s--) {
        xcb_screen_next(&it);
    }

    state->screen = it.data;
    state->window = xcb_generate_id(state->connection);

    u32 eventMask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    u32 eventValues = XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE | 
                        XCB_EVENT_MASK_GONTI_KEY_PRESS | XCB_EVENT_MASK_GONTI_KEY_RELEASE | 
                        XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_POINTER_MOTION | 
                        XCB_EVENT_MASK_STRUCTURE_NOTIFY;

    u32 valueList[] = {state->screen->black_pixel, eventValues};

    xcb_void_cookie_t cookie = xcb_create_window(
        state->connection,
        XCB_COPY_FROM_PARENT,
        state->window,
        state->screen->root,
        x,
        y,
        width,
        height,
        0,
        XCB_WINDOW_CLASS_INPUT_OUTPUT,
        state->screen->root_visual,
        eventMask,
        valueList
    );

    xcb_change_property(
        state->connection,
        XCB_PROP_MODE_REPLACE,
        state->window,
        XCB_ATOM_WM_NAME,
        XCB_ATOM_STRING,
        8,
        gontiStringLength(appName),
        appName
    );

    xcb_intern_atom_cookie_t wmDeleteCookie = xcb_intern_atom(
        state->connection,
        0,
        gontiStringLength("WM_DELETE_WINDOW"),
        "WM_DELETE_WINDOW"
    );

    xcb_intern_atom_cookie_t wmProtocolsCookie = xcb_intern_atom(
        state->connection,
        0,
        gontiStringLength("WM_PROTOCOLS"),
        "WM_PROTOCOLS"
    );

    xcb_intern_atom_reply_t* wmDeleteReply = xcb_intern_atom_reply(
        state->connection,
        wmDeleteCookie,
        NULL
    );

    xcb_intern_atom_reply_t* wmProtocolsReply = xcb_intern_atom_reply(
        state->connection,
        wmProtocolsCookie,
        NULL
    );

    state->wmDeleteWin = wmDeleteReply->atom;
    state->wmProtocols = wmProtocolsReply->atom;

    xcb_change_property(
        state->connection,
        XCB_PROP_MODE_REPLACE,
        state->window,
        wmProtocolsReply->atom,
        4,
        32,
        1,
        &wmDeleteReply->atom
    );

    xcb_map_window(state->connection, state->window);

    i32 streamResult = xcb_flush(state->connection);
    if (streamResult <= 0) {
        KFATAL("An error occurred when flushing the stream: %d", streamResult);
        return false;
    }

    return true;
}
b8 gontiVkPlatformPumpMessage(GontiVulkanPlatformState* platState) {
    GontiVulkanInternalStateLinux* state = (GontiVulkanInternalStateLinux*)platState->internalState;

    if (state->linux_process_message == NULL) {
        KFATAL("linux_process_message must be set.");
        gontiVkPlatformShutdown();
        return false;
    }

    b8 quitFlagged = false;

    while ((state->event = xcb_poll_for_event(state->connection)) != NULL) {
        if ((state->event->response_type & ~0x80) == XCB_CLIENT_MESSAGE) {
            state->clientMessage = (xcb_client_message_event_t*)state->event;
        } else {
            state->clientMessage = NULL;
        }

        if (!state->linux_process_message(state)) {
            quitFlagged = true;
        }

        free(state->event);
    }

    return !quitFlagged;
}

b8 gontiVkPlatformCreateVulkanSurface(GontiVulkanPlatformState* platState, GontiVulkanContext* context) {
    GontiVulkanInternalStateLinux* state = (GontiVulkanInternalStateLinux*)platState->internalState;

    VkXcbSurfaceCreateInfoKHR createInfo = {VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR};
    createInfo.connection = state->connection;
    createInfo.window = state->window;

    VkResult result = vkCreateXcbSurfaceKHR(
        context->instance,
        &createInfo,
        context->allocator,
        &platState->vkInternalState->surface
    ); if (result != VK_SUCCESS) {
        KFATAL("Vulkan surface creation failed!");
        return false;
    }

    context->surface = platState->vkInternalState->surface;
    return true;
}

f64 gontiVkPlatformGetAbsoluteTime() {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec + now.tv_nsec * 0.000000001;
}

void gontiVkPlatformShutdown(GontiVulkanPlatformState* platState) {
    GontiVulkanInternalStateLinux* state = (GontiVulkanInternalStateLinux*)platState->internalState;

    XAutoRepeatOn(state->display);
    xcb_destroy_window(state->connection, state->window);

    k_free(platState->internalState, sizeof(GontiVulkanInternalStateLinux), GONTI_MEMORY_TAG_WINDOW);
}

void gontiVkPlatformGetRequiredExtensionNames(const char*** namesDarray) {
    darrayPush(*namesDarray, &"VK_KHR_xcb_surface"); // "VK_KHR_xlib_surface" or "VK_KHR_xcb_surface"?
}

#endif