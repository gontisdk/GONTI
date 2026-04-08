#include "GtVkObjectShader.h"

#include "../Utils/GtVkShaderUtils.h"
#include "../../Resources/Pipeline/GtVkPipeline.h"
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Memory/GtMemory.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Logging/GtLogger.h>
#define GT_BUILTIN_SHADER_NAME_OBJECT "Builtin.ObjectShader"

/* 
* B8 
*/
GtB8 gontiVkObjectShaderCreate(GtVkContext* context, GtVkObjectShader* outShader) {
    char stageTypesStr[GT_VK_OBJECT_SHADER_STAGE_COUNT][5] = {"vert", "frag"};
    VkShaderStageFlagBits stageTypes[GT_VK_OBJECT_SHADER_STAGE_COUNT] = {VK_SHADER_STAGE_VERTEX_BIT, VK_SHADER_STAGE_FRAGMENT_BIT};

    for (GtU32 i = 0; i < GT_VK_OBJECT_SHADER_STAGE_COUNT; i++) {
        if (!gontiVkShaderModuleCreate(context, GT_BUILTIN_SHADER_NAME_OBJECT, stageTypesStr[i], stageTypes[i], i, outShader->stages)) {
            GTERROR("Unable to cerate %s shader module for '%s'.", stageTypesStr[i], GT_BUILTIN_SHADER_NAME_OBJECT);
            return GtFalse;
        }
    }

    VkViewport viewport;
    viewport.x = 0.0f;
    viewport.y = (GtF32)context->framebufferHeight;
    viewport.width = (GtF32)context->framebufferWidth;
    viewport.height = -(GtF32)context->framebufferHeight;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor;
    scissor.offset.x = scissor.offset.y = 0;
    scissor.extent.width = context->framebufferWidth;
    scissor.extent.height = context->framebufferHeight;

    GtU32 offset = 0;
    const GtU32 attributeCount = 1;
    VkVertexInputAttributeDescription attribute_descriptions[attributeCount];
    VkFormat formats[attributeCount];
    formats[0] = VK_FORMAT_R32G32B32_SFLOAT;
    
    GtU32 sizes[attributeCount];
    sizes[0] = sizeof(GtU32);
    for (GtU32 i = 0; i < attributeCount; ++i) {
        attribute_descriptions[i].binding = 0;
        attribute_descriptions[i].location = i;
        attribute_descriptions[i].format = formats[i];
        attribute_descriptions[i].offset = offset;
        offset += sizes[i];
    }

    // TODO: Desciptor set layouts.

    // NOTE: Should match the number of shader->stages.
    VkPipelineShaderStageCreateInfo stage_create_infos[GT_VK_OBJECT_SHADER_STAGE_COUNT];
    gt_zeroMemory(stage_create_infos, sizeof(stage_create_infos));
    for (GtU32 i = 0; i < GT_VK_OBJECT_SHADER_STAGE_COUNT; ++i) {
        stage_create_infos[i].sType = outShader->stages[i].shaderStageCreateInfo.sType;
        stage_create_infos[i] = outShader->stages[i].shaderStageCreateInfo;
    }

    if (!gontiVkPipelineGraphicsCreate(
        context,
        &context->mainRenderpass,
        attributeCount,
        attribute_descriptions,
        0,
        0,
        GT_VK_OBJECT_SHADER_STAGE_COUNT,
        stage_create_infos,
        viewport,
        scissor,
        GtFalse,
        &outShader->pipeline
    )) {
        GTERROR("Failed to load graphics pipeline for object shader.");
        return GtFalse;
    }

    return GtTrue;
}

/* 
* VOID 
*/
void gontiVkObjectShaderDestroy(GtVkContext* context, GtVkObjectShader* shader) {
    gontiVkPipelineDestroy(context, &shader->pipeline);

    for (GtU32 i = 0; i < GT_VK_OBJECT_SHADER_STAGE_COUNT; ++i) {
        vkDestroyShaderModule(context->device.logicalDevice, shader->stages[i].handle, context->allocator);
        shader->stages[i].handle = 0;
    }
}
void gontiVkObjectShaderUse(GtVkContext* context, GtVkObjectShader* shader) {
    GtU32 imageIndex = context->imageIndex;
    gontiVkPipelineBind(&context->graphicsCommandBuffers[imageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, &shader->pipeline);
}