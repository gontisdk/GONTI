#ifndef GtAssertsH
#define GtAssertsH

#ifdef __cplusplus
extern "C" {
#endif

    #include <Defines/GtDefines.inl>

    #define GTASSERTIONS_ENABLED
    #ifdef GTASSERTIONS_ENABLED
        #if _MSC_VER
            #include <intrin.h>
            #define debugBreak() __debugbreak()
        #else
            #define debugBreak() __builtin_trap()
        #endif

        GTAPI void gontiReportAssertionFailure(const char* expression, const char* message, const char* file, GtI32 line);

        #define GTASSERT(expr) {                                         \
            if (expr) {                                                 \
            } else {                                                    \
                gontiReportAssertionFailure(#expr, "", __FILE__, __LINE__);  \
                debugBreak();                                           \
            }                                                           \
        }

        #define GTASSERT_MSG(expr, message) {                                 \
            if (expr) {                                                      \
            } else {                                                         \
                gontiReportAssertionFailure(#expr, message, __FILE__, __LINE__);  \
                debugBreak();                                                \
            }                                                                \
        }

        #ifdef _DEBUG
            #define GTASSERT_DEBUG(expr) {                                   \
                if (expr) {                                                 \
                } else {                                                    \
                    gontiReportAssertionFailure(#expr, "", __FILE__, __LINE__);  \
                    debugBreak();                                           \
                }                                                           \
            }
        #else
            #define GTASSERT_DEBUG(expr)
        #endif
    #else
        #define GTASSERT(expr)
        #define GTASSERT_MSG(expr, message)
        #define GTASSERT_DEBUG(expr)
    #endif


#ifdef __cplusplus
}
#endif

#endif