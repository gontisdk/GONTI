#include "RendererBackend.h"

#ifdef RendererBackend_ON

#include "Vulkan/VulkanRendererBackend.h"
#include "../../../../Utilities/Logging/Scripts/Logger.h"

b8 rendererBackendCreate(gontiRendererBackendType type, struct gontiPlatformState* platState, gontiRendererBackend* outRendererBackend) {
    outRendererBackend->platformState = platState;

    if (type == RENDERER_BACKEND_TYPE_VULKAN) {
        #if defined(USE_VULKAN)
            outRendererBackend->initialize = vulkanRendererBackendInitialize;
            outRendererBackend-> shutdown = vulkanRendererBackendShutdown;
            outRendererBackend->beginFrame = vulkanRendererBackendBeginFrame;
            outRendererBackend->endFrame = vulkanRendererBackendEndFrame;
            outRendererBackend->resized = vulkanRendererBackendOnResized;
        #elif !defined(USE_VULKAN)
            KFATAL("Vulkan backend selected, but engine was not compiled with Vulkan support.");
            return false;
        #endif

        return true;
    } else if (type == RENDERER_BACKEND_TYPE_OPENGL_GLAD) {
        #if defined(USE_OPENGL) && defined(OPENGL_GLAD)

            // TODO: fill for opengl glad
            
        #elif !defined(USE_OPENGL) && !defined(OPENGL_GLAD)
            KFATAL("OpenGL GLAD backend selected, but engine was not compiled with OpenGL GLAD support.");
            return false;
        #endif

        return true;
    } else if (type == RENDERER_BACKEND_TYPE_OPENGL_GLEW) {
        #if defined(USE_OPENGL) && defined(OPENGL_GLEW)

            // TODO: fill for opengl glew

        #elif !defined(USE_OPENGL) && !defined(OPENGL_GLEW)
            KFATAL("OpenGL GLEW backend selected, but engine was not compiled with OpenGL GLEW support.");
            return false;
        #endif

        return true;
    } else if (type == RENDERER_BACKEND_TYPE_DIRECTX_11) {
        #if defined(USE_DIRECTX) && defined(DIRECTX_X11)
            
            // TODO: fill for directx 11

        #elif !defined(USE_DIRECTX) && !defined(DIRECTX_X11)
            KFATAL("DirectX 11 backend selected, but engine was not compiled with DirectX 11 support.");
            return false;
        #endif

        return true;
    } else if (type == RENDERER_BACKEND_TYPE_DIRECTX_12) {
        #if defined(USE_DIRECTX) && defined(DIRECTX_X12)

            // TODO: fill for directx 12

        #elif !defined(USE_DIRECTX) && !defined(DIRECTX_X12)
            KFATAL("DirectX 12 backend selected, but engine was not compiled with DirectX 12 support.");
            return false;
        #endif

        return true;
    }

    KFATAL("Not supported backend selected.");
    return false;
}

void rendererBackendDestroy(gontiRendererBackend* rendererBackend) {
    rendererBackend->initialize = 0;
    rendererBackend->shutdown = 0;
    rendererBackend-> beginFrame = 0;
    rendererBackend->endFrame = 0;
    rendererBackend->resized = 0;
}

#endif