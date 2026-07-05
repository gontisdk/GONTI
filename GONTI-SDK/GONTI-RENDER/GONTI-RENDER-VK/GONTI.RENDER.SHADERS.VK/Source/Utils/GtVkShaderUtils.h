#ifndef GtVkShaderUtilsH
#define GtVkShaderUtilsH

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI-CORE/GONTI.CORE/Source/Defines/GtDefines.inl>
        #include <Types/GtVkShaderStage.inl>
        #include <GONTI-RENDER/GONTI-RENDER-VK/GONTI.RENDER.COMMON.VK/Source/Types/GtVkContextTypes.inl>

        GTAPI GtB8 gontiVkShaderModuleCreate(GtVkContext* context, const char* name, const char* typeStr, VkShaderStageFlagBits shaderStageFlag, GtU32 stageIndex, GtVkShaderStage* shaderStages);

#ifdef __cplusplus
    }
#endif

#endif