#ifndef VULKANCOMMANDBUFFERTYPES_INL
#define VULKANCOMMANDBUFFERTYPES_INL

#ifdef __cplusplus 
    extern "C" {
#endif

        #include "../../../Config/SetupGraphicBackend/UserSetGraphicBackend.h"

        #ifdef GONTI_USE_VULKAN

            #include <vulkan/vulkan.h>

            typedef enum GontiVulkanCommandBufferState {
                COMMAND_BUFFER_STATE_READY,
                COMMAND_BUFFER_STATE_RECORDING,
                COMMAND_BUFFER_STATE_IN_RENDER_PASS,
                COMMAND_BUFFER_STATE_RECORDING_ENDED,
                COMMAND_BUFFER_STATE_SUBMITTED,
                COMMAND_BUFFER_STATE_NOT_ALLOCATED
            }GontiVulkanCommandBufferState;

            typedef struct GontiVulkanCommandBuffer {
                VkCommandBuffer handle;

                GontiVulkanCommandBufferState state;
            }GontiVulkanCommandBuffer;

        #endif

#ifdef __cplusplus 
    }
#endif

#endif