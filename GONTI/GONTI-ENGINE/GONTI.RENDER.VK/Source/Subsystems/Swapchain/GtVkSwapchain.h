#ifndef GtVkSwapchainH
#define GtVkSwapchainH

#ifdef __cplusplus
    extern "C" {
#endif

        #include <Types/GtVkContextTypes.inl>

        GTAPI GtB8 gontiVkSwapchainAcquireNextImageIndex(GtVkContext* context, GtVkSwapchain* swapchain, GtU64 timeoutNs, VkSemaphore imageAnavibleSemaphore, VkFence fence, GtU32* outImageIndex);

        GTAPI void gontiVkSwapchainCreate(GtVkContext* context, GtU32 width, GtU32 height, GtVkSwapchain* outSwapchain);
        GTAPI void gontiVkSwapchainRecreate(GtVkContext* context, GtU32 width, GtU32 height, GtVkSwapchain* swapchain);
        GTAPI void gontiVkSwapchainDestroy(GtVkContext* context, GtVkSwapchain* swapchain);
        GTAPI void gontiVkSwapchainDarraysDestroy(GtVkSwapchain* swapchain);
        GTAPI VkResult gontiVkSwapchainPresent(GtVkContext* context, GtVkSwapchain* swapchain, VkQueue graphicsQueue, VkQueue presentQueue, VkSemaphore renderCompleteSemaphore, GtU32 presentImageIndex);

#ifdef __cplusplus
    }
#endif

#endif