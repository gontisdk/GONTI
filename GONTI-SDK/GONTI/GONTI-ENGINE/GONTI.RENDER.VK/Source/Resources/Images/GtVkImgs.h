#ifndef GtVkImgsH
#define GtVkImgsH

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../../Types/GtVkContextTypes.inl"

        GTAPI void gontiVkImageCreate(
            GtVkContext* context,
            VkImageType imageType,
            GtU32 width, GtU32 height,
            VkFormat format,
            VkImageTiling tiling,
            VkImageUsageFlags usage,
            VkMemoryPropertyFlags memoryFlags,
            GtB32 createView,
            VkImageAspectFlags viewAspectFlags,
            GtVkImage* outImage
        );

        GTAPI void gontiVkImageViewCreate(
            GtVkContext* context,
            VkFormat format,
            GtVkImage* image,
            VkImageAspectFlags aspectFlags
        );

        GTAPI void gontiVkImageDestroy(
            GtVkContext* context,
            GtVkImage* image
        );

#ifdef __cplusplus
    }
#endif

#endif