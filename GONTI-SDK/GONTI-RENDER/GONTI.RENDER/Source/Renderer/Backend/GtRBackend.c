#include <Renderer/Backend/GtRBackend.h>

#include <GONTI-RENDER/GONTI-RENDER-VK/GONTI.RENDER.VK/Source/Backend/GtVkRBackend.h>
#include <GONTI-CORE/GONTI.CORE/Source/Logging/GtLogger.h>
#include <Config/SetupGraphicBackend/GtUsrSetGraphBackend.h>

GtB8 gontiRendererBackendCreate(GtRendererBackendType type, GtVkPlatformState* platState, GtRendererBackend* outRendererBackend) {
    if (type == RENDERER_BACKEND_TYPE_VULKAN) {
        #if GONTI_USE_VULKAN
            outRendererBackend->initialize = gontiVkRenderBackendInitialize;
            outRendererBackend->shutdown = gontiVkRenderBackendShutdown;
            outRendererBackend->beginFrame = gontiVkRenderBackendBeginFrame;
            outRendererBackend->updateGlobalStage = gontiVkRenderBackendUpdateGlobalStage;
            outRendererBackend->endFrame = gontiVkRenderBackendEndFrame;
            outRendererBackend->resized = gontiVkRenderBackendOnResized;
        #elif !GONTI_USE_VULKAN
            GTFATAL("Vulkan backend selected, but engine was not compiled with Vulkan support.");
            return GtFalse;
        #endif

        return GtTrue;
    } else if (type == RENDERER_BACKEND_TYPE_OPENGL) {
        #if GONTI_USE_OPENGL

            // TODO: fill for opengl glad
            
        #elif !GONTI_USE_OPENGL
            GTFATAL("OpenGL backend selected, but engine was not compiled with OpenGL support.");
            return GtFalse;
        #endif

        return GtTrue;
    } else if (type == RENDERER_BACKEND_TYPE_DIRECTX_11) {
        #if GONTI_USE_DIRECTX_11
            
            // TODO: fill for directx 11

        #elif !GONTI_USE_DIRECTX_11
            GTFATAL("DirectX 11 backend selected, but engine was not compiled with DirectX 11 support.");
            return GtFalse;
        #endif

        return GtTrue;
    } else if (type == RENDERER_BACKEND_TYPE_DIRECTX_12) {
        #if GONTI_USE_DIRECTX_12

            // TODO: fill for directx 12

        #elif !GONTI_USE_DIRECTX_12
            GTFATAL("DirectX 12 backend selected, but engine was not compiled with DirectX 12 support.");
            return GtFalse;
        #endif

        return GtTrue;
    }

    GTFATAL("Not supported backend selected.");
    return GtFalse;
}

void gontiRendererBackendDestroy(GtRendererBackend* rendererBackend) {
    rendererBackend->initialize = 0;
    rendererBackend->shutdown = 0;
    rendererBackend->beginFrame = 0;
    rendererBackend->updateGlobalStage = 0;
    rendererBackend->endFrame = 0;
    rendererBackend->resized = 0;
}