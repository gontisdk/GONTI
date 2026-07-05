#ifndef GtVkContextTypesINL
#define GtVkContextTypesINL

#ifdef __cplusplus 
    extern "C" {
#endif

        #include <vulkan/vulkan.h>
        #include <GONTI-CORE/GONTI.CORE/Source/Defines/GtDefines.inl>

        #include <GONTI-RENDER/GONTI-RENDER-VK/GONTI.RENDER.CORE.VK/Source/Hardware/Types/GtVkDeviceTypes.inl>
        #include <GONTI-RENDER/GONTI-RENDER-VK/GONTI.RENDER.CORE.VK/Source/Hardware/Types/GtVkLDeviceTypes.inl>
        #include <GONTI-RENDER/GONTI-RENDER-VK/GONTI.RENDER.CORE.VK/Source/Hardware/Types/GtVkPDeviceTypes.inl>

        #include <GONTI-RENDER/GONTI-RENDER-VK/GONTI.RENDER.CORE.VK/Source/Subsystems/Types/GtVkCmdBufferTypes.inl>
        #include <GONTI-RENDER/GONTI-RENDER-VK/GONTI.RENDER.CORE.VK/Source/Subsystems/Types/GtVkFenceTypes.inl>
        #include <GONTI-RENDER/GONTI-RENDER-VK/GONTI.RENDER.CORE.VK/Source/Subsystems/Types/GtVkFrameBufferTypes.inl>
        #include <GONTI-RENDER/GONTI-RENDER-VK/GONTI.RENDER.CORE.VK/Source/Subsystems/Types/GtVkRenderpassTypes.inl>
        #include <GONTI-RENDER/GONTI-RENDER-VK/GONTI.RENDER.CORE.VK/Source/Subsystems/Types/GtVkSwapchainTypes.inl>
        
        #include <GONTI-RENDER/GONTI-RENDER-VK/GONTI.RENDER.CORE.VK/Source/Resources/Types/GtVkImgTypes.inl>
        #include <GONTI-RENDER/GONTI-RENDER-VK/GONTI.RENDER.CORE.VK/Source/Resources/Types/GtVkBufferTypes.inl>

        #include <GONTI-RENDER/GONTI-RENDER-VK/GONTI.RENDER.SHADERS.VK/Source/Types/GtVkObjectShaderTypes.inl>

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
