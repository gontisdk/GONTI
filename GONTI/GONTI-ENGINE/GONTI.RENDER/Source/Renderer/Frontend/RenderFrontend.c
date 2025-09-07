#include "RenderFrontend.h"


#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Memory/Memory.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Logging/Logger.h>
#include "../Backend/RenderBackend.h"

static GontiRendererBackend* backend = 0;

b8 gontiRendererInitialize(const char* appName, struct GontiVulkanPlatformState* platState) {
    backend = k_allocate(sizeof(GontiRendererBackend), GONTI_MEMORY_TAG_RENDERER);

    // TODO: make it configurable
    gontiRendererBackendCreate(RENDERER_BACKEND_TYPE_VULKAN, platState, backend);
    backend->frameNumber = 0;

    if (!backend->initialize(backend, appName, platState)) {
        KFATAL("Renderer backend failed to initialize. Shutting down...");
        return false;
    }

    return true;
}
b8 gontiRendererBeginFrame(f32 deltaTime) {
    return backend->beginFrame(backend, deltaTime);
}
b8 gontiRendererEndFrame(f32 deltaTime) {
    b8 result = backend->endFrame(backend, deltaTime);
    backend->frameNumber++;
    return result;
}
b8 gontiRendererDrawFrame(GontiRendererPacket* packet) {
    if (gontiRendererBeginFrame(packet->deltaTime)) {
        b8 result = gontiRendererEndFrame(packet->deltaTime);

        if (!result) {
            KERROR("Renderer end frame failed. Application shutting down...");
            return false;
        }
    }

    return true;
}

void gontiRendererShutdown() {
    backend->shutdown(backend);
    k_free(backend, sizeof(GontiRendererBackend), GONTI_MEMORY_TAG_RENDERER);
}
void gontiRendererOnResized(u16 width, u16 height) {
    if (backend) {
        backend->resized(backend, width, height);
    } else {
        KWARN("Renderer backend does not exist to accept resize: %i:%i", width, height);
    }
}