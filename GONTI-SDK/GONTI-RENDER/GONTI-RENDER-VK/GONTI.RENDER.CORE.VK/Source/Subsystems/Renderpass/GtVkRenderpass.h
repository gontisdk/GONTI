#ifndef GtVkRenderpassH
#define GtVkRenderpassH

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI-CORE/GONTI.CORE/Source/Defines/GtDefines.inl>
        #include <Subsystems/Types/GtVkCmdBufferTypes.inl>
        #include <Subsystems/Types/GtVkRenderpassTypes.inl>
        #include <Subsystems/Types/GtVkFrameBufferTypes.inl>
        #include <GONTI-RENDER/GONTI-RENDER-VK/GONTI.RENDER.COMMON.VK/Source/Types/GtVkContextTypes.inl>

        GTAPI void gontiVkRenderpassCreate(
            GtVkContext* context, GtVkRenderpass* outRenderpass,
            GtF32 x, GtF32 y, GtF32 w, GtF32 h,
            GtF32 r, GtF32 g, GtF32 b, GtF32 a,
            GtF32 depth, GtU32 stencil
        );
        GTAPI void gontiVkRenderpassBegin(
            GtVkCmdBuffer* commandBuffer,
            GtVkRenderpass* renderpass,
            VkFramebuffer frameBuffer
        );
        GTAPI void gontiVkRenderpassEnd(GtVkCmdBuffer* commandBuffer, GtVkRenderpass* renderpass);
        GTAPI void gontiVkRenderpassDestroy(GtVkContext* context, GtVkRenderpass* renderpass);

#ifdef __cplusplus
    }
#endif

#endif