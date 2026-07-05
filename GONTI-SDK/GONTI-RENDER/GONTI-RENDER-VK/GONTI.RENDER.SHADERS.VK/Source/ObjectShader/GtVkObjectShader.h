#ifndef GtVkObjectShaderH
#define GtVkObjectShaderH

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI-CORE/GONTI.CORE/Source/Defines/GtDefines.inl>
        #include <Types/GtVkObjectShaderTypes.inl>
        #include <GONTI-RENDER/GONTI-RENDER-VK/GONTI.RENDER.COMMON.VK/Source/Types/GtVkContextTypes.inl>

        /* B8 */
        GTAPI GtB8 gontiVkObjectShaderCreate(GtVkContext* context, GtVkObjectShader* outShader);

        /* VOID */
        GTAPI void gontiVkObjectShaderDestroy(GtVkContext* context, GtVkObjectShader* shader);
        GTAPI void gontiVkObjectShaderUse(GtVkContext* context, GtVkObjectShader* shader);
        GTAPI void gontiVkObjectShaderUpdateGlobalStage(GtVkContext* context, GtVkObjectShader* shader);

#ifdef __cplusplus
    }
#endif

#endif