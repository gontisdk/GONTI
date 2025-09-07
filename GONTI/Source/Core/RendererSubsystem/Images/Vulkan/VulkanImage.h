#ifndef VULKANIMAGE_H
#define VULKANIMAGE_H

    #ifdef __cplusplus
        extern "C" {
    #endif

            #include "../../../../__GONTI_INCLUDES.h"
            
            #ifdef USE_VULKAN
            #ifdef Image_ON

                #include "../../Types/Vulkan/VulkanContextTypes.inl"

                KAPI void vulkanImageCreate(
                    gontiVulkanContext* context,
                    VkImageType imageType,
                    u32 width, u32 height,
                    VkFormat format,
                    VkImageTiling tiling,
                    VkImageUsageFlags usage,
                    VkMemoryPropertyFlags memoryFlags,
                    b32 createView,
                    VkImageAspectFlags viewAspectFlags,
                    gontiVulkanImage* outImage
                );

                KAPI void vulkanImageViewCreate(
                    gontiVulkanContext* context,
                    VkFormat format,
                    gontiVulkanImage* image,
                    VkImageAspectFlags aspectFlags
                );

                KAPI void vulkanImageDestroy(
                    gontiVulkanContext* context,
                    gontiVulkanImage* image
                );

            #endif
            #endif

    #ifdef __cplusplus
        }
    #endif

#endif