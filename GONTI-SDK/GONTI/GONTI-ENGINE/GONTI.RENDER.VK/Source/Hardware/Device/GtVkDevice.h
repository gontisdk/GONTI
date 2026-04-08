#ifndef GtVkDeviceH
#define GtVkDeviceH

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/GtDefines.h>
        #include "../Types/GtVkDeviceTypes.inl"
        #include "../../Types/GtVkContextTypes.inl"

        /*B8*/
        GTAPI GtB8 gontiVkDeviceCreate(struct GtVkContext* context);
        GTAPI GtB8 gontiVkDeviceDetectDepthFormat(struct GtVkContext* context);

        /*VOID*/
        GTAPI void gontiVkDeviceRelease(struct GtVkContext* context);
        GTAPI void gontiVkDeviceQuerySwapchainSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, GtVkSwapchainInfo* outSupportInfo);

#ifdef __cplusplus
    }
#endif

#endif