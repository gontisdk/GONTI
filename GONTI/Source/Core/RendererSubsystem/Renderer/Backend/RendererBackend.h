#ifndef RENDERERBACKEND_H
#define RENDERERBACKEND_H

#ifdef __cplusplus 
    extern "C" {
#endif

        #include "../../../../__GONTI_INCLUDES.h"

        #ifdef RendererBackend_ON

            #include "../../Types/RendererTypes.inl"

            struct gontiPlatformState;

            KAPI b8 rendererBackendCreate(gontiRendererBackendType type, struct gontiPlatformState* platState, gontiRendererBackend* outRendererBackend);
            KAPI void rendererBackendDestroy(gontiRendererBackend* rendererBackend);

        #endif

#ifdef __cplusplus 
    }
#endif

#endif