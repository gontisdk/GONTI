#include <Platform/GtPlatform.h>

#if GTPLATFORM_WINDOWS

    #include <Windows.h>
    #include <windowsx.h>
    #include <CStringTools/GtCStrTools.h>

    static GtF64 clockFrequency;

    void gontiPlatformConsoleWrite(const char* message, GtU8 colour) {
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
        WORD savedAttributes;
        GetConsoleScreenBufferInfo(consoleHandle, &consoleInfo);
        savedAttributes = consoleInfo.wAttributes;

        // NOTE: FATAL = 64, ERROR = 4, WARN = 6, INFO = 1, DEBUG = 2, TRACE = 8
        static GtU8 levels[6] = {64, 4, 6, 1, 2, 8};
        SetConsoleTextAttribute(consoleHandle, levels[colour]);

        GtU64 length = gontiStringLength(message);
        LPDWORD numerWritten = 0;
        WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), message, (DWORD)length, numerWritten, NULL);

        SetConsoleTextAttribute(consoleHandle, savedAttributes);
    }
    void gontiPlatformConsoleWriteError(const char* message, GtU8 colour) {
        HANDLE consoleHandle = GetStdHandle(STD_ERROR_HANDLE);

        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
        WORD savedAttributes;
        GetConsoleScreenBufferInfo(consoleHandle, &consoleInfo);
        savedAttributes = consoleInfo.wAttributes;

        // NOTE: FATAL = 64, ERROR = 4, WARN = 6, INFO = 1, DEBUG = 2, TRACE = 8
        static GtU8 levels[6] = {64, 4, 6, 1, 2, 8};
        SetConsoleTextAttribute(consoleHandle, levels[colour]);

        GtU64 length = gontiStringLength(message);
        LPDWORD numerWritten = 0;
        WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), message, (DWORD)length, numerWritten, NULL);

        SetConsoleTextAttribute(consoleHandle, savedAttributes);
    }
    void gontiPlatformSleep(GtU64 ms) {
        Sleep(ms);
    }
    void gontiPlatformFree(void* block, GtB8 aligned) {
        free(block);
    }
    void gontiPlatformSetClockFrequency(GtF64 newClockFrequency) {
        clockFrequency = newClockFrequency;
    }

    void* gontiPlatformAllocate(GtU64 size, GtB8 aligned) {
        return malloc(size);
    }
    void* gontiPlatformZeroMemory(void* block, GtU64 size) {
        return memset(block, 0, size);
    }
    void* gontiPlatformCopyMemory(void* dest, const void* source, GtU64 size) {
        return memcpy(dest, source, size);
    }
    void* gontiPlatformSetMemory(void* dest, GtI32 value, GtU64 size) {
        return memset(dest, value, size);
    }
    void* gontiPlatformReallocate(void* block, GtU64 size) {
        return realloc(block, size);
    }

    GtF64 gontiPlatformGetAbsoluteTime(void) {
        LARGE_INTEGER nowTime;
        QueryPerformanceCounter(&nowTime);
        return (GtF64)nowTime.QuadPart * clockFrequency;
    }
    GtF64 gontiPlatformGetClockFrequency(void) {
        return clockFrequency;
    }

#endif