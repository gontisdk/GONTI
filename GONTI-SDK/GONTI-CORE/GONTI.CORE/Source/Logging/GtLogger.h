#ifndef GtLoggerH
#define GtLoggerH

#ifdef __cplusplus
extern "C" {
#endif

    #include <Defines/GtDefines.inl>

    #define GT_LOG_WARN_ENABLED 1
    #define GT_LOG_INFO_ENABLED 1
    #define GT_LOG_DEBUG_ENABLED 1
    #define GT_LOG_TRACE_ENABLED 1

    #if GT_RELEASE == 1
        #define GT_LOG_DEBUG_ENABLED 0
        #define GT_LOG_TRACE_ENABLED 0
    #endif

    // Fatal error
    #define GTFATAL(message, ...) gontiLoggerLogOutput(GT_LOG_LEVEL_FATAL, message, ##__VA_ARGS__)

    // Error level
    #ifndef GTERROR
        #define GTERROR(message, ...) gontiLoggerLogOutput(GT_LOG_LEVEL_ERROR, message, ##__VA_ARGS__)
    #endif
        
    // Warnings
    #if GT_LOG_WARN_ENABLED == 1
        #define GTWARN(message, ...) gontiLoggerLogOutput(GT_LOG_LEVEL_WARN, message, ##__VA_ARGS__)
    #else
        #define GTWARN(message, ...)
    #endif

    // Informations
    #if GT_LOG_INFO_ENABLED == 1
        #define GTINFO(message, ...) gontiLoggerLogOutput(GT_LOG_LEVEL_INFO, message, ##__VA_ARGS__)
    #else
        #define GTINFO(message, ...)
    #endif

    // Debug
    #if GT_LOG_DEBUG_ENABLED == 1
        #define GTDEBUG(message, ...) gontiLoggerLogOutput(GT_LOG_LEVEL_DEBUG, message, ##__VA_ARGS__)
    #else
        #define GTDEBUG(message, ...)
    #endif

    // Trace
    #if GT_LOG_TRACE_ENABLED == 1
        #define GTTRACE(message, ...) gontiLoggerLogOutput(GT_LOG_LEVEL_TRACE, message, ##__VA_ARGS__)
    #else
        #define GTTRACE(message, ...)
    #endif

    typedef enum GtLogLvl {
        GT_LOG_LEVEL_FATAL = 0,
        GT_LOG_LEVEL_ERROR = 1,
        GT_LOG_LEVEL_WARN = 2,
        GT_LOG_LEVEL_INFO = 3,
        GT_LOG_LEVEL_DEBUG = 4,
        GT_LOG_LEVEL_TRACE = 5
    }GtLogLvl;

    /*B8*/
    GTAPI GtB8 gontiLoggerInitializeLogging(GtU64* memoryRequirement, void* state);

    /*VOID*/
    GTAPI void gontiLoggerShutdown(void* state);
    GTAPI void gontiLoggerLogOutput(GtLogLvl level, const char* message, ...);

#ifdef __cplusplus
}
#endif

#endif
