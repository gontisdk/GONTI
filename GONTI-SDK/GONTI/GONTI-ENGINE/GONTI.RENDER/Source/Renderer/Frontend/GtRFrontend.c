#include "GtRFrontend.h"


#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Memory/GtMemory.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Logging/GtLogger.h>
#include "../Backend/GtRBackend.h"

static GtRendererBackend* backend = 0;

GtB8 gontiRendererInitialize(GtU64* memoryRequirement, void* state, const char* appName, struct GtVkPlatformState* platState) {
    *memoryRequirement = sizeof(GtRendererBackend);
    if (!state) return GtTrue;

    backend = state;

    // TODO: make it configurable
    gontiRendererBackendCreate(RENDERER_BACKEND_TYPE_VULKAN, platState, backend);
    backend->frameNumber = 0;

    if (!backend->initialize(appName, platState)) {
        GTFATAL("Renderer backend failed to initialize. Shutting down...");
        return GtFalse;
    }

    return GtTrue;
}
GtB8 gontiRendererBeginFrame(GtF32 deltaTime) {
    return backend->beginFrame(deltaTime);
}
GtB8 gontiRendererEndFrame(GtF32 deltaTime) {
    GtB8 result = backend->endFrame(deltaTime);
    backend->frameNumber++;
    return result;
}
GtB8 gontiRendererDrawFrame(GtRendererPacket* packet) {
    if (gontiRendererBeginFrame(packet->deltaTime)) {
        GtB8 result = gontiRendererEndFrame(packet->deltaTime);

        if (!result) {
            GTERROR("Renderer end frame failed. Application shutting down...");
            return GtFalse;
        }
    }

    return GtTrue;
}

void gontiRendererShutdown(void* state) {
    backend->shutdown();
    backend = 0;
}
void gontiRendererOnResized(GtU16 width, GtU16 height) {
    if (backend) {
        backend->resized(width, height);
    } else {
        GTWARN("Renderer backend does not exist to accept resize: %i:%i", width, height);
    }
}