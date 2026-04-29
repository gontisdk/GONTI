#ifndef GtVkSyncObjectsH
#define GtVkSyncObjectsH

#ifdef __cplusplus
    extern "C" {
#endif

        #include <Types/GtVkContextTypes.inl>

        GTAPI void gontiVkSyncObjectsCreate(GtVkContext* context);
        GTAPI void gontiVkSyncObjectsDestroy(GtVkContext* context);

#ifdef __cplusplus
    }
#endif

#endif