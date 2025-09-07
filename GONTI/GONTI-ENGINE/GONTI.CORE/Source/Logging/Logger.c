#include "Logger.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "../Platform/Platform.h"
#include "../Memory/Memory.h"

/*B8*/
b8 gontiLoggerInitializeLogging() {
    // TODO: Create log file
    return true;
}

/*VOID*/
void gontiLoggerShutdownLogging() {
    // TODO: Cleanup logging/write queued entries
}
void gontiLoggerLogOutput(GontiLogLevel level, const char* message, ...) {
    const char* levelStrings[6] = {
        "[FATAL]: ",
        "[ERROR]: ",
        "[WARN]: ",
        "[INFO]: ",
        "[DEBUG]: ",
        "[TRACE]: "
    };
    b8 isError = level < LOG_LEVEL_WARN;

    const i32 msgLenght = 32000;
    char outMessage[msgLenght];
    k_setMemory(outMessage, 0, sizeof(outMessage));

    __builtin_va_list argPtr;
    va_start(argPtr, message);
    vsnprintf(outMessage, msgLenght, message, argPtr);
    va_end(argPtr);

    char outMessage2[msgLenght];
    sprintf(outMessage2, "%s%s\n", levelStrings[level], outMessage);

    // Platform specyfics output
    if (isError) {
        gontiPlatformConsoleWriteError(outMessage2, level);
    } else {
        gontiPlatformConsoleWrite(outMessage2, level);
    }
}

/*==[{ m_logger }]==*/
GontiLogger gonti_logger = {
    .initializeLogging = gontiLoggerInitializeLogging,
    .shutdownLogging = gontiLoggerShutdownLogging
};