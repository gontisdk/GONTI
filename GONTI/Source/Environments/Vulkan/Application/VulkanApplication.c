#include "../../../__GONTI_INCLUDES.h"

#ifdef USE_VULKAN
#ifdef ApplicationVulkan_ON

    #include "VulkanApplication.h"
    #include "../../../Core/Platform/Platform.h"
    #include "../../../Core/Platform/Vulkan/VulkanPlatform.h"
    #include "../../../Utilities/Logging/Scripts/Logger.h"
    #include "../../../Core/EntryPoint/Vulkan/VulkanEntryTypes.h"
    #include "../../../Core/EntryPoint/Vulkan/VulkanEntry.h"
    #include "../../../Core/MemorySubsystem/MemorySubsystem.h"
    #include "../../../Events/Events.h"
    #include "../../../Events/InputSystem/Vulkan/VulkanInputs.h"
    #include "../Clock/Clock.h"
    #include "../../../Core/RendererSubsystem/Renderer/Frontend/RendererFrontend.h"

    typedef struct ApplicationState {
        b8 isRunning;
        b8 isSuspended;
        gontiPlatformState platform;
        gontiVkEntry* entryInst;
        i16 width;
        i16 height;
        Clock clock;
        f64 lastTime;
    }ApplicationState;

    static b8 initialized = false;
    static ApplicationState appState;

    /*
    * BOOL
    */
    b8 applicationCreate(gontiVkEntry* entryInst) {
        if (initialized) {
            KERROR("applicationCreate() method called more than once.");
            return false;
        }

        appState.entryInst = entryInst;

        logger_initializeLogging();
        inputInitialize();

        appState.isRunning = true;
        appState.isSuspended = false;

        if (!eventInitialize()) {
            KERROR("Event system failed initialization. Application can not continue");
            return false;
        }

        if (!platformStartup(&appState.platform, 
            entryInst->appConfig.className,
            entryInst->appConfig.appName, 
            entryInst->appConfig.startPosX, 
            entryInst->appConfig.startPosY,
            entryInst->appConfig.startWidth, 
            entryInst->appConfig.startHeight)) 
                return false;

        if (!rendererInitialize(entryInst->appConfig.appName, &appState.platform)) {
            KFATAL("Failed to initialize renderer. Aborting application...");
            return false;
        }

        if (!appState.entryInst->initialize(appState.entryInst)) {
            KFATAL("Game failed to initialize");
            return false;
        }

        appState.entryInst->onResize(appState.entryInst, appState.width, appState.height);
        if (appState.entryInst->registerEvents != NULL) appState.entryInst->registerEvents();

        initialized = true;
        
        return true;
    }
    b8 applicationRun(u64 fpsLimit, b8 isFpsLimit) {
        if (fpsLimit <= 0) isFpsLimit = false;

        clockStart(&appState.clock);
        clockUpdate(&appState.clock);
        appState.lastTime = appState.clock.elapsed;

        f64 runningTime = 0;
        u8 frameCount = 0;
        f64 targetFrameSeconds = 1.0f / fpsLimit;

        KINFO(k_getMemoryUsageStr());

        while (appState.isRunning) {
            if (!platformPumpMessage(&appState.platform)) appState.isRunning = false;

            if (!appState.isSuspended) {
                clockUpdate(&appState.clock);
                f64 currentTime = appState.clock.elapsed;
                f64 delta = (currentTime - appState.lastTime);
                f64 frameStartTime = platformGetAbsoluteTime();

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
                gontiRendererPacket packet;
                packet.deltaTime = delta;
                rendererDrawFrame(&packet);

                f64 frameEndTime = platformGetAbsoluteTime();
                f64 frameElapsedTime = frameEndTime - frameStartTime;
                runningTime += frameElapsedTime;
                f64 remainingSeconds = targetFrameSeconds - frameElapsedTime;

                if (remainingSeconds > 0) {
                    u64 remainingMs = (remainingSeconds * 1000);

                    if (remainingMs > 0 && isFpsLimit) {
                        platformSleep(remainingMs - 1);
                    }

                    frameCount++;
                }

                inputUpdate(delta);

                appState.lastTime = currentTime;
            }
        }

        appState.isRunning = false;

        if (appState.entryInst->unregisterEvents != NULL) appState.entryInst->unregisterEvents();
        eventShutdown();
        inputShutdown();
        rendererShutdown();
        platformShutdown(&appState.platform);

        return true;
    }

    /*
    * VOID
    */
    void applicationShutDown() {
        if (appState.isRunning)
            appState.isRunning = false;
    }
    void applicationPause() {
        if (!appState.isSuspended)
            appState.isSuspended = true;
    }
    void applicationResume() {
        if (appState.isSuspended)
            appState.isSuspended = false;
    }
    void applicationGetFramebufferSize(u32* width, u32* height) {
        *width = appState.width;
        *height = appState.height;
    }

#endif
#endif