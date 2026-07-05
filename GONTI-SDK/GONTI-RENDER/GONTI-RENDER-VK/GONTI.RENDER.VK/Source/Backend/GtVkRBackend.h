#ifndef GtVkRBackendH
#define GtVkRBackendH

#ifdef __cplusplus 
    extern "C" {
#endif

        #include <GONTI-RENDER/GONTI-RENDER-VK/GONTI.RENDER.COMMON.VK/Source/Types/GtVkContextTypes.inl>
        #include <Platform/GtVkPlatform.h>
        #include <GONTI-CORE/GONTI.MATH/Source/Algebra/Vectors/GtVecTypes.inl>
        #include <GONTI-CORE/GONTI.MATH/Source/Algebra/Matrices/GtMatTypes.inl>
    
        /* B8 */
        GTAPI GtB8 gontiVkRenderBackendRecreateSwapchain(GtVkContext* context);
        GTAPI GtB8 gontiVkRenderBackendInitialize(const char* appName, struct GtVkPlatformState* platState);
        GTAPI GtB8 gontiVkRenderBackendBeginFrame(GtF32 deltaTime);
        GTAPI GtB8 gontiVkRenderBackendEndFrame(GtF32 deltaTime);

        /* VOID */
        GTAPI void gontiVkRenderBackendShutdown(void);
        GTAPI void gontiVkRenderBackendOnResized(GtU16 width, GtU16 height);
        GTAPI void gontiVkRenderBackendUpdateGlobalStage(GtMat4 projection, GtMat4 view, GtVec3 viewPosition, GtVec4 ambientColour, GtI32 mode);

#ifdef __cplusplus 
    }
#endif

#endif