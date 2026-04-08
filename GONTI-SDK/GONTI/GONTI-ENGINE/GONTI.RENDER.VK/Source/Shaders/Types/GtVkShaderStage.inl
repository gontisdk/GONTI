#ifndef GtVkShaderStageINL
#define GtVkShaderStageINL

#ifdef __cplusplus
    extern "C" {
#endif

        #include <vulkan/vulkan.h>
        #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/GtDefines.h>

        typedef struct GtVkShaderStage {
            VkShaderModuleCreateInfo createInfo;
            VkShaderModule handle;
            VkPipelineShaderStageCreateInfo shaderStageCreateInfo;
        } GtVkShaderStage;

#ifdef __cplusplus
    }
#endif

#endif