#ifndef VULKANCONTEXTTYPES_INL
#define VULKANCONTEXTTYPES_INL

#ifdef __cplusplus 
    extern "C" {
#endif

        #include "../../Config/SetupGraphicBackend/UserSetGraphicBackend.h"

        #if GONTI_USE_VULKAN

            #include <vulkan/vulkan.h>
            #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/Defines.h>
            #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Asserts/Asserts.h>

            #include "../../Hardware/Types/Vulkan/VulkanDeviceTypes.inl"
            #include "../../Hardware/Types/Vulkan/VulkanLogicalDeviceTypes.inl"
            #include "../../Hardware/Types/Vulkan/VulkanPhysicalDeviceTypes.inl"

            #include "../../RendererSubsystem/Types/Vulkan/VulkanCommandBufferTypes.inl"
            #include "../../RendererSubsystem/Types/Vulkan/VulkanFenceTypes.inl"
            #include "../../RendererSubsystem/Types/Vulkan/VulkanFrameBufferTypes.inl"
            #include "../../RendererSubsystem/Types/Vulkan/VulkanImageTypes.inl"
            #include "../../RendererSubsystem/Types/Vulkan/VulkanRenderpassTypes.inl"
            #include "../../RendererSubsystem/Types/Vulkan/VulkanSwapchainTypes.inl"

            #define VK_CHECK(expr) { \
                KASSERT(expr == VK_SUCCESS); \
            }

            typedef struct GontiVulkanContext {
                VkInstance instance;
                VkAllocationCallbacks* allocator;
                VkSurfaceKHR surface;

                VkSemaphore* imageAvailableSemaphores;
                VkSemaphore* queueCompleteSemaphore;

                GontiVulkanDevice device;
                GontiVulkanSwapchain swapchain;
                GontiVulkanRenderpass mainRenderpass;
                GontiVulkanCommandBuffer* graphicsCommandBuffers;
                GontiVulkanFence* inFlightFences;
                GontiVulkanFence** imagesInFlight;

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
            } GontiVulkanContext;

        #endif

#ifdef __cplusplus 
    }
#endif

#endif
