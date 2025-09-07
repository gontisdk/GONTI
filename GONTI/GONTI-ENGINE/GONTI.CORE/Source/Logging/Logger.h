#ifndef LOGGER_H
#define LOGGER_H

#ifdef __cplusplus
extern "C" {
#endif

    #include "../Defines/Defines.h"

    #define LOG_WARN_ENABLED 1
    #define LOG_INFO_ENABLED 1
    #define LOG_DEBUG_ENABLED 1
    #define LOG_TRACE_ENABLED 1

    #if KRELEASE == 1
        #define LOG_DEBUG_ENABLED 0
        #define LOG_TRACE_ENABLED 0
    #endif

    // Fatal error
    #define KFATAL(message, ...) gontiLoggerLogOutput(LOG_LEVEL_FATAL, message, ##__VA_ARGS__)

    // Error level
    #ifndef KERROR
        #define KERROR(message, ...) gontiLoggerLogOutput(LOG_LEVEL_ERROR, message, ##__VA_ARGS__)
    #endif
        
    // Warnings
    #if LOG_WARN_ENABLED == 1
        #define KWARN(message, ...) gontiLoggerLogOutput(LOG_LEVEL_WARN, message, ##__VA_ARGS__)
    #else
        #define KWARN(message, ...)
    #endif

    // Informations
    #if LOG_INFO_ENABLED == 1
        #define KINFO(message, ...) gontiLoggerLogOutput(LOG_LEVEL_INFO, message, ##__VA_ARGS__)
    #else
        #define KINFO(message, ...)
    #endif

    // Debug
    #if LOG_DEBUG_ENABLED == 1
        #define KDEBUG(message, ...) gontiLoggerLogOutput(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__)
    #else
        #define KDEBUG(message, ...)
    #endif

    // Trace
    #if LOG_TRACE_ENABLED == 1
        #define KTRACE(message, ...) gontiLoggerLogOutput(LOG_LEVEL_TRACE, message, ##__VA_ARGS__)
    #else
        #define KTRACE(message, ...)
    #endif

    typedef enum GontiLogLevel {
        LOG_LEVEL_FATAL = 0,
        LOG_LEVEL_ERROR = 1,
        LOG_LEVEL_WARN = 2,
        LOG_LEVEL_INFO = 3,
        LOG_LEVEL_DEBUG = 4,
        LOG_LEVEL_TRACE = 5
    }GontiLogLevel;

    /*B8*/
    KAPI b8 gontiLoggerInitializeLogging();

    /*VOID*/
    KAPI void gontiLoggerShutdownLogging();
    KAPI void gontiLoggerLogOutput(GontiLogLevel level, const char* message, ...);


    typedef struct {
        b8(*initializeLogging)();
        void(*shutdownLogging)();
    }GontiLogger;
    extern KAPI GontiLogger gonti_logger;

#ifdef __cplusplus
}
#endif

#endif // !LOGGER_H