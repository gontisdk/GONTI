#ifndef GtVkObjectShaderTypesINL
#define GtVkObjectShaderTypesINL

#ifdef __cplusplus
    extern "C" {
#endif

        #include <Shaders/Types/GtVkShaderStage.inl>
        #include <Resources/Types/GtVkPipelineTypes.inl>

        #define GT_VK_OBJECT_SHADER_STAGE_COUNT 2
        typedef struct GtVkObjectShader {
            GtVkShaderStage stages[GT_VK_OBJECT_SHADER_STAGE_COUNT];
            GtVkPipeline pipeline;
        } GtVkObjectShader;

#ifdef __cplusplus
    }
#endif

#endif