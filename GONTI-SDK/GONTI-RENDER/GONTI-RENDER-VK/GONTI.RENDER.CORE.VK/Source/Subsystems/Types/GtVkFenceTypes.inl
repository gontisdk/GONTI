#ifndef GtVkFenceTypesINL
#define GtVkFenceTypesINL

#ifdef __cplusplus 
    extern "C" {
#endif

        #include <vulkan/vulkan.h>
        #include <GONTI-CORE/GONTI.CORE/Source/Defines/GtDefines.inl>

        typedef struct GtVkFence {
            VkFence handle;
            GtB8 isSignaled;
        }GtVkFence;

#ifdef __cplusplus 
    }
#endif

#endif