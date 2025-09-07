#include "VulkanApplication.h"

#if GONTI_USE_VULKAN

#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Logging/Logger.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Memory/Memory.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Platform/Platform.h>
#include <GONTI/GONTI-ENGINE/GONTI.RENDER/Source/Platform/Vulkan/VulkanPlatform.h>
#include <GONTI/GONTI-ENGINE/GONTI.RENDER/Source/Renderer/Frontend/RenderFrontend.h>
#include "../../Clock/Vulkan/VulkanClock.h"
#include "../../Events/Platform/PlatformEvents.h"
#include "../../Events/Events.h"
#include "../../Events/InputSystem/Inputs.h"

typedef struct GontiVulkanApplicationState {
    b8 isRunning;
    b8 isSuspended;
    GontiVulkanPlatformState gontiPlatform;
    GontiVulkanEntry* entryInst;
    i16 width;
    i16 height;
    Clock clock;
    f64 lastTime;
}GontiVulkanApplicationState;

static b8 initialized = false;
static GontiVulkanApplicationState appState;

/*
* B8
*/
b8 gontiVkApplicationCreate(GontiVulkanEntry* entryInst) {
    if (initialized) {
        KERROR("gontiVkApplicationCreate() method called more than once.");
        return false;
    }

    appState.entryInst = entryInst;

    gontiLoggerInitializeLogging();
    gontiInputInitialize();

    appState.isRunning = true;
    appState.isSuspended = false;

    if (!gontiEventInitialize()) {
        KERROR("Event system failed initialization. Application can not continue");
        return false;
    }

    #if KPLATFORM_WINDOWS
        appState.gontiPlatform.gontiVkInternalStateWin32.win32_process_message = WndProc;
    #endif
    #if KPLATFORM_LINUX
        appState.gontiPlatform.gontiVkInternalStateLinux.linux_process_message = LinuxProc;
    #endif

    if (!gontiVkPlatformStartup(
        &appState.gontiPlatform,
        entryInst->windowConfig.windowName,
        entryInst->windowConfig.className,
        entryInst->windowConfig.startPosX, 
        entryInst->windowConfig.startPosY,
        entryInst->windowConfig.startWidth, 
        entryInst->windowConfig.startHeight
    )) {
        KFATAL("Window failed to create.");
        return false;
    }

    if (!gontiRendererInitialize(entryInst->windowConfig.windowName, &appState.gontiPlatform)) {
        KFATAL("Failed to initialize renderer. Aborting gontiVkApplication...");
        return false;
    }

    if (!appState.entryInst->initialize(appState.entryInst)) {
        KFATAL("Game failed to initialize");
        return false;
    }

    if (appState.entryInst->onResize != NULL) appState.entryInst->onResize(appState.entryInst, appState.width, appState.height);

    initialized = true;
    return true;
}
b8 gontiVkApplicationRun(u64 fpsLimit, b8 isFpsLimit) {
    if (fpsLimit <= 0) {
        fpsLimit = 1;
        isFpsLimit = false;
    }

    gontiVkClockStart(&appState.clock);
    gontiVkClockUpdate(&appState.clock);
    appState.lastTime = appState.clock.elapsed;

    f64 runningTime = 0;
    u8 frameCount = 0;
    f64 targetFrameSeconds = 1.0f / fpsLimit;

    KINFO(k_getMemoryUsageStr());

    while (appState.isRunning) {
        if (!gontiVkPlatformPumpMessage(&appState.gontiPlatform)) appState.isRunning = false;

        if (!appState.isSuspended) {
            gontiVkClockUpdate(&appState.clock);

            f64 currentTime = appState.clock.elapsed;
            f64 delta = (currentTime - appState.lastTime);
            f64 frameStartTime = gontiVkPlatformGetAbsoluteTime();

            if (!appState.entryInst->update(appState.entryInst, (f32)delta)) {
                KFATAL("Game update failed, shutting down...");
                appState.isRunning = false;
                break;
            }

            if (!appState.entryInst->render(appState.entryInst, (f32)delta)) {
                KFATAL("Game render failed, shutting down...");
                appState.isRunning = false;
                break;
            }

            // TODO: refactor packet creation
            GontiRendererPacket packet;
            packet.deltaTime = delta;

            gontiRendererDrawFrame(&packet);

            f64 frameEndTime = gontiVkPlatformGetAbsoluteTime();
            f64 frameElapsedTime = frameEndTime - frameStartTime;

            runningTime += frameElapsedTime;
            f64 remainingSeconds = targetFrameSeconds - frameElapsedTime;

            if (remainingSeconds > 0) {
                u64 remainingMs = (remainingSeconds * 1000);

                if (remainingMs > 0 && isFpsLimit) {
                    gontiPlatformSleep(remainingMs - 1);
                }

                frameCount++;
            }

            gontiInputUpdate(delta);
            appState.lastTime = currentTime;
        }
    }

    appState.isRunning = false;

    if (appState.entryInst->shutdown) appState.entryInst->shutdown(appState.entryInst);
    gontiVkClockStop(&appState.clock);
    gontiEventShutdown();
    gontiInputShutdown();
    gontiRendererShutdown();
    gontiVkPlatformShutdown(&appState.gontiPlatform);

    return true;
}

/*
* VOID
*/
void gontiVkApplicationShutDown() {
    if (appState.isRunning)
        appState.isRunning = false;
}
void gontiVkApplicationPause() {
    if (!appState.isSuspended)
        appState.isSuspended = true;
}
void gontiVkApplicationResume() {
    if (appState.isSuspended)
        appState.isSuspended = false;
}
void gontiVkApplicationGetFramebufferSizePtr(u32* width, u32* height) {
    *width = appState.width;
    *height = appState.height;
}
void gontiVkApplicationGetWidthPtr(u32* width) {
    *width = appState.width;
}
void gontiVkApplicationGetHeightPtr(u32* height) {
    *height = appState.height;
}
void gontiVkApplicationWidthSet(u32 width) {
    appState.width = width;
}
void gontiVkApplicationHeightSet(u32 height) {
    appState.height = height;
}
void gontiVkApplicationOnResized(u32 width, u32 height) {
    appState.entryInst->onResize(appState.entryInst, width, height);
}

/*
* i16
*/
i16 gontiVkApplicationGetWidth() {
    return appState.width;
}
i16 gontiVkApplicationGetHeight() {
    return appState.height;
}

/*
* b8
*/
b8 gontiVkApplicationIsPaused() {
    return appState.isSuspended;
}

#endif