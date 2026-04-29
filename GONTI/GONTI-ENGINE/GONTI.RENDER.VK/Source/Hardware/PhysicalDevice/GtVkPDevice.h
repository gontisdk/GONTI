#ifndef GtVkPDeviceH
#define GtVkPDeviceH

#ifdef __cplusplus
    extern "C" {
#endif
        
        #include <GONTI-ENGINE/GONTI.CORE/Source/Defines/GtDefines.inl>

        #include <Hardware/Types/GtVkPDeviceTypes.inl>
        #include <Types/GtVkContextTypes.inl>

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
            GtVkSwapchainInfo* outSwapchainSupport
        );
        GTAPI GtB8 gontiVkNumeratePhysicalDevices(GtVkContext* context);

        GTAPI void gontiVkPhysicalDeviceRelease(GtVkContext* context);

#ifdef __cplusplus
    }
#endif

#endif