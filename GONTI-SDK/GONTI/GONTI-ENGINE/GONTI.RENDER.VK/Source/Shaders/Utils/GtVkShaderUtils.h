#ifndef GtVkShaderUtilsH
#define GtVkShaderUtilsH

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/GtDefines.h>
        #include "../../Types/GtVkContextTypes.inl"
        #include "../Types/GtVkShaderStage.inl"

        GTAPI GtB8 gontiVkShaderModuleCreate(GtVkContext* context, const char* name, const char* typeStr, VkShaderStageFlagBits shaderStageFlag, GtU32 stageIndex, GtVkShaderStage* shaderStages);

#ifdef __cplusplus
    }
#endif

#endif