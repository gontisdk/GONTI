#ifndef GtVkPipelineH
#define GtVkPipelineH

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/GtDefines.h>
        #include "../../Types/GtVkContextTypes.inl"

        GTAPI GtB8 gontiVkPipelineGraphicsCreate(
            GtVkContext* context,
            GtVkRenderpass* renderpass,
            GtU32 attributeCount,
            VkVertexInputAttributeDescription* attributes,
            GtU32 descriptorSetLayoutCount,
            VkDescriptorSetLayout* descriptorSetLayouts,
            GtU32 stageCount,
            VkPipelineShaderStageCreateInfo* stages,
            VkViewport viewport,
            VkRect2D scissor,
            GtB8 isWireframe,
            GtVkPipeline* outPipeline
        );

        GTAPI void gontiVkPipelineDestroy(GtVkContext* context, GtVkPipeline* pipeline);
        GTAPI void gontiVkPipelineBind(GtVkCmdBuffer* command_buffer, VkPipelineBindPoint bind_point, GtVkPipeline* pipeline);

#ifdef __cplusplus
    }
#endif

#endif