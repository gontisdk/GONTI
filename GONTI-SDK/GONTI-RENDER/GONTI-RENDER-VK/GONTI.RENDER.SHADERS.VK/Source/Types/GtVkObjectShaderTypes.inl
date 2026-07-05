#ifndef GtVkObjectShaderTypesINL
#define GtVkObjectShaderTypesINL

#ifdef __cplusplus
    extern "C" {
#endif

        #include <vulkan/vulkan.h>
        #include <Types/GtVkShaderStage.inl>
        #include <GONTI-RENDER/GONTI-RENDER-VK/GONTI.RENDER.CORE.VK/Source/Resources/Types/GtVkPipelineTypes.inl>
        #include <GONTI-RENDER/GONTI-RENDER-VK/GONTI.RENDER.CORE.VK/Source/Resources/Types/GtVkBufferTypes.inl>
        #include <GONTI-RENDER/GONTI.RENDER.COMMON/Source/Uniforms/GtRUniformsTypes.inl>

        #define GT_VK_OBJECT_SHADER_STAGE_COUNT 2
        typedef struct GtVkObjectShader {
            GtVkShaderStage stages[GT_VK_OBJECT_SHADER_STAGE_COUNT];
            VkDescriptorPool globalDescriptorPool;
            VkDescriptorSetLayout globalDescriptorSetLayout;
            VkDescriptorSet globalDescriptorSets[3];
            GtGlobalUniformObject globalUbo;
            GtVkBuffer globalUniformBuffer;
            GtVkPipeline pipeline;
        } GtVkObjectShader;

#ifdef __cplusplus
    }
#endif

#endif