#ifndef GtVkSwapchainTypesINL
#define GtVkSwapchainTypesINL

#ifdef __cplusplus 
    extern "C" {
#endif

        #include <vulkan/vulkan.h>
        #include <GONTI-CORE/GONTI.CORE/Source/Defines/GtDefines.inl>
        #include <Subsystems/Types/GtVkFrameBufferTypes.inl>
        #include <Resources/Types/GtVkImgTypes.inl>

        typedef struct GtVkSwapchainInfo {
            GtU32 formatCount;
            GtU32 presentModeCount;
            VkSurfaceCapabilitiesKHR capabilities;
            VkPresentModeKHR* presentMode;
            VkSurfaceFormatKHR* formats;
        }GtVkSwapchainInfo;

        typedef struct GtVkSwapchain {
            VkImage* images;
            VkImageView* views;
            VkSwapchainKHR handle;
            VkSurfaceFormatKHR imageFormat;

            GtU32 imageCount;
            GtU8 maxFramesInFlight;
            GtB8 recreatingSwapchain;

            GtVkImage depthAttachment;
            GtVkSwapchainInfo swapchainInfo;
            GtVkFramebuffer* framebuffers;
        }GtVkSwapchain;

#ifdef __cplusplus 
    }
#endif

#endif