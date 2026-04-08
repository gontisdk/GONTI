#ifndef GtVkLDeviceH
#define GtVkLDeviceH

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/GtDefines.h>
        #include "../Types/GtVkLDeviceTypes.inl"
        #include "../../Types/GtVkContextTypes.inl"

        GTAPI GtB8 gontiVkLogicalDeviceCreate(
            VkPhysicalDevice physicalDevice, 
            VkDeviceCreateInfo* createInfo, 
            VkAllocationCallbacks* allocator, 
            VkDevice* outLogicalDevice
        );
            
        GTAPI void gontiVkLogicalDeviceInitialize(GtVkContext* context);
        GTAPI void gontiVkLogicalDeviceSharedSetup(
            GtVkDevice* gVkDevice,
            GtVkLDeviceQueueSharedSetup* gVkLdQueueSharedSetup
        );
        GTAPI void gontiVkLogicalDeviceSharedFree(GtVkLDeviceQueueSharedSetup* gVkLdQueueSharedSetup);
        GTAPI void gontiVkLogicalDeviceCommandPoolCreate(
            GtVkContext* context,
            GtVkDevice* gVkDevice,
            VkCommandPool* outCommandPool
        );
        GTAPI void gontiVkLogicalDeviceQueueCreateInfos(
            GtVkDevice* gVkDevice,
            GtVkLDeviceQueueSharedSetup* gVkLdQueueSharedSetup,
            GtVkLDeviceQueueSharedInfo* gVkLdQueueSharedInfo
        );
        GTAPI void gontiVkLogicalDeviceQueueFreeInfos(
            GtVkLDeviceQueueSharedSetup* gVkLdQueueSharedSetup,
            GtVkLDeviceQueueSharedInfo* gVkLdQueueSharedInfo
        );
        GTAPI void gontiVkLogicalDeviceConfigDriven(
            GtVkDevice* gVkDevice
        );
        GTAPI void gontiVkLogicalDeviceDeviceCreateInfo(
            GtVkLDeviceQueueSharedSetup* gVkLdQueueSharedSetup,
            GtVkLDeviceQueueSharedInfo* gVkLdQueueSharedInfo
        );
        GTAPI void gontiVkLogicalDeviceQueuesGet(
            GtVkDevice* gVkDevice
        );
        GTAPI void gontiVkLogicalDeviceQueuesFree(
            GtVkDevice* gVkDevice
        );
        GTAPI void gontiVkLogicalDeviceDestroy(VkDevice logicalDevice, VkAllocationCallbacks* allocator);
        GTAPI void gontiVkLogicalDeviceRelease(GtVkContext* context);

#ifdef __cplusplus
    }
#endif

#endif