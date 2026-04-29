#ifndef GtVkContextTypesINL
#define GtVkContextTypesINL

#ifdef __cplusplus 
    extern "C" {
#endif

        #include <vulkan/vulkan.h>
        #include <GONTI-ENGINE/GONTI.CORE/Source/Defines/GtDefines.inl>
        #include <GONTI-ENGINE/GONTI.CORE/Source/Asserts/GtAsserts.h>

        #include <Hardware/Types/GtVkDeviceTypes.inl>
        #include <Hardware/Types/GtVkLDeviceTypes.inl>
        #include <Hardware/Types/GtVkPDeviceTypes.inl>

        #include <Subsystems/Types/GtVkCmdBufferTypes.inl>
        #include <Subsystems/Types/GtVkFenceTypes.inl>
        #include <Subsystems/Types/GtVkFrameBufferTypes.inl>
        #include <Subsystems/Types/GtVkRenderpassTypes.inl>
        #include <Subsystems/Types/GtVkSwapchainTypes.inl>
        
        #include <Resources/Types/GtVkImgTypes.inl>
        #include <Resources/Types/GtVkBufferTypes.inl>

        #include <Shaders/Types/GtVkObjectShaderTypes.inl>

        #define GTVK_CHECK(expr) { \
            GTASSERT(expr == VK_SUCCESS); \
        }

        typedef struct GtVkContext {
            VkInstance instance;
            VkAllocationCallbacks* allocator;
            VkSurfaceKHR surface;

            VkSemaphore* imageAvailableSemaphores;
            VkSemaphore* queueCompleteSemaphore;

            GtVkDevice device;
            GtVkSwapchain swapchain;
            GtVkRenderpass mainRenderpass;
            GtVkCmdBuffer* graphicsCommandBuffers;
            GtVkFence* inFlightFences;
            GtVkFence** imagesInFlight;
            GtVkObjectShader objectShader;
            GtVkBuffer objectVertexBuffer;
            GtVkBuffer objectIndexBuffer;

            GtU32 framebufferWidth;
            GtU32 framebufferHeight;
            GtU32 imageIndex;
            GtU32 currentFrame;
            GtU32 inFlightFenceCount;

            GtU64 framebufferSizeGeneration;
            GtU64 framebufferSizeLastGeneration;
            GtU64 geometryVertexOffset;
            GtU64 geometryIndexOffset;

            GtI32 (*gontiVkFindMemoryIndex)(GtU32 typeFilter, GtU32 propertyFlags);

            #if defined(_DEBUG) 
                VkDebugUtilsMessengerEXT debugMessenger;
            #endif
        } GtVkContext;

#ifdef __cplusplus 
    }
#endif

#endif
