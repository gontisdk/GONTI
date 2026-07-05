#ifndef GtVkCmdBufferTypesINL
#define GtVkCmdBufferTypesINL

#ifdef __cplusplus 
    extern "C" {
#endif

        #include <vulkan/vulkan.h>

        typedef enum GtVkCmdBufferState {
            GT_VK_CMD_BUFFER_STATE_READY,
            GT_VK_CMD_BUFFER_STATE_RECORDING,
            GT_VK_CMD_BUFFER_STATE_IN_RENDER_PASS,
            GT_VK_CMD_BUFFER_STATE_RECORDING_ENDED,
            GT_VK_CMD_BUFFER_STATE_SUBMITTED,
            GT_VK_CMD_BUFFER_STATE_NOT_ALLOCATED
        }GtVkCmdBufferState;

        typedef struct GtVkCmdBuffer {
            VkCommandBuffer handle;

            GtVkCmdBufferState state;
        }GtVkCmdBuffer;

#ifdef __cplusplus 
    }
#endif

#endif