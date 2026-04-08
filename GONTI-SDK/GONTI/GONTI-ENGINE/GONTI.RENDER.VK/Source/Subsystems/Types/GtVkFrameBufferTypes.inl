#ifndef GtVkFrameBufferTypesINL
#define GtVkFrameBufferTypesINL

#ifdef __cplusplus 
    extern "C" {
#endif

        #include <vulkan/vulkan.h>
        #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/GtDefines.h>
        #include "GtVkRenderpassTypes.inl"

        typedef struct GtVkFramebuffer {
            VkFramebuffer handle;
            GtU32 attachmentCount;
            VkImageView* attachments;
            GtVkRenderpass* renderpass;
        }GtVkFramebuffer;

#ifdef __cplusplus 
    }
#endif

#endif