#ifndef GtRFrontendH
#define GtRFrontendH

#ifdef __cplusplus 
    extern "C" {
#endif

        #include <GONTI/GONTI-ENGINE/GONTI.RENDER.VK/Source/Platform/GtVkPlatform.h>
        #include <GONTI/GONTI-ENGINE/GONTI.RENDER.VK/Source/Types/GtVkContextTypes.inl>
        #include "../Types/GtRTypes.inl"

        struct GontiStaticMeshData;
        struct GtVkPlatformState;

        GTAPI GtB8 gontiRendererInitialize(GtU64* memoryRequirement, void* state, const char* appName, GtVkPlatformState* platState);
        GTAPI GtB8 gontiRendererBeginFrame(GtF32 deltaTime);
        GTAPI GtB8 gontiRendererEndFrame(GtF32 deltaTime);
        GTAPI GtB8 gontiRendererDrawFrame(GtRendererPacket* packet);
        GTAPI void gontiRendererShutdown(void* state);
        GTAPI void gontiRendererOnResized(GtU16 width, GtU16 height);

#ifdef __cplusplus 
    }
#endif

#endif