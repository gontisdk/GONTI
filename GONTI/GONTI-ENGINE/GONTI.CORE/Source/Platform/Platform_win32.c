#include "Platform.h"

#if KPLATFORM_WINDOWS

    #include <Windows.h>
    #include <windowsx.h>
    #include "../String/String.h"

    void gontiPlatformConsoleWrite(const char* message, u8 colour) {
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
        WORD savedAtributes;
        GetConsoleScreenBufferInfo(consoleHandle, &consoleInfo);
        savedAtributes = consoleInfo.wAttributes;

        // NOTE: FATAL = 64, ERROR = 4, WARN = 6, INFO = 1, DEBUG = 2, TRACE = 8
        static u8 levels[6] = {64, 4, 6, 1, 2, 8};
        SetConsoleTextAttribute(consoleHandle, levels[colour]);

        u64 length = gontiStringLength(message);
        LPDWORD numerWritten = 0;
        WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), message, (DWORD)length, numerWritten, NULL);

        SetConsoleTextAttribute(consoleHandle, savedAtributes);
    }
    void gontiPlatformConsoleWriteError(const char* message, u8 colour) {
        HANDLE consoleHandle = GetStdHandle(STD_ERROR_HANDLE);

        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
        WORD savedAtributes;
        GetConsoleScreenBufferInfo(consoleHandle, &consoleInfo);
        savedAtributes = consoleInfo.wAttributes;

        // NOTE: FATAL = 64, ERROR = 4, WARN = 6, INFO = 1, DEBUG = 2, TRACE = 8
        static u8 levels[6] = {64, 4, 6, 1, 2, 8};
        SetConsoleTextAttribute(consoleHandle, levels[colour]);

        u64 length = gontiStringLength(message);
        LPDWORD numerWritten = 0;
        WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), message, (DWORD)length, numerWritten, NULL);

        SetConsoleTextAttribute(consoleHandle, savedAtributes);
    }
    void gontiPlatformSleep(u64 ms) {
        Sleep(ms);
    }
    void gontiPlatformFree(void* block, b8 aligned) {
        free(block);
    }

    void* gontiPlatformAllocate(u64 size, b8 aligned) {
        return malloc(size);
    }
    void* gontiPlatformZeroMemory(void* block, u64 size) {
        return memset(block, 0, size);
    }
    void* gontiPlatformCopyMemory(void* dest, const void* source, u64 size) {
        return memcpy(dest, source, size);
    }
    void* gontiPlatformSetMemory(void* dest, i32 value, u64 size) {
        return memset(dest, value, size);
    }
    void* gontiPlatformReallocate(void* block, u64 size) {
        return realloc(block, size);
    }

#endif