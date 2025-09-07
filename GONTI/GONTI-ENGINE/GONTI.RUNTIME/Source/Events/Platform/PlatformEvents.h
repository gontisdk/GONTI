#ifndef VULKANPLATFORMEVENTS_H
#define VULKANPLATFORMEVENTS_H

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI/GONTI-ENGINE/GONTI.RENDER/Source/Config/SetupGraphicBackend/UserSetGraphicBackend.h>

        #if GONTI_USE_VULKAN

            #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/Defines.h>
            #include <GONTI/GONTI-ENGINE/GONTI.RENDER/Source/Platform/Vulkan/VulkanPlatform.h>
            #include "../InputSystem/Inputs.h"

            #if KPLATFORM_WINDOWS
                #include <Windows.h>
                
                LRESULT CALLBACK WndProc(HWND hwnd, u32 msg, WPARAM wParam, LPARAM lParam);
            #endif

            #if KPLATFORM_LINUX
                b8 LinuxProc(GontiVulkanInternalStateLinux* state);
                KAPI GontiInputKeyboardKeys translateKeycode(u32 xKeyCode);
            #endif

        #endif

#ifdef __cplusplus
    }
#endif

#endif