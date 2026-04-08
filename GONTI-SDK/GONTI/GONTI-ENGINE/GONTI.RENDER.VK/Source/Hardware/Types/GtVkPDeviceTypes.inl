#ifndef GtVkPDeviceTypesINL
#define GtVkPDeviceTypesINL

#ifdef __cplusplus
    extern "C" {
#endif

        #include <vulkan/vulkan.h>
        #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/GtDefines.h>
        
        typedef struct GtVkPDevicesList {
            VkPhysicalDevice* devices;
            GtU32 count;
        }GtVkPDevicesList;

        typedef struct GtVkPDeviceRequirements {
            GtB8 graphics;
            GtB8 present;
            GtB8 compute;
            GtB8 transfer;

            GtB8 samplerAnisotropy;
            GtB8 discreteGpu;

            const char** deviceExtensionNames;
        }GtVkPDeviceRequirements;

        typedef struct GtVkPDeviceQueueFamilyInfo {
            GtI32 graphicsFamilyIndex;
            GtI32 presentFamilyIndex;
            GtI32 computeFamilyIndex;
            GtI32 transferFamilyIndex;
        }GtVkPDeviceQueueFamilyInfo;

#ifdef __cplusplus
    }
#endif

#endif