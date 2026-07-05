#include <Renderer/Frontend/GtRFrontend.h>

#include <GONTI-CORE/GONTI.CORE/Source/Memory/GtMemory.h>
#include <GONTI-CORE/GONTI.CORE/Source/Logging/GtLogger.h>
#include <GONTI-CORE/GONTI.MATH/Source/Algebra/Matrices/Matrix4/GtMatrix4.h>
#include <GONTI-CORE/GONTI.MATH/Source/Algebra/Vectors/Vector3/GtVector3.h>
#include <GONTI-CORE/GONTI.MATH/Source/Algebra/Vectors/Vector4/GtVector4.h>
#include <GONTI-CORE/GONTI.MATH/Source/Algebra/Quaternions/Quaternion/GtQuaternion.h>
#include <GONTI-CORE/GONTI.CORE/Source/Defines/GtDefines.inl>
#include <GONTI-CORE/GONTI.MATH/Source/Algebra/Vectors/GtVecTypes.inl>
#include <Renderer/Backend/GtRBackend.h>

static GtRendererBackend* backend = 0;

GtB8 gontiRendererInitialize(GtU64* memoryRequirement, void* state, const char* appName, GtVkPlatformState* platState) {
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
        GtMat4 projection = gontiMat4Perspective(gontiQuatDegToRad(45.0f), 1280/720.0f, 0.1f, 1000.0f); 
        static GtF32 z = -1.0f;
        z -= 0.01f;
        GtMat4 view = gontiMat4Translation((GtVec3){.elements = {0, 0, z}});        
        
        backend->updateGlobalStage(projection, view, gontiVec3Zero(), gontiVec4One(), 0);

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
