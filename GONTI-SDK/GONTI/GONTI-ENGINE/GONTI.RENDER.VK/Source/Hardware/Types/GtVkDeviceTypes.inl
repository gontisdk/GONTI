#ifndef GtVkDeviceTypesINL
#define GtVkDeviceTypesINL

#ifdef __cplusplus
    extern "C" {
#endif
        
        #include "GtVkLDeviceTypes.inl"
        #include "GtVkPDeviceTypes.inl"
        #include "../../Subsystems/Types/GtVkSwapchainTypes.inl"

        typedef struct GtVkDeviceInfo{
            VkPhysicalDeviceProperties properties;
            VkPhysicalDeviceFeatures features;
            VkPhysicalDeviceMemoryProperties memory;
        }GtVkDeviceInfo;

        typedef struct GtVkDevice {
            VkPhysicalDevice physicalDevice;
            VkDevice logicalDevice;

            GtVkSwapchainInfo swapchainSupport;
            GtVkLDevice vkLdDevice;
            GtVkPDevicesList vkDevices;
            GtVkDeviceInfo* vkDeviceInfo;

            GtI32 graphicsQueueIndex;
            GtI32 presentQueueIndex;
            GtI32 transferQueueIndex;
            GtI32 computeQueueIndex;

            VkQueue graphicsQueue;
            VkQueue presentQueue;
            VkQueue transferQueue;
            VkQueue computeQueue;

            VkCommandPool graphicsCommandPool;

            VkPhysicalDeviceProperties properties;
            VkPhysicalDeviceFeatures features;
            VkPhysicalDeviceMemoryProperties memory;
            VkFormat depthFormat;
        }GtVkDevice;

#ifdef __cplusplus
    }
#endif

#endif