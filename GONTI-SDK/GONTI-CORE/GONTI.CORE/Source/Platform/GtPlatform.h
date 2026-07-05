#ifndef GtPlatformH
#define GtPlatformH

#ifdef __cplusplus
    extern "C" {
#endif

        #include <Defines/GtDefines.inl>

        #if GTPLATFORM_WINDOWS
            GTAPI GtF64 gontiPlatformGetClockFrequency(void);
            GTAPI void gontiPlatformSetClockFrequency(GtF64 newClockFrequency);
        #endif
        
        GTAPI void gontiPlatformConsoleWrite(const char* message, GtU8 colour);
        GTAPI void gontiPlatformConsoleWriteError(const char* message, GtU8 colour);
        GTAPI void gontiPlatformSleep(GtU64 ms);
        GTAPI void gontiPlatformFree(void* block, GtB8 aligned);

        GTAPI void* gontiPlatformAllocate(GtU64 size, GtB8 aligned);
        GTAPI void* gontiPlatformZeroMemory(void* block, GtU64 size);
        GTAPI void* gontiPlatformCopyMemory(void* dest, const void* source, GtU64 size);
        GTAPI void* gontiPlatformSetMemory(void* dest, GtI32 value, GtU64 size);
        GTAPI void* gontiPlatformReallocate(void* block, GtU64 size);

        GTAPI GtF64 gontiPlatformGetAbsoluteTime(void);

#ifdef __cplusplus
    }
#endif

#endif