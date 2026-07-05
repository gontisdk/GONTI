#ifndef GtVkUtilH
#define GtVkUtilH

#ifdef __cplusplus
    extern "C" {
#endif

        #include <vulkan/vulkan.h>
        #include <GONTI-CORE/GONTI.CORE/Source/Defines/GtDefines.inl>
        #include <GONTI-CORE/GONTI.CORE/Source/Asserts/GtAsserts.h>

        #define GTVK_CHECK(expr) { \
            GTASSERT(expr == VK_SUCCESS); \
        }

        GTAPI const char* gontiVkUtilResultToString(VkResult result, GtB8 getExtended);

        GTAPI GtB8 gontiVkUtilResultIsSuccess(VkResult result);

#ifdef __cplusplus
    }
#endif

#endif