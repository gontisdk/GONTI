#include <ObjectShader/GtVkObjectShader.h>

#include <Utils/GtVkShaderUtils.h>
#include <GONTI-RENDER/GONTI-RENDER-VK/GONTI.RENDER.CORE.VK/Source/Resources/Pipeline/GtVkPipeline.h>
#include <GONTI-RENDER/GONTI-RENDER-VK/GONTI.RENDER.CORE.VK/Source/Resources/Buffer/GtVkBuffer.h>
#include <GONTI-CORE/GONTI.CORE/Source/Memory/GtMemory.h>
#include <GONTI-CORE/GONTI.CORE/Source/Logging/GtLogger.h>
#include <GONTI-RENDER/GONTI.RENDER.COMMON/Source/Uniforms/GtRUniformsTypes.inl>
#include <GONTI-RENDER/GONTI-RENDER-VK/GONTI.RENDER.UTILITIES.VK/Source/GtVkUtil.h>
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

    VkDescriptorSetLayoutBinding globalUboLayoutBinding;
    globalUboLayoutBinding.binding = 0;
    globalUboLayoutBinding.descriptorCount = 1;
    globalUboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    globalUboLayoutBinding.pImmutableSamplers = 0;
    globalUboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutCreateInfo globalLayoutInfo = {VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
    globalLayoutInfo.bindingCount = 1;
    globalLayoutInfo.pBindings = &globalUboLayoutBinding;
    GTVK_CHECK(vkCreateDescriptorSetLayout(
        context->device.logicalDevice,
        &globalLayoutInfo,
        context->allocator,
        &outShader->globalDescriptorSetLayout
    ));

    VkDescriptorPoolSize globalPoolSize;
    globalPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    globalPoolSize.descriptorCount = context->swapchain.imageCount; // 3

    VkDescriptorPoolCreateInfo globalPoolInfo = {VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
    globalPoolInfo.poolSizeCount = 1;
    globalPoolInfo.pPoolSizes = &globalPoolSize;
    globalPoolInfo.maxSets = context->swapchain.imageCount; // 3
    GTVK_CHECK(vkCreateDescriptorPool(
        context->device.logicalDevice,
        &globalPoolInfo,
        context->allocator,
        &outShader->globalDescriptorPool
    ));

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
    VkVertexInputAttributeDescription attributeDescriptions[attributeCount];
    VkFormat formats[attributeCount];
    formats[0] = VK_FORMAT_R32G32B32_SFLOAT;
    
    GtU32 sizes[attributeCount];
    sizes[0] = sizeof(GtU32);
    for (GtU32 i = 0; i < attributeCount; ++i) {
        attributeDescriptions[i].binding = 0;
        attributeDescriptions[i].location = i;
        attributeDescriptions[i].format = formats[i];
        attributeDescriptions[i].offset = offset;
        offset += sizes[i];
    }

    const GtI32 descriptorSetLayoutCount = 1;
    VkDescriptorSetLayout layouts[descriptorSetLayoutCount];
    layouts[0] = outShader->globalDescriptorSetLayout;

    // NOTE: Should match the number of shader->stages.
    VkPipelineShaderStageCreateInfo stageCreateInfos[GT_VK_OBJECT_SHADER_STAGE_COUNT];
    gt_zeroMemory(stageCreateInfos, sizeof(stageCreateInfos));
    for (GtU32 i = 0; i < GT_VK_OBJECT_SHADER_STAGE_COUNT; ++i) {
        stageCreateInfos[i].sType = outShader->stages[i].shaderStageCreateInfo.sType;
        stageCreateInfos[i] = outShader->stages[i].shaderStageCreateInfo;
    }

    if (!gontiVkPipelineGraphicsCreate(
        context,
        &context->mainRenderpass,
        attributeCount,
        attributeDescriptions,
        descriptorSetLayoutCount,
        layouts,
        GT_VK_OBJECT_SHADER_STAGE_COUNT,
        stageCreateInfos,
        viewport,
        scissor,
        GtFalse,
        &outShader->pipeline
    )) {
        GTERROR("Failed to load graphics pipeline for object shader.");
        return GtFalse;
    }

    if (!gontiVkBufferCreate(
        context, sizeof(GtGlobalUniformObject),
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        GtTrue, &outShader->globalUniformBuffer
    )) {
        GTERROR("Vulkan buffer creation failed for object shader.");
        return GtFalse;
    }

    const GtU32 globalLayoutCount = context->swapchain.imageCount;
    VkDescriptorSetLayout globalLayouts[globalLayoutCount];
    for (GtU32 i = 0; i < globalLayoutCount; i++) {
        globalLayouts[i] = outShader->globalDescriptorSetLayout;
    }

    VkDescriptorSetAllocateInfo allocInfo = {VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
    allocInfo.descriptorPool = outShader->globalDescriptorPool;
    allocInfo.descriptorSetCount = globalLayoutCount;
    allocInfo.pSetLayouts = globalLayouts;
    GTVK_CHECK(vkAllocateDescriptorSets(
        context->device.logicalDevice,
        &allocInfo,
        outShader->globalDescriptorSets
    ));

    return GtTrue;
}

/* 
* VOID 
*/
void gontiVkObjectShaderDestroy(GtVkContext* context, GtVkObjectShader* shader) {
    gontiVkBufferDestroy(context, &shader->globalUniformBuffer);
    gontiVkPipelineDestroy(context, &shader->pipeline);
    vkDestroyDescriptorPool(context->device.logicalDevice, shader->globalDescriptorPool, context->allocator);
    vkDestroyDescriptorSetLayout(context->device.logicalDevice, shader->globalDescriptorSetLayout, context->allocator);

    for (GtU32 i = 0; i < GT_VK_OBJECT_SHADER_STAGE_COUNT; ++i) {
        vkDestroyShaderModule(context->device.logicalDevice, shader->stages[i].handle, context->allocator);
        shader->stages[i].handle = 0;
    }
}
void gontiVkObjectShaderUse(GtVkContext* context, GtVkObjectShader* shader) {
    GtU32 imageIndex = context->imageIndex;
    gontiVkPipelineBind(&context->graphicsCommandBuffers[imageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, &shader->pipeline);
}

void gontiVkObjectShaderUpdateGlobalStage(GtVkContext* context, GtVkObjectShader* shader) {
    GtU32 imageIndex = context->imageIndex;
    VkCommandBuffer cmdBuffer = context->graphicsCommandBuffers[imageIndex].handle;
    VkDescriptorSet globalDescriptor = shader->globalDescriptorSets[imageIndex];
    GtU32 range = sizeof(GtGlobalUniformObject);
    GtU64 offset = 0;

    gontiVkBufferLoadData(
        context, 
        &shader->globalUniformBuffer, 
        offset, range, 0, 
        &shader->globalUbo
    );

    VkDescriptorBufferInfo bufferInfo;
    bufferInfo.buffer = shader->globalUniformBuffer.handle;
    bufferInfo.offset = offset;
    bufferInfo.range = range;

    VkWriteDescriptorSet descriptorWrite = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
    descriptorWrite.dstSet = shader->globalDescriptorSets[imageIndex];
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;

    vkUpdateDescriptorSets(context->device.logicalDevice, 1, &descriptorWrite, 0, 0);
    vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shader->pipeline.pipelineLayout, 0, 1, &globalDescriptor, 0, 0);
}