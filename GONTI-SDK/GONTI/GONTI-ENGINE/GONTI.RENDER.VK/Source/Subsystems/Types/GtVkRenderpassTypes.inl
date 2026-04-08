#ifndef GtVkRenderpassTypesINL
#define GtVkRenderpassTypesINL

#ifdef __cplusplus 
    extern "C" {
#endif

        #include <vulkan/vulkan.h>
        #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/GtDefines.h>

        typedef enum GtVkRenderpassState {
            GT_VK_RENDERPASS_READY,
            GT_VK_RENDERPASS_RECORDING,
            GT_VK_RENDERPASS_IN_RENDER_PASS,
            GT_VK_RENDERPASS_RECORDING_ENDEDS,
            GT_VK_RENDERPASS_SUBMITTED,
            GT_VK_RENDERPASS_NOT_ALLOCATED
        }GtVkRenderpassState;

        typedef struct GtVkRenderpass {
            VkRenderPass handle;

            GtF32 x, y, w, h;
            GtF32 r, g, b, a;
            GtF32 depth;

            GtU32 stencil;

            GtVkRenderpassState state;
        }GtVkRenderpass;

#ifdef __cplusplus 
    }
#endif

#endif