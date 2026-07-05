#ifndef GtVkPDeviceH
#define GtVkPDeviceH

#ifdef __cplusplus
    extern "C" {
#endif
        
        #include <GONTI-CORE/GONTI.CORE/Source/Defines/GtDefines.inl>
        #include <Hardware/Types/GtVkPDeviceTypes.inl>

        #include <GONTI-RENDER/GONTI-RENDER-VK/GONTI.RENDER.COMMON.VK/Source/Types/GtVkContextTypes.inl>
        struct GtVkSwapchainInfo;
        extern const GtVkPDevicesList ZERO_PHYSICAL_DEVICES;

        GTAPI GtB8 gontiVkSelectPhysicalDevice(GtVkContext* context);
        GTAPI GtB8 gontiVkFindPhysicalDevices(GtVkContext* context);
        GTAPI GtB8 gontiVkPhysicalDeviceMeetsRequirements(
            VkPhysicalDevice device,
            VkSurfaceKHR surface,
            const VkPhysicalDeviceProperties* properties,
            const VkPhysicalDeviceFeatures* features,
            const GtVkPDeviceRequirements* requirements,
            GtVkPDeviceQueueFamilyInfo* outQueueInfo,
            struct GtVkSwapchainInfo* outSwapchainSupport
        );
        GTAPI GtB8 gontiVkNumeratePhysicalDevices(GtVkContext* context);

        GTAPI void gontiVkPhysicalDeviceRelease(GtVkContext* context);

#ifdef __cplusplus
    }
#endif

#endif