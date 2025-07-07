#include "RendererFrontend.h"

#ifdef RendererFrontend_ON

#include "../../../MemorySubsystem/MemorySubsystem.h"
#include "../../../../Utilities/Logging/Scripts/Logger.h"
#include "../Backend/RendererBackend.h"

static gontiRendererBackend* backend = 0;

b8 rendererInitialize(const char* appName, struct gontiPlatformState* platState) {
    backend = k_allocate(sizeof(gontiRendererBackend), MEMORY_TAG_RENDERER);

    // TODO: make it configurable
    rendererBackendCreate(RENDERER_BACKEND_TYPE_VULKAN, platState, backend);
    backend->frameNumber = 0;

    if (!backend->initialize(backend, appName, platState)) {
        KFATAL("Renderer backend failed to initialize. Shutting down...");
        return false;
    }

    return true;
}
b8 rendererBeginFrame(f32 deltaTime) {
    return backend->beginFrame(backend, deltaTime);
}
b8 rendererEndFrame(f32 deltaTime) {
    b8 result = backend->endFrame(backend, deltaTime);
    backend->frameNumber++;
    return result;
}
b8 rendererDrawFrame(gontiRendererPacket* packet) {
    if (rendererBeginFrame(packet->deltaTime)) {
        b8 result = rendererEndFrame(packet->deltaTime);

        if (!result) {
            KERROR("Renderer end frame failed. Application shutting down...");
            return false;
        }
    }

    return true;
}

void rendererShutdown() {
    backend->shutdown(backend);
    k_free(backend, sizeof(gontiRendererBackend), MEMORY_TAG_RENDERER);
}
void rendererOnResized(u16 width, u16 height);

#endif