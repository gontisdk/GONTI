#ifndef VULKANFENCE_H
#define VULKANFENCE_H

    #ifdef __cplusplus
        extern "C" {
    #endif

            #include "../../../../__GONTI_INCLUDES.h"
            
            #ifdef USE_VULKAN
            #ifdef Fence_ON

                #include "../../Types/Vulkan/VulkanContextTypes.inl"

                KAPI b8 vulkanFenceWait(
                    gontiVulkanContext* context,
                    gontiVulkanFence* fence,
                    u64 timeoutNs
                );

                KAPI void vulkanFenceCreate(
                    gontiVulkanContext* context,
                    b8 createSignaled,
                    gontiVulkanFence* outFence
                );
                KAPI void vulkanFenceDestroy(gontiVulkanContext* context, gontiVulkanFence* fence);
                KAPI void vulkanFenceReset(gontiVulkanContext* context, gontiVulkanFence* fence);

            #endif
            #endif

    #ifdef __cplusplus
        }
    #endif

#endif