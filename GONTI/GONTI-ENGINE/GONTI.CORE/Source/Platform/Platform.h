#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../Defines/Defines.h"

        KAPI void gontiPlatformConsoleWrite(const char* message, u8 colour);
        KAPI void gontiPlatformConsoleWriteError(const char* message, u8 colour);
        KAPI void gontiPlatformSleep(u64 ms);
        KAPI void gontiPlatformFree(void* block, b8 aligned);

        KAPI void* gontiPlatformAllocate(u64 size, b8 aligned);
        KAPI void* gontiPlatformZeroMemory(void* block, u64 size);
        KAPI void* gontiPlatformCopyMemory(void* dest, const void* source, u64 size);
        KAPI void* gontiPlatformSetMemory(void* dest, i32 value, u64 size);
        KAPI void* gontiPlatformReallocate(void* block, u64 size);

#ifdef __cplusplus
    }
#endif

#endif