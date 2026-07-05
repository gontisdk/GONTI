#ifndef GtPlatformEventsH
#define GtPlatformEventsH

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI-RENDER/GONTI.RENDER/Source/Config/SetupGraphicBackend/GtUsrSetGraphBackend.h>

        #include <GONTI-CORE/GONTI.CORE/Source/Defines/GtDefines.inl>
        #include <Inputs/GtInputs.h>

        #if GTPLATFORM_WINDOWS
            #include <Windows.h>
                
            LRESULT CALLBACK WndProc(HWND hwnd, GtU32 msg, WPARAM wParam, LPARAM lParam);
        #endif

        #if GTPLATFORM_LINUX
            #if defined(GONTI_USE_VK)
                #include <GONTI-RENDER/GONTI-RENDER-VK/GONTI.RENDER.CORE.VK/Source/Platform/GtVkPlatform.h>
                GtB8 LinuxProc(void* state);
                GTAPI GtInputKeyboardKeys translateKeycode(GtU32 xKeyCode);
            #endif
        #endif

#ifdef __cplusplus
    }
#endif

#endif