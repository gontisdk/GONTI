#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

    #include "../../__GONTI_INCLUDES.h"

    #ifdef Platform_ON

    KAPI void platformConsoleWrite(const char* message, u8 colour);
    KAPI void platformConsoleWriteError(const char* message, u8 colour);
    KAPI void platformSleep(u64 ms);
    KAPI void platformFree(void* block, b8 aligned);

    KAPI void* platformAllocate(u64 size, b8 aligned);
    KAPI void* platformZeroMemory(void* block, u64 size);
    KAPI void* platformCopyMemory(void* dest, const void* source, u64 size);
    KAPI void* platformSetMemory(void* dest, i32 value, u64 size);
    KAPI void* platformReallocate(void* block, u64 size);

    #endif

#ifdef __cplusplus
}
#endif

#endif