#include <Shaders/Utils/GtVkShaderUtils.h>

#include <GONTI-ENGINE/GONTI.CORE/Source/CStringTools/GtCStrTools.h>
#include <GONTI-ENGINE/GONTI.CORE/Source/Logging/GtLogger.h>
#include <GONTI-ENGINE/GONTI.CORE/Source/Memory/GtMemory.h>
#include <GONTI-ENGINE/GONTI.CORE/Source/Filesystem/GtFilesystem.h>

GtB8 gontiVkShaderModuleCreate(GtVkContext* context, const char* name, const char* typeStr, VkShaderStageFlagBits shaderStageFlag, GtU32 stageIndex, GtVkShaderStage* shaderStages) {
    // TODO: Move path to args (temporary path)
    char fileName[512];
    gontiStringFormat(fileName, "assets/shaders/%s.%s.spv", name, typeStr);

    gt_zeroMemory(&shaderStages[stageIndex].createInfo, sizeof(VkShaderModuleCreateInfo));
    shaderStages[stageIndex].createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

    GtFileHandle handle;
    if (!gontiFilesystemOpen(fileName, GT_FILE_MODE_READ, GtTrue, &handle)) {
        GTERROR("Unable to read shader module: %s.", fileName);
        return GtFalse;
    }

    GtU64 size = 0;
    GtU8* fileBuffer = 0;
    if (!gontiFilesystemReadAllBytes(&handle, &fileBuffer, &size)) {
        GTERROR("Unable to binary read shader module: %s.", fileName);
        return GtFalse;
    }

    shaderStages[stageIndex].createInfo.codeSize = size;
    shaderStages[stageIndex].createInfo.pCode = (GtU32*)fileBuffer;

    gontiFilesystemClose(&handle);
    GTVK_CHECK(vkCreateShaderModule(
        context->device.logicalDevice,
        &shaderStages[stageIndex].createInfo,
        0,
        &shaderStages[stageIndex].handle
    ));

    gt_zeroMemory(&shaderStages[stageIndex].shaderStageCreateInfo, sizeof(VkPipelineShaderStageCreateInfo));
    shaderStages[stageIndex].shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[stageIndex].shaderStageCreateInfo.stage = shaderStageFlag;
    shaderStages[stageIndex].shaderStageCreateInfo.module = shaderStages[stageIndex].handle;
    shaderStages[stageIndex].shaderStageCreateInfo.pName = "main";

    if (fileBuffer) {
        gt_free(fileBuffer);
        fileBuffer = 0;
    }

    return GtTrue;
}