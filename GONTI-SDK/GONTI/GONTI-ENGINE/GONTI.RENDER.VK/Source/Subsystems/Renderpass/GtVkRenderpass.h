#ifndef GtVkRenderpassH
#define GtVkRenderpassH

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../../Types/GtVkContextTypes.inl"

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