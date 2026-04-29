#include <Application/Vulkan/GtVkApp.h>

#if GONTI_USE_VULKAN

#include <GONTI-ENGINE/GONTI.CORE/Source/Logging/GtLogger.h>
#include <GONTI-ENGINE/GONTI.CORE/Source/Memory/GtMemory.h>
#include <GONTI-ENGINE/GONTI.CORE/Source/Platform/GtPlatform.h>
#include <GONTI-ENGINE/GONTI.CORE/Source/LinearAllocator/GtLinearAlloc.h>
#include <GONTI-ENGINE/GONTI.RENDER.VK/Source/Platform/GtVkPlatform.h>
#include <GONTI-ENGINE/GONTI.RENDER.VK/Source/Backend/GtVkRBackend.h>
#include <GONTI-ENGINE/GONTI.RENDER/Source/Renderer/Frontend/GtRFrontend.h>
#include <Clock/GtClock.h>
#include <Events/Platform/GtPlatformEvents.h>
#include <Events/GtEvents.h>
#include <Inputs/GtInputs.h>
#include <Events/System/GtSystemEvents.h>

typedef struct GtVkAppState {
    GtB8 isRunning;
    GtB8 isSuspended;
    GtB8 isPaused;
    GtEntry* entryInst;
    GtI16 width;
    GtI16 height;
    GtClock clock;
    GtF64 lastTime;
    GtLinearAlloc systemAllocator;

    GtU64 loggingSystemMemoryRequirement;
    void* loggingSystemState;

    GtU64 inputSystemMemoryRequirement;
    void* inputSystemState;

    GtU64 eventSystemMemoryRequirement;
    void* eventSystemState;

    GtU64 platformSystemMemoryRequirement;
    void* platformSystemState;

    GtU64 rendererSystemMemoryRequirement;
    void* rendererSystemState;
}GtVkAppState;
static GtVkAppState* appState;

/*
* B8
*/
GtB8 gontiVkApplicationCreate(GtEntry* entryInst) {
    if (entryInst->GtAppState) {
        GTERROR("gontiVkApplicationCreate() method called more than once.");
        return GtFalse;
    }

    GTINFO("Creating application state...");
    entryInst->GtAppState = gt_allocate(sizeof(GtVkAppState), GT_MEM_TAG_APPLICATION);
    appState = entryInst->GtAppState;
    appState->entryInst = entryInst;
    appState->isRunning = GtFalse;
    appState->isSuspended = GtFalse;

    GTINFO("Creating system allocator...");
    GtU64 systemAllocatorTotalSize = 64 * 1024*1024; // NOTE: 64MB
    gontiLinearAllocatorCreate(systemAllocatorTotalSize, 0, &appState->systemAllocator);

    GTINFO("Creating logger system...");
    gontiLoggerInitializeLogging(&appState->loggingSystemMemoryRequirement, 0);
    appState->loggingSystemState = gontiLinearAllocatorAllocate(&appState->systemAllocator, appState->loggingSystemMemoryRequirement);
    if (!gontiLoggerInitializeLogging(&appState->loggingSystemMemoryRequirement, appState->loggingSystemState)) {
        GTERROR("Failed to initialize logging system. Shutting down.");
        return GtFalse;
    }

    GTINFO("Creating input system...");
    gontiInputInitialize(&appState->inputSystemMemoryRequirement, 0);
    appState->inputSystemState = gontiLinearAllocatorAllocate(&appState->systemAllocator, appState->inputSystemMemoryRequirement);
    gontiInputInitialize(&appState->inputSystemMemoryRequirement, appState->inputSystemState);

    GTINFO("Creating event system...");
    gontiEventInitialize(&appState->eventSystemMemoryRequirement, 0);
    appState->eventSystemState = gontiLinearAllocatorAllocate(&appState->systemAllocator, appState->eventSystemMemoryRequirement);
    if (!gontiEventInitialize(&appState->eventSystemMemoryRequirement, appState->eventSystemState)) {
        GTERROR("Event system failed initialization. Application can not continue");
        return GtFalse;
    }

    GTINFO("Creating platform...");
    gontiVkPlatformStartup(&appState->platformSystemMemoryRequirement, 0, 0, 0, 0, 0, 0, 0);
    appState->platformSystemState = gontiLinearAllocatorAllocate(&appState->systemAllocator, appState->platformSystemMemoryRequirement);
    GtVkPlatformState* platState = (GtVkPlatformState*)appState->platformSystemState;

    #if GTPLATFORM_WINDOWS
        platState->gontiVkInternalStateWin32.win32_process_message = WndProc;
    #endif
    #if GTPLATFORM_LINUX
        platState->gontiVkInternalStateLinux.linux_process_message = LinuxProc;
    #endif

    if (!platState->get_frame_buffer_size_ptr)
        platState->get_frame_buffer_size_ptr = gontiVkApplicationGetFramebufferSizePtr;

    GTINFO("Creating window...");
    if (!gontiVkPlatformStartup(
        &appState->platformSystemMemoryRequirement,
        appState->platformSystemState,
        entryInst->windowConfig.windowName,
        entryInst->windowConfig.className,
        entryInst->windowConfig.startPosX, 
        entryInst->windowConfig.startPosY,
        entryInst->windowConfig.startWidth, 
        entryInst->windowConfig.startHeight
    )) {
        GTFATAL("Window failed to create.");
        return GtFalse;
    }

    GTINFO("Creating render...");
    gontiRendererInitialize(&appState->rendererSystemMemoryRequirement, 0, 0, 0);
    appState->rendererSystemState = gontiLinearAllocatorAllocate(&appState->systemAllocator, appState->rendererSystemMemoryRequirement);
    if (!gontiRendererInitialize(&appState->rendererSystemMemoryRequirement, appState->rendererSystemState, entryInst->windowConfig.windowName, platState)) {
        GTFATAL("Failed to initialize renderer. Aborting gontiVkApplication...");
        return GtFalse;
    }

    if (!appState->entryInst->disableSystemEvents) {
        gontiEventRegister(GT_EVENT_CODE_KEY_PRESSED, 0, gontiSystemEventsOnKey);
        gontiEventRegister(GT_EVENT_CODE_KEY_RELEASED, 0, gontiSystemEventsOnKey);
        gontiEventRegister(GT_EVENT_CODE_BUTTON_PRESSED, 0, gontiSystemEventsOnButton);
        gontiEventRegister(GT_EVENT_CODE_BUTTON_RELEASED, 0, gontiSystemEventsOnButton);
        gontiEventRegister(GT_EVENT_CODE_MOUSE_WHEEL, 0, gontiSystemEventsOnMouseWheel);
        gontiEventRegister(GT_EVENT_CODE_MOUSE_MOVED, 0, gontiSystemEventsOnMouseMove);
        gontiEventRegister(GT_EVENT_CODE_RESIZED, 0, gontiSystemEventsOnResized);
    }

    if (!appState->entryInst->GtInitializeFN(appState->entryInst)) {
        GTFATAL("Game failed to initialize");
        return GtFalse;
    }

    if (appState->entryInst->GtOnResizeFN != NULL) appState->entryInst->GtOnResizeFN(appState->entryInst, appState->width, appState->height);

    return GtTrue;
}
GtB8 gontiVkApplicationRun(GtU64 fpsLimit, GtB8 isFpsLimit) {
    appState->isRunning = GtTrue;

    if (fpsLimit <= 0) {
        fpsLimit = 1;
        isFpsLimit = GtFalse;
    }

    gontiClockStart(&appState->clock);
    gontiClockUpdate(&appState->clock);
    appState->lastTime = appState->clock.elapsed;

    const GtF64 fixedDelta = 1.0 / 60.0;
    GtF64 accumulator = 0.0;
    GtF64 targetFrameSeconds = 1.0f / fpsLimit;

    GtF64 runningTime = 0;
    GtU8 frameCount = 0;

    char* memUsage = gontiGetMemoryUsageStr();
    GTINFO(memUsage);
    gt_free(memUsage);

    while (appState->isRunning) {
        if (!gontiVkPlatformPumpMessage(appState->platformSystemState)) appState->isRunning = GtFalse;

        gontiClockUpdate(&appState->clock);
        GtF64 currentTime = appState->clock.elapsed;
        GtF64 delta = (currentTime - appState->lastTime);
        delta = GT_CLAMP(delta, 0.0, 0.25);
        GtF64 frameStartTime = gontiPlatformGetAbsoluteTime();

        if (!appState->isSuspended) {
            if (!appState->entryInst->GtUpdateFN(appState->entryInst, (GtF32)delta)) {
                GTFATAL("Game update failed, shutting down...");
                appState->isRunning = GtFalse;
                break;
            }

            if (!appState->isPaused) {
                accumulator += delta;

                while (accumulator >= fixedDelta) {
                    if (appState->entryInst->GtFixedUpdateFN) {
                        appState->entryInst->GtFixedUpdateFN(appState->entryInst, (GtF32)fixedDelta);
                    }
                    accumulator -= fixedDelta;
                }
            } else accumulator = 0;

            GtF32 alpha = (GtF32)(accumulator / fixedDelta);
            if (!appState->entryInst->GtRenderFN(appState->entryInst, (GtF32)delta, (GtF32)alpha)) {
                GTFATAL("Game render failed, shutting down...");
                appState->isRunning = GtFalse;
                break;
            }

            // TODO: refactor packet creation
            GtRendererPacket packet;
            packet.deltaTime = delta;
            packet.interpolation = alpha;
            gontiRendererDrawFrame(&packet);
        } else accumulator = 0;
        
        gontiInputUpdate(delta);

        GtF64 frameEndTime = gontiPlatformGetAbsoluteTime();
        GtF64 frameElapsedTime = frameEndTime - frameStartTime;

        runningTime += frameElapsedTime;
        GtF64 remainingSeconds = targetFrameSeconds - frameElapsedTime;

        if (remainingSeconds > 0) {
            GtU64 remainingMs = (remainingSeconds * 1000);

            if (remainingMs > 0 && isFpsLimit) {
                gontiPlatformSleep(remainingMs - 1);
            }

            frameCount++;
        }

        (void)runningTime;
        (void)frameCount;
        appState->lastTime = currentTime;
    }

    appState->isRunning = GtFalse;

    if (appState->entryInst->GtShutdownFN) appState->entryInst->GtShutdownFN(appState->entryInst);
    gontiClockStop(&appState->clock);

    if (!appState->entryInst->disableSystemEvents) {
        gontiEventUnregister(GT_EVENT_CODE_KEY_PRESSED, 0, gontiSystemEventsOnKey);
        gontiEventUnregister(GT_EVENT_CODE_KEY_RELEASED, 0, gontiSystemEventsOnKey);
        gontiEventUnregister(GT_EVENT_CODE_BUTTON_PRESSED, 0, gontiSystemEventsOnButton);
        gontiEventUnregister(GT_EVENT_CODE_BUTTON_RELEASED, 0, gontiSystemEventsOnButton);
        gontiEventUnregister(GT_EVENT_CODE_MOUSE_WHEEL, 0, gontiSystemEventsOnMouseWheel);
        gontiEventUnregister(GT_EVENT_CODE_MOUSE_MOVED, 0, gontiSystemEventsOnMouseMove);
        gontiEventUnregister(GT_EVENT_CODE_RESIZED, 0, gontiSystemEventsOnResized);
    }
    
    GTINFO("Shutting down render...");
    gontiRendererShutdown(appState->rendererSystemState);
    GTINFO("Destroying window...");
    gontiVkPlatformShutdown(&appState->platformSystemState);
    GTINFO("Destroying event system...");
    gontiEventShutdown(appState->eventSystemState);
    GTINFO("Destroying input system...");
    gontiInputShutdown(appState->inputSystemState); 
    GTINFO("Destroying logger system...");
    gontiLoggerShutdown(appState->loggingSystemState);
    GTINFO("Destroying system allocator...");
    gontiLinearAllocatorDestroy(&appState->systemAllocator);

    GTINFO("Destroying application state...");
    gt_free(appState->entryInst->GtAppState);
    gt_free(appState->entryInst->GtState);

    appState = 0;
    return GtTrue;
}

/*
* VOID
*/
void gontiVkApplicationShutdown(void) {
    if (appState->isRunning)
        appState->isRunning = GtFalse;
}
void gontiVkApplicationPause(void) {
    if (!appState->isPaused)
        appState->isPaused = GtTrue;
}
void gontiVkApplicationResume(void) {
    if (appState->isPaused)
        appState->isPaused = GtFalse;
}
void gontiVkApplicationSuspend(void) {
    if (!appState->isSuspended)
        appState->isSuspended = GtTrue;
}
void gontiVkApplicationUnSuspend(void) {
    if (appState->isSuspended)
        appState->isSuspended = GtFalse;
}
void gontiVkApplicationGetFramebufferSizePtr(GtU32* width, GtU32* height) {
    *width = appState->width;
    *height = appState->height;
}
void gontiVkApplicationGetWidthPtr(GtU32* width) {
    *width = appState->width;
}
void gontiVkApplicationGetHeightPtr(GtU32* height) {
    *height = appState->height;
}
void gontiVkApplicationSetWidth(GtU32 width) {
    appState->width = width;
}
void gontiVkApplicationSetHeight(GtU32 height) {
    appState->height = height;
}
void gontiVkApplicationOnResized(GtU32 width, GtU32 height) {
    appState->entryInst->GtOnResizeFN(appState->entryInst, width, height);
}

/*
* GtI16
*/
GtI16 gontiVkApplicationGetWidth(void) {
    return appState->width;
}
GtI16 gontiVkApplicationGetHeight(void) {
    return appState->height;
}

/*
* GtB8
*/
GtB8 gontiVkApplicationIsSuspended(void) {
    return appState->isSuspended;
}
GtB8 gontiVkApplicationIsPaused(void) {
    return appState->isPaused;
}

#endif