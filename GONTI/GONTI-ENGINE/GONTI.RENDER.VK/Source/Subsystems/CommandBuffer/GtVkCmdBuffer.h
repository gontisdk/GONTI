#ifndef GtVkCmdBufferH
#define GtVkCmdBufferH

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI-ENGINE/GONTI.CORE/Source/Defines/GtDefines.inl>
        #include <Types/GtVkContextTypes.inl>

        GTAPI void gontiVkCommandBufferAllocate(
            GtVkContext* context,
            VkCommandPool pool,
            GtB8 isPrimary,
            GtVkCmdBuffer* outCommandBuffer
        );
        GTAPI void gontiVkCommandBufferFree(
            GtVkContext* context,
            VkCommandPool pool,
            GtVkCmdBuffer* commandBuffer
        );
        GTAPI void gontiVkCommandBufferBegin(
            GtVkCmdBuffer* commandBuffer,
            GtB8 isSingleUse,
            GtB8 isRenderpassContinue,
            GtB8 isSimultaneous
        );
        GTAPI void gontiVkCommandBufferAllocateAndBeginSingleUse(
            GtVkContext* context,
            VkCommandPool pool,
            GtVkCmdBuffer* outCommandBuffer
        );
        GTAPI void gontiVkCommandBufferEndSingleUse(
            GtVkContext* context,
            VkCommandPool pool,
            GtVkCmdBuffer* commandBuffer,
            VkQueue queue
        );
        GTAPI void gontiVkCommandBufferEnd(GtVkCmdBuffer* commandBuffer);
        GTAPI void gontiVkCommandBufferSubmitted(GtVkCmdBuffer* commandBuffer);
        GTAPI void gontiVkCommandBufferReset(GtVkCmdBuffer* commandBuffer);
        GTAPI void gontiVkCommandBufferUpdateSubmitted(GtVkCmdBuffer* commandBuffer);
        GTAPI void gontiVkCommandBuffersCreate(GtVkContext* context);

#ifdef __cplusplus
    }
#endif

#endif