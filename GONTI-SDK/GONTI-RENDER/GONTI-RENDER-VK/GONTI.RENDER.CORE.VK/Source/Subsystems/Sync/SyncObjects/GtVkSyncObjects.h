#ifndef GtVkSyncObjectsH
#define GtVkSyncObjectsH

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI-CORE/GONTI.CORE/Source/Defines/GtDefines.inl>
        #include <GONTI-RENDER/GONTI-RENDER-VK/GONTI.RENDER.COMMON.VK/Source/Types/GtVkContextTypes.inl>

        GTAPI void gontiVkSyncObjectsCreate(GtVkContext* context);
        GTAPI void gontiVkSyncObjectsDestroy(GtVkContext* context);

#ifdef __cplusplus
    }
#endif

#endif