#ifndef VULKANCONTEXTTYPES_INL
#define VULKANCONTEXTTYPES_INL

#ifdef __cplusplus 
    extern "C" {
#endif

        #include "../../../../__GONTI_INCLUDES.h"

        #ifdef USE_VULKAN
        #ifdef RendererSubsystem_ON

            #include <vulkan/vulkan.h>
            #include "../../../../Asserts/Asserts.h"

            #include "VulkanCommandBufferTypes.inl"
            #include "VulkanDeviceTypes.inl"
            #include "VulkanFenceTypes.inl"
            #include "VulkanFramebufferTypes.inl"
            #include "VulkanImageTypes.inl"
            #include "VulkanRenderpassTypes.inl"
            #include "VulkanSwapchainTypes.inl"

            #define VK_CHECK(expr) { \
                KASSERT(expr == VK_SUCCESS); \
            }

            typedef struct gontiVulkanContext {
                VkInstance instance;
                VkAllocationCallbacks* allocator;
                VkSurfaceKHR surface;

                VkSemaphore* imageAvailableSemaphores;
                VkSemaphore* queueCompleteSemaphore;

                gontiVulkanDevice device;
                gontiVulkanSwapchain swapchain;
                gontiVulkanRenderpass mainRenderpass;
                gontiVulkanCommandBuffer* graphicsCommandBuffers;
                gontiVulkanFence* inFlightFences;
                gontiVulkanFence** imagesInFlight;

                u32 framebufferWidth;
                u32 framebufferHeight;
                u32 imageIndex;
                u32 currentFrame;
                u32 inFlightFenceCount;

                u64 framebufferSizeGeneration;
                u64 framebufferSizeLastGeneration;

                i32 (*findMemoryIndex)(u32 typeFilter, u32 propertyFlags);
                
                #if defined(_DEBUG) 
                    VkDebugUtilsMessengerEXT debugMessenger;
                #endif
            }gontiVulkanContext;

        #endif
        #endif

#ifdef __cplusplus 
    }
#endif

#endif