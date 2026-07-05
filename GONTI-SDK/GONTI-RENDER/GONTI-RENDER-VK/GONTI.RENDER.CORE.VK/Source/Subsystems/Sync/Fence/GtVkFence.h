#ifndef GtVkFenceH
#define GtVkFenceH

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI-CORE/GONTI.CORE/Source/Defines/GtDefines.inl>
        #include <Subsystems/Types/GtVkFenceTypes.inl>

        #include <GONTI-RENDER/GONTI-RENDER-VK/GONTI.RENDER.COMMON.VK/Source/Types/GtVkContextTypes.inl>

        GTAPI GtB8 gontiVkFenceWait(
            GtVkContext* context,
            GtVkFence* fence,
            GtU64 timeoutNs
        );

        GTAPI void gontiVkFenceCreate(
            GtVkContext* context,
            GtB8 createSignaled,
            GtVkFence* outFence
        );
        GTAPI void gontiVkFenceDestroy(GtVkContext* context, GtVkFence* fence);
        GTAPI void gontiVkFenceReset(GtVkContext* context, GtVkFence* fence);

#ifdef __cplusplus
    }
#endif

#endif