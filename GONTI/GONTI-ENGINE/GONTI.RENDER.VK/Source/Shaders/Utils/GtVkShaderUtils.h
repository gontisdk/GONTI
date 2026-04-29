#ifndef GtVkShaderUtilsH
#define GtVkShaderUtilsH

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI-ENGINE/GONTI.CORE/Source/Defines/GtDefines.inl>
        #include <Types/GtVkContextTypes.inl>
        #include <Shaders/Types/GtVkShaderStage.inl>

        GTAPI GtB8 gontiVkShaderModuleCreate(GtVkContext* context, const char* name, const char* typeStr, VkShaderStageFlagBits shaderStageFlag, GtU32 stageIndex, GtVkShaderStage* shaderStages);

#ifdef __cplusplus
    }
#endif

#endif