#ifndef GtVkLDeviceTypesINL
#define GtVkLDeviceTypesINL

#ifdef __cplusplus
    extern "C" {
#endif

        #include <vulkan/vulkan.h>
        #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/GtDefines.h>
        
        typedef struct GtVkLDeviceQueueSharedInfo {
            VkDeviceQueueCreateInfo* queueCreateInfos;
            VkDeviceCreateInfo deviceCreateInfo;
            VkPhysicalDeviceFeatures deviceFeatures;
        }GtVkLDeviceQueueSharedInfo;

        typedef struct GtVkLDeviceQueueSharedSetup {
            GtB8 presentSharesGraphicsQueue;
            GtB8 transferSharesGraphicsQueue;

            GtU32 indexCount;
            GtU32* indices;
            GtU32 index;

            GtF32* queuePriority;

                const char* extensionNames;
        }GtVkLDeviceQueueSharedSetup;

        typedef struct GtVkLDevice {
            GtVkLDeviceQueueSharedInfo vkLdQueueSharedInfo;
            GtVkLDeviceQueueSharedSetup vkLdQueueSharedSetup;
        }GtVkLDevice;

#ifdef __cplusplus
    }
#endif

#endif