#ifndef LOGGER_H
#define LOGGER_H

#ifdef __cplusplus
extern "C" {
#endif

    #include "../../../__GONTI_INCLUDES.h"

    #ifdef Logger_ON

        #define LOG_WARN_ENABLED 1
        #define LOG_INFO_ENABLED 1
        #define LOG_DEBUG_ENABLED 1
        #define LOG_TRACE_ENABLED 1

        #if KRELEASE == 1
            #define LOG_DEBUG_ENABLED 0
            #define LOG_TRACE_ENABLED 0
        #endif

        // Fatal error
        #define KFATAL(message, ...) logger_logOutput(LOG_LEVEL_FATAL, message, ##__VA_ARGS__)

        // Error level
        #ifndef KERROR
            #define KERROR(message, ...) logger_logOutput(LOG_LEVEL_ERROR, message, ##__VA_ARGS__)
        #endif
        
        // Warnings
        #if LOG_WARN_ENABLED == 1
            #define KWARN(message, ...) logger_logOutput(LOG_LEVEL_WARN, message, ##__VA_ARGS__)
        #else
            #define KWARN(message, ...)
        #endif

        // Informations
        #if LOG_INFO_ENABLED == 1
            #define KINFO(message, ...) logger_logOutput(LOG_LEVEL_INFO, message, ##__VA_ARGS__)
        #else
            #define KINFO(message, ...)
        #endif

        // Debug
        #if LOG_DEBUG_ENABLED == 1
            #define KDEBUG(message, ...) logger_logOutput(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__)
        #else
            #define KDEBUG(message, ...)
        #endif

        // Trace
        #if LOG_TRACE_ENABLED == 1
            #define KTRACE(message, ...) logger_logOutput(LOG_LEVEL_TRACE, message, ##__VA_ARGS__)
        #else
            #define KTRACE(message, ...)
        #endif

        typedef enum logLevel {
            LOG_LEVEL_FATAL = 0,
            LOG_LEVEL_ERROR = 1,
            LOG_LEVEL_WARN = 2,
            LOG_LEVEL_INFO = 3,
            LOG_LEVEL_DEBUG = 4,
            LOG_LEVEL_TRACE = 5
        }logLevel;

        /*B8*/
        KAPI b8 logger_initializeLogging();

        /*VOID*/
        KAPI void logger_shutdownLogging();
        KAPI void logger_logOutput(logLevel level, const char* message, ...);


        typedef struct {
            b8(*initializeLogging)();
            void(*shutdownLogging)();
        }Logger;
        extern KAPI Logger m_logger;

    #endif // !Logger_ON

#ifdef __cplusplus
}
#endif

#endif // !LOGGER_H