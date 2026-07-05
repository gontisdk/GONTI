#ifndef GtVkDeviceH
#define GtVkDeviceH

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI-CORE/GONTI.CORE/Source/Defines/GtDefines.inl>
        #include <Hardware/Types/GtVkDeviceTypes.inl>
        
        #include <GONTI-RENDER/GONTI-RENDER-VK/GONTI.RENDER.COMMON.VK/Source/Types/GtVkContextTypes.inl>

        /*B8*/
        GTAPI GtB8 gontiVkDeviceCreate(GtVkContext* context);
        GTAPI GtB8 gontiVkDeviceDetectDepthFormat(GtVkContext* context);

        /*VOID*/
        GTAPI void gontiVkDeviceRelease(GtVkContext* context);
        GTAPI void gontiVkDeviceQuerySwapchainSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, GtVkSwapchainInfo* outSupportInfo);

#ifdef __cplusplus
    }
#endif

#endif