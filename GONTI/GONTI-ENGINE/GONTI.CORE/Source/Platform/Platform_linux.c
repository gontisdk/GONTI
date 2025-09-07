#include "Platform.h"

#if KPLATFORM_LINUX

    #include <xcb/xcb.h>
    #include <X11/keysym.h>
    #include <X11/XKBlib.h> // sudo apt-get install libx11-dev
    #include <X11/Xlib.h>
    #include <X11/Xlib-xcb.h> // sudo apt-get install libxkbcommon-x11-dev
    #include <sys/time.h>

    #if _POSIX_C_SOURCE >= 199309L
        #include <time.h>
    #else
        #include <unistd.h>
    #endif

    #include <stdlib.h>
    #include <stdio.h>
    #include "../String/String.h"

    void gontiPlatformConsoleWrite(const char* message, u8 colour) {
        // FATAL = 0;41, ERROR = 1;31, WARN = 1;33, INFO = 1;34, DEBUG = 1;32, TRACE = 1;30
        const char* colourStrings[] = {"0;41", "1;31", "1;33", "1;34", "1;32", "1;30"};
        printf("\033[%sm%s\033[0m", colourStrings[colour], message);
    }
    void gontiPlatformConsoleWriteError(const char* message, u8 colour) {
        // FATAL = 0;41, ERROR = 1;31, WARN = 1;33, INFO = 1;34, DEBUG = 1;32, TRACE = 1;30
        const char* colourStrings[] = {"0;41", "1;31", "1;33", "1;34", "1;32", "1;30"};
        printf("\033[%sm%s\033[0m", colourStrings[colour], message);
    }
    void gontiPlatformSleep(u64 ms) {
        #if _POSIX_C_SOURCE >= 199309L
            struct timespec ts;
            ts.tv_sec = ms / 1000;
            ts.tv_nsec = (ms % 1000) * 1000 * 1000;
            nanosleep(&ts, NULL);
        #else
            if (ms >= 1000) sleep(ms / 1000);
            usleep((ms % 1000) * 1000);
        #endif
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