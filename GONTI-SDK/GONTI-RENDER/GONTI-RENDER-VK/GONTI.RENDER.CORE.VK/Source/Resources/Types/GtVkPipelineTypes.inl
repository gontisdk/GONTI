#ifndef GtVkPipelineTypesINL
#define GtVkPipelineTypesINL

#ifdef __cplusplus
    extern "C" {
#endif

        #include <vulkan/vulkan.h>
        #include <GONTI-CORE/GONTI.CORE/Source/Defines/GtDefines.inl>

        typedef struct GtVkPipeline {
            VkPipeline handle;
            VkPipelineLayout pipelineLayout;
        } GtVkPipeline;

#ifdef __cplusplus
    }
#endif

#endif