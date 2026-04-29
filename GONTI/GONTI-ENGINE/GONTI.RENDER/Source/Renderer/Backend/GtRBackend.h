#ifndef GtRBackendH
#define GtRBackendH

#ifdef __cplusplus 
    extern "C" {
#endif

        #include <GONTI-ENGINE/GONTI.RENDER.VK/Source/Types/GtVkContextTypes.inl>
        #include <Renderer/Types/GtRTypes.inl>

        struct GtVkPlatformState;

        GTAPI GtB8 gontiRendererBackendCreate(GtRendererBackendType type, struct GtVkPlatformState* platState, GtRendererBackend* outRendererBackend);
        GTAPI void gontiRendererBackendDestroy(GtRendererBackend* rendererBackend);

        #endif

#ifdef __cplusplus 
    }
#endif