#ifndef GtVkFrameBufferH
#define GtVkFrameBufferH

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../../Types/GtVkContextTypes.inl"

        GTAPI void gontiVkFramebufferCreate(
            GtVkContext* context,
            GtVkRenderpass* renderpass,
            GtU32 width, GtU32 height,
            GtU32 attachmentCount,
            VkImageView* attachments,
            GtVkFramebuffer* outFramebuffer
        );
        GTAPI void gontiVkFramebufferDestroy(GtVkContext* context, GtVkFramebuffer* framebuffer);
        GTAPI void gontiVkFramebuffersRegenerate(GtVkContext* context, GtVkSwapchain* swapchain, GtVkRenderpass* renderpass);

#ifdef __cplusplus
    }
#endif

#endif