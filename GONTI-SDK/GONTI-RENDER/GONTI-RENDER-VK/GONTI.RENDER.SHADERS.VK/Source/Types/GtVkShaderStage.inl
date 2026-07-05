#ifndef GtVkShaderStageINL
#define GtVkShaderStageINL

#ifdef __cplusplus
    extern "C" {
#endif

        #include <vulkan/vulkan.h>
        #include <GONTI-CORE/GONTI.CORE/Source/Defines/GtDefines.inl>

        typedef struct GtVkShaderStage {
            VkShaderModuleCreateInfo createInfo;
            VkShaderModule handle;
            VkPipelineShaderStageCreateInfo shaderStageCreateInfo;
        } GtVkShaderStage;

#ifdef __cplusplus
    }
#endif

#endif