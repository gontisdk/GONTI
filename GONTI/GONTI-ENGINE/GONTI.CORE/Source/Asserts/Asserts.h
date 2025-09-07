#ifndef ASSERTS_H
#define ASSERTS_H

#ifdef __cplusplus
extern "C" {
#endif

    #include "../Defines/Defines.h"

    #define KASSERTIONS_ENABLED
    #ifdef KASSERTIONS_ENABLED
        #if _MSC_VER
            #include <intrin.h>
            #define debugBreak() __debugbreak()
        #else
            #define debugBreak() __bulitin_trap()
        #endif

        KAPI void reportAssertionFailure(const char* expression, const char* message, const char* file, i32 line);

        #define KASSERT(expr) {                                         \
            if (expr) {                                                 \
            } else {                                                    \
                reportAssertionFailure(#expr, "", __FILE__, __LINE__);  \
                debugBreak();                                           \
            }                                                           \
        }

        #define KASSERT_MSG(expr, message) {                                 \
            if (expr) {                                                      \
            } else {                                                         \
                reportAssertionFailure(#expr, message, __FILE__, __LINE__);  \
                debugBreak();                                                \
            }                                                                \
        }

        #ifdef _DEBUG
            #define KASSERT_DEBUG(expr) {                                   \
                if (expr) {                                                 \
                } else {                                                    \
                    reportAssertionFailure(#expr, "", __FILE__, __LINE__);  \
                    debugBreak();                                           \
                }                                                           \
            }
        #else
            #define KASSERT_DEBUG(expr)
        #endif
    #else
        #define KASSERT(expr)
        #define KASSERT_MSG(expr, message)
        #define KASSERT_DEBUG(expr)
    #endif


#ifdef __cplusplus
}
#endif

#endif