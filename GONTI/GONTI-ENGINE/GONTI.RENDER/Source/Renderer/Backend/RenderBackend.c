#include "RenderBackend.h"

#include "Vulkan/VulkanRenderBackend.h"
#include "../../Config/SetupGraphicBackend/UserSetGraphicBackend.h"
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Logging/Logger.h>

b8 gontiRendererBackendCreate(GontiRendererBackendType type, struct GontiVulkanPlatformState* platState, GontiRendererBackend* outRendererBackend) {
    outRendererBackend->GontiVulkanPlatformState = platState;

    if (type == RENDERER_BACKEND_TYPE_VULKAN) {
        #if GONTI_USE_VULKAN
            outRendererBackend->initialize = gontiVkRendererBackendInitialize;
            outRendererBackend-> shutdown = gontiVkRendererBackendShutdown;
            outRendererBackend->beginFrame = gontiVkRendererBackendBeginFrame;
            outRendererBackend->endFrame = gontiVkRendererBackendEndFrame;
            outRendererBackend->resized = gontiVkRendererBackendOnResized;
        #elif !GONTI_USE_VULKAN
            KFATAL("Vulkan backend selected, but engine was not compiled with Vulkan support.");
            return false;
        #endif

        return true;
    } else if (type == RENDERER_BACKEND_TYPE_OPENGL) {
        #if GONTI_USE_OPENGL

            // TODO: fill for opengl glad
            
        #elif !GONTI_USE_OPENGL
            KFATAL("OpenGL backend selected, but engine was not compiled with OpenGL support.");
            return false;
        #endif

        return true;
    } else if (type == RENDERER_BACKEND_TYPE_DIRECTX_11) {
        #if GONTI_USE_DIRECTX_11
            
            // TODO: fill for directx 11

        #elif !GONTI_USE_DIRECTX_11
            KFATAL("DirectX 11 backend selected, but engine was not compiled with DirectX 11 support.");
            return false;
        #endif

        return true;
    } else if (type == RENDERER_BACKEND_TYPE_DIRECTX_12) {
        #if GONTI_USE_DIRECTX_12

            // TODO: fill for directx 12

        #elif !GONTI_USE_DIRECTX_12
            KFATAL("DirectX 12 backend selected, but engine was not compiled with DirectX 12 support.");
            return false;
        #endif

        return true;
    }

    KFATAL("Not supported backend selected.");
    return false;
}

void gontiRendererBackendDestroy(GontiRendererBackend* rendererBackend) {
    rendererBackend->initialize = 0;
    rendererBackend->shutdown = 0;
    rendererBackend-> beginFrame = 0;
    rendererBackend->endFrame = 0;
    rendererBackend->resized = 0;
}