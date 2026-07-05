#ifndef GtVkFrameBufferH
#define GtVkFrameBufferH

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI-CORE/GONTI.CORE/Source/Defines/GtDefines.inl>
        #include <Subsystems/Types/GtVkFrameBufferTypes.inl>
        #include <Subsystems/Types/GtVkRenderpassTypes.inl>
        #include <Subsystems/Types/GtVkSwapchainTypes.inl>
        #include <GONTI-RENDER/GONTI-RENDER-VK/GONTI.RENDER.COMMON.VK/Source/Types/GtVkContextTypes.inl>

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