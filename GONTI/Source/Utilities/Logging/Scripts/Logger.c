#include "Logger.h"

#ifdef Logger_ON

    #include <stdio.h>
    #include <string.h>
    #include <stdarg.h>

    #include "../../../Core/Platform/Platform.h"
    #include "../../../Core/MemorySubsystem/MemorySubsystem.h"

    /*B8*/
    b8 logger_initializeLogging() {
        // TODO: Create log file
        return true;
    }

    /*VOID*/
    void logger_shutdownLogging() {
        // TODO: Cleanup logging/write queued entries
    }
    void logger_logOutput(logLevel level, const char* message, ...) {
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
            platformConsoleWriteError(outMessage2, level);
        } else {
            platformConsoleWrite(outMessage2, level);
        }
    }

    /*==[{ m_logger }]==*/
    Logger m_logger = {
        .initializeLogging = logger_initializeLogging,
        .shutdownLogging = logger_shutdownLogging
    };

#endif