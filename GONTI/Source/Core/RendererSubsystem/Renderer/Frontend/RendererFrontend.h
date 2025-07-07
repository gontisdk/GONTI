#ifndef RENDERERFRONTEND_H
#define RENDERERFRONTEND_H

#ifdef __cplusplus 
    extern "C" {
#endif

        #include "../../../../__GONTI_INCLUDES.h"

        #ifdef RendererFrontend_ON

            #include "../../Types/RendererTypes.inl"

            struct StaticMeshData;
            struct gontiPlatformState;

            KAPI b8 rendererInitialize(const char* appName, struct gontiPlatformState* platState);
            KAPI b8 rendererDrawFrame(gontiRendererPacket* packet);
            KAPI void rendererShutdown();
            KAPI void rendererOnResized(u16 width, u16 height);

        #endif

#ifdef __cplusplus 
    }
#endif

#endif