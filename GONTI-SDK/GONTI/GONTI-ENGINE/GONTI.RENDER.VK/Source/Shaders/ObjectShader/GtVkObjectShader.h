#ifndef GtVkObjectShaderH
#define GtVkObjectShaderH

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/GtDefines.h>
        #include "../../Types/GtVkContextTypes.inl"
        #include "../Types/GtVkObjectShaderTypes.inl"

        /* B8 */
        GTAPI GtB8 gontiVkObjectShaderCreate(GtVkContext* context, GtVkObjectShader* outShader);

        /* VOID */
        GTAPI void gontiVkObjectShaderDestroy(GtVkContext* context, GtVkObjectShader* shader);
        GTAPI void gontiVkObjectShaderUse(GtVkContext* context, GtVkObjectShader* shader);

#ifdef __cplusplus
    }
#endif

#endif