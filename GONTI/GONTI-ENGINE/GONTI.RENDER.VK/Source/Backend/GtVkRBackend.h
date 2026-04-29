#ifndef GtVkRBackendH
#define GtVkRBackendH

#ifdef __cplusplus 
    extern "C" {
#endif

        #include <Types/GtVkContextTypes.inl>
        #include <Platform/GtVkPlatform.h>
    
        /* B8 */
        GTAPI GtB8 gontiVkRendererBackendRecreateSwapchain(GtVkContext* context);
        GTAPI GtB8 gontiVkRendererBackendInitialize(const char* appName, GtVkPlatformState* platState);
        GTAPI GtB8 gontiVkRendererBackendBeginFrame(GtF32 deltaTime);
        GTAPI GtB8 gontiVkRendererBackendEndFrame(GtF32 deltaTime);

        /* VOID */
        GTAPI void gontiVkRendererBackendShutdown(void);
        GTAPI void gontiVkRendererBackendOnResized(GtU16 width, GtU16 height);

#ifdef __cplusplus 
    }
#endif

#endif