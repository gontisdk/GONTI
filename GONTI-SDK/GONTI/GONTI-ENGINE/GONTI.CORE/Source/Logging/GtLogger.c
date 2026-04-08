#include "GtLogger.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "../Platform/GtPlatform.h"
#include "../Memory/GtMemory.h"

typedef struct GtLogSysState {
    GtB8 initialized;
} GtLogSysState;
static GtLogSysState* statePtr;

/*B8*/
GtB8 gontiLoggerInitializeLogging(GtU64* memoryRequirement, void* state) {
    *memoryRequirement = sizeof(GtLogSysState);
    if (!state) return GtTrue;

    statePtr = state;
    statePtr->initialized = GtTrue;

    // TODO: Create log file
    return GtTrue;
}

/*VOID*/
void gontiLoggerShutdown(void* state) {
    // TODO: Cleanup logging/write queued entries

    statePtr = 0;
}
void gontiLoggerLogOutput(GtLogLvl level, const char* message, ...) {
    const char* levelStrings[6] = {
        "[FATAL]: ",
        "[ERROR]: ",
        "[WARN]: ",
        "[INFO]: ",
        "[DEBUG]: ",
        "[TRACE]: "
    };
    GtB8 isError = level < GT_LOG_LEVEL_WARN;

    const GtI32 msgLenght = 32000;
    char outMessage[msgLenght];
    gt_setMemory(outMessage, 0, sizeof(outMessage));

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
