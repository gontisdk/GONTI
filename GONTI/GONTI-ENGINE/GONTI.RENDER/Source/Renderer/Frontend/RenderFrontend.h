#ifndef RENDERERFRONTEND_H
#define RENDERERFRONTEND_H

#ifdef __cplusplus 
    extern "C" {
#endif

        #include "../../Types/Vulkan/VulkanContextTypes.inl"
        #include "../Types/RenderTypes.inl"
        #include "../../Platform/Vulkan/VulkanPlatform.h"

        struct GontiStaticMeshData;
        struct GontiVulkanPlatformState;

        KAPI b8 gontiRendererInitialize(const char* appName, GontiVulkanPlatformState* platState);
        KAPI b8 gontiRendererBeginFrame(f32 deltaTime);
        KAPI b8 gontiRendererEndFrame(f32 deltaTime);
        KAPI b8 gontiRendererDrawFrame(GontiRendererPacket* packet);
        KAPI void gontiRendererShutdown();
        KAPI void gontiRendererOnResized(u16 width, u16 height);

#ifdef __cplusplus 
    }
#endif

#endif