#ifndef GtVkPlatformH
#define GtVkPlatformH

#ifdef __cplusplus
    extern "C" {
#endif

        #include <vulkan/vulkan.h>
        #include <GONTI-CORE/GONTI.CORE/Source/Defines/GtDefines.inl>
        #include <GONTI-RENDER/GONTI-RENDER-VK/GONTI.RENDER.COMMON.VK/Source/Types/GtVkContextTypes.inl>

        typedef struct GtVkInternalStateVK {
            VkSurfaceKHR surface;
        } GtVkInternalStateVK;

        #if GTPLATFORM_WINDOWS
            #include <Windows.h>
            #include <windowsx.h>

            typedef struct GtVkInternalStateWIN {
                HINSTANCE hInstance;
                HWND hwnd;
                LRESULT (CALLBACK *win32_process_message)(HWND hwnd, GtU32 msg, WPARAM wParam, LPARAM lParam);
            } GtVkInternalStateWIN;
        #endif
        #if GTPLATFORM_LINUX
            #include <xcb/xcb.h>
            #include <X11/Xlib.h>

            typedef struct GtVkInternalStateLINUX {
                Display* display;
                xcb_connection_t* connection;
                xcb_window_t window;
                xcb_screen_t* screen;
                xcb_atom_t wmProtocols;
                xcb_atom_t wmDeleteWin;

                xcb_generic_event_t* event; 
                xcb_client_message_event_t* clientMessage;
                GtB8 (*linux_process_message)(void* platState);
            } GtVkInternalStateLINUX;
        #endif

        typedef struct GtVkPlatformState {
            #if GTPLATFORM_WINDOWS
                GtVkInternalStateWIN gontiVkInternalStateWin32;
            #endif
            #if GTPLATFORM_LINUX
                GtVkInternalStateLINUX gontiVkInternalStateLinux;
            #endif

            GtVkInternalStateVK vkInternalState;
            void (*get_frame_buffer_size_ptr)(GtU32* width, GtU32* height);
            void* internalState;
        } GtVkPlatformState;

        GTAPI GtB8 gontiVkPlatformStartup(
            GtU64* memoryRequirement, 
            void* platState,
            const char* windowName,
            const char* windowClass,
            GtI32 x,
            GtI32 y,
            GtI32 width,
            GtI32 height
        );

        GTAPI GtB8 gontiVkPlatformPumpMessage(void* platState);
        GTAPI GtB8 gontiVkPlatformCreateVulkanSurface(GtVkPlatformState* platState, GtVkContext* context);

        GTAPI void gontiVkPlatformShutdown(void* platState);
        GTAPI void gontiVkPlatformGetRequiredExtensionNames(const char*** namesDarray);

#ifdef __cplusplus
    }
#endif

#endif