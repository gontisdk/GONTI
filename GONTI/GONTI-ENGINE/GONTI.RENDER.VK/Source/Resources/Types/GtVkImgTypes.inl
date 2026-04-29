#ifndef GtVkImgTypesINL
#define GtVkImgTypesINL

#ifdef __cplusplus 
    extern "C" {
#endif

        #include <vulkan/vulkan.h>
        #include <GONTI-ENGINE/GONTI.CORE/Source/Defines/GtDefines.inl>

        typedef struct GtVkImage {
            VkImage handle;
            VkDeviceMemory memory;
            VkImageView view;
            GtU32 width;
            GtU32 height;
        }GtVkImage;

#ifdef __cplusplus 
    }
#endif

#endif