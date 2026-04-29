#ifndef GtVkUtilH
#define GtVkUtilH

#ifdef __cplusplus
    extern "C" {
#endif

        #include <vulkan/vulkan.h>
        #include <GONTI-ENGINE/GONTI.CORE/Source/Defines/GtDefines.inl>

        GTAPI const char* gontiVkUtilResultToString(VkResult result, GtB8 getExtended);

        GTAPI GtB8 gontiVkUtilResultIsSuccess(VkResult result);

#ifdef __cplusplus
    }
#endif

#endif