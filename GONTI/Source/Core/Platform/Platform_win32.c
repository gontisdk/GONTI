#include "Platform.h"

#if KPLATFORM_WINDOWS
#ifdef Platform_ON

    #include <Windows.h>
    #include <Windowsx.h>
    #include <string.h>

    #include "../../Libs/StringLib/StringLib.h"

    void platformConsoleWrite(const char* message, u8 colour) {
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
        WORD saved_attributes;
        GetConsoleScreenBufferInfo(consoleHandle, &consoleInfo);
        saved_attributes = consoleInfo.wAttributes;

        // NOTE: FATAL = 64, ERROR = 4, WARN = 6, INFO = 1, DEBUG = 2, TRACE = 8
        static u8 levels[6] = {64, 4, 6, 1, 2, 8};
        SetConsoleTextAttribute(consoleHandle, levels[colour]);

        u64 lenght = stringLength(message);
        LPDWORD numerWritten = 0;
        WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), message, (DWORD)lenght, numerWritten, NULL);

        SetConsoleTextAttribute(consoleHandle, saved_attributes);
    }

    void platformConsoleWriteError(const char* message, u8 colour) {
        HANDLE consoleHandle = GetStdHandle(STD_ERROR_HANDLE);

        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
        WORD saved_attributes;
        GetConsoleScreenBufferInfo(consoleHandle, &consoleInfo);
        saved_attributes = consoleInfo.wAttributes;

        // NOTE: FATAL = 64, ERROR = 4, WARN = 6, INFO = 1, DEBUG = 2, TRACE = 8
        static u8 levels[6] = {64, 4, 6, 1, 2, 8};
        SetConsoleTextAttribute(consoleHandle, levels[colour]);

        u64 lenght = stringLength(message);
        LPDWORD numerWritten = 0;
        WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), message, (DWORD)lenght, numerWritten, NULL);

        SetConsoleTextAttribute(consoleHandle, saved_attributes);
    }

    void platformSleep(u64 ms) {
        Sleep(ms);
    }

    void platformFree(void* block, b8 aligned) {
        free(block);
    }

    void* platformAllocate(u64 size, b8 aligned) {
        return malloc(size);
    }
    void* platformZeroMemory(void* block, u64 size) {
        return memset(block, 0, size);
    }
    void* platformCopyMemory(void* dest, const void* source, u64 size) {
        return memcpy(dest, source, size);
    }
    void* platformSetMemory(void* dest, i32 value, u64 size) {
        return memset(dest, value, size);
    }
    void* platformReallocate(void* block, u64 size) {
        return realloc(block, size);
    }

#endif
#endif