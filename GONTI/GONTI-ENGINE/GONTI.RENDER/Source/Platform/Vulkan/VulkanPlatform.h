#ifndef VULKANPLATFORM_H
#define VULKANPLATFORM_H

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../../Config/SetupGraphicBackend/UserSetGraphicBackend.h"

        #if GONTI_USE_VULKAN

            #include <vulkan/vulkan.h>
            #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/Defines.h>
            #include "../../Types/Vulkan/VulkanContextTypes.inl"

            typedef struct GontiVulkanInternalStateVK {
                VkSurfaceKHR surface;
            } GontiVulkanInternalStateVK;

            #if KPLATFORM_WINDOWS
                #include <Windows.h>
                #include <windowsx.h>

                typedef struct GontiVulkanInternalStateWindows {
                    HINSTANCE hInstance;
                    HWND hwnd;
                    LRESULT (CALLBACK *win32_process_message)(HWND hwnd, u32 msg, WPARAM wParam, LPARAM lParam);
                } GontiVulkanInternalStateWindows;
            #endif
            #if KPLATFORM_LINUX
                #include <xcb/xcb.h>
                #include <X11/Xlib.h>

                typedef struct GontiVulkanInternalStateLinux {
                    Display* display;
                    xcb_connection_t* connection;
                    xcb_window_t window;
                    xcb_screen_t* screen;
                    xcb_atom_t wmProtocols;
                    xcb_atom_t wmDeleteWin;

                    xcb_generic_event_t* event; 
                    xcb_client_message_event_t* clientMessage;
                    b8 (*linux_process_message)(GontiVulkanInternalStateLinux gontiVkInternalStateLinux);
                } GontiVulkanInternalStateLinux;
            #endif

            typedef struct GontiVulkanPlatformState {
                #if KPLATFORM_WINDOWS
                    GontiVulkanInternalStateWindows gontiVkInternalStateWin32;
                #endif
                #if KPLATFORM_LINUX
                    GontiVulkanInternalStateLinux gontiVkInternalStateLinux;
                #endif

                GontiVulkanInternalStateVK* vkInternalState;
                void (*get_frame_buffer_size_ptr)(u32* width, u32* height);
                void* internalState;
            } GontiVulkanPlatformState;

            KAPI b8 gontiVkPlatformStartup(
                GontiVulkanPlatformState* platState,
                const char* windowName,
                const char* windowClass,
                i32 x,
                i32 y,
                i32 width,
                i32 height
            );

            KAPI b8 gontiVkPlatformPumpMessage(GontiVulkanPlatformState* platState);
            KAPI b8 gontiVkPlatformCreateVulkanSurface(GontiVulkanPlatformState* platState, GontiVulkanContext* context);
            
            KAPI f64 gontiVkPlatformGetAbsoluteTime();

            KAPI void gontiVkPlatformShutdown(GontiVulkanPlatformState* platState);
            KAPI void gontiVkPlatformGetRequiredExtensionNames(const char*** namesDarray);

        #endif

#ifdef __cplusplus
    }
#endif

#endif