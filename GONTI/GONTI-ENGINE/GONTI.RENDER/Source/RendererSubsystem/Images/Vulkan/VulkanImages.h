#ifndef VULKANIMAGE_H
#define VULKANIMAGE_H

    #ifdef __cplusplus
        extern "C" {
    #endif

            #include "../../../Config/SetupGraphicBackend/UserSetGraphicBackend.h"
            
            #ifdef GONTI_USE_VULKAN

                #include "../../../Types/Vulkan/VulkanContextTypes.inl"

                KAPI void gontiVkImageCreate(
                    GontiVulkanContext* context,
                    VkImageType imageType,
                    u32 width, u32 height,
                    VkFormat format,
                    VkImageTiling tiling,
                    VkImageUsageFlags usage,
                    VkMemoryPropertyFlags memoryFlags,
                    b32 createView,
                    VkImageAspectFlags viewAspectFlags,
                    GontiVulkanImage* outImage
                );

                KAPI void gontiVkImageViewCreate(
                    GontiVulkanContext* context,
                    VkFormat format,
                    GontiVulkanImage* image,
                    VkImageAspectFlags aspectFlags
                );

                KAPI void gontiVkImageDestroy(
                    GontiVulkanContext* context,
                    GontiVulkanImage* image
                );

            #endif

    #ifdef __cplusplus
        }
    #endif

#endif