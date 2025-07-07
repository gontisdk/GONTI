#ifndef VULKANCOMMANDBUFFERTYPES_INL
#define VULKANCOMMANDBUFFERTYPES_INL

#ifdef __cplusplus 
    extern "C" {
#endif

        #include "../../../../__GONTI_INCLUDES.h"

        #ifdef USE_VULKAN
        #ifdef CommandBuffer_ON

            #include <vulkan/vulkan.h>

            typedef enum gontiVulkanCommandBufferState {
                COMMAND_BUFFER_STATE_READY,
                COMMAND_BUFFER_STATE_RECORDING,
                COMMAND_BUFFER_STATE_IN_RENDER_PASS,
                COMMAND_BUFFER_STATE_RECORDING_ENDED,
                COMMAND_BUFFER_STATE_SUBMITTED,
                COMMAND_BUFFER_STATE_NOT_ALLOCATED
            }gontiVulkanCommandBufferState;

            typedef struct gontiVulkanCommandBuffer {
                VkCommandBuffer handle;

                gontiVulkanCommandBufferState state;
            }gontiVulkanCommandBuffer;

        #endif
        #endif

#ifdef __cplusplus 
    }
#endif

#endif