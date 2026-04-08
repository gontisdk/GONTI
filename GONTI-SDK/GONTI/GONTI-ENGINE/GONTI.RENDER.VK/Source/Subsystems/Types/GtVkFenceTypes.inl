#ifndef GtVkFenceTypesINL
#define GtVkFenceTypesINL

#ifdef __cplusplus 
    extern "C" {
#endif

        #include <vulkan/vulkan.h>
        #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/GtDefines.h>

        typedef struct GtVkFence {
            VkFence handle;
            GtB8 isSignaled;
        }GtVkFence;

#ifdef __cplusplus 
    }
#endif

#endif