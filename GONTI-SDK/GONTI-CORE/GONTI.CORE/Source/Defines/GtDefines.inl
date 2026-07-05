#ifndef GtDefinesINL
#define GtDefinesINL

#ifdef __cplusplus
extern "C" {
#endif

    // Define NULL if undefined
    #ifndef __cplusplus
        #ifdef NULL
            #undef NULL
        #endif
        #ifndef NULL
            #define NULL ((void*)0)
        #endif
    #endif

    // Define boolen return types for C

    /** @brief GtB8 and GtB32 returns type GtTrue*/
    #define GtTrue 1

    /** @brief GtB8 and GtB32 returns type Gtfalse*/
    #define GtFalse 0

    // Unsigned int types.

    /** @brief Unsigned 8-bit integer */
    typedef unsigned char GtU8;

    /** @brief Unsigned 16-bit integer */
    typedef unsigned short GtU16;

    /** @brief Unsigned 32-bit integer */
    typedef unsigned int GtU32;

    /** @brief Unsigned 64-bit integer */
    typedef unsigned long long GtU64;

    // Signed int types.

    /** @brief Signed 8-bit integer */
    typedef signed char GtI8;

    /** @brief Signed 16-bit integer */
    typedef signed short GtI16;

    /** @brief Signed 32-bit integer */
    typedef signed int GtI32;

    /** @brief Signed 64-bit integer */
    typedef signed long long GtI64;

    // Floating point types

    /** @brief 32-bit floating point number */
    typedef float GtF32;

    /** @brief 64-bit floating point number */
    typedef double GtF64;

    // Boolean types

    /** @brief 32-bit boolean type, used for APIs which require it */
    typedef int GtB32;

    /** @brief 8-bit boolean type */
    typedef char GtB8;

    /** @brief A range, typically of memory */
    typedef struct GtRange {
        /** @brief The offset in bytes. */
        GtU64 offset;
        /** @brief The size in bytes. */
        GtU64 size;
    } GtRange;

    /** @brief A range, typically of memory */
    typedef struct GtRange32 {
        /** @brief The offset in bytes. */
        GtI32 offset;
        /** @brief The size in bytes. */
        GtI32 size;
    } GtRange32;
    // Properly define static assertions.
    #if defined(__clang__) || defined(__GNUC__)
    /** @brief Static assertion */
    #    define GT_STATIC_ASSERT _Static_assert
    #else

    /** @brief Static assertion */
    #    define GT_STATIC_ASSERT static_assert
    #endif

    // Ensure all types are of the correct size.

    /** @brief Assert GtU8 to be 1 byte.*/
    GT_STATIC_ASSERT(sizeof(GtU8) == 1, "Expected GtU8 to be 1 byte.");

    /** @brief Assert GtU16 to be 2 bytes.*/
    GT_STATIC_ASSERT(sizeof(GtU16) == 2, "Expected GtU16 to be 2 bytes.");

    /** @brief Assert GtU32 to be 4 bytes.*/
    GT_STATIC_ASSERT(sizeof(GtU32) == 4, "Expected GtU32 to be 4 bytes.");

    /** @brief Assert GtU64 to be 8 bytes.*/
    GT_STATIC_ASSERT(sizeof(GtU64) == 8, "Expected GtU64 to be 8 bytes.");

    /** @brief Assert GtI8 to be 1 byte.*/
    GT_STATIC_ASSERT(sizeof(GtI8) == 1, "Expected GtI8 to be 1 byte.");

    /** @brief Assert GtI16 to be 2 bytes.*/
    GT_STATIC_ASSERT(sizeof(GtI16) == 2, "Expected GtI16 to be 2 bytes.");

    /** @brief Assert GtI32 to be 4 bytes.*/
    GT_STATIC_ASSERT(sizeof(GtI32) == 4, "Expected GtI32 to be 4 bytes.");

    /** @brief Assert GtI64 to be 8 bytes.*/
    GT_STATIC_ASSERT(sizeof(GtI64) == 8, "Expected GtI64 to be 8 bytes.");

    /** @brief Assert GtF32 to be 4 bytes.*/
    GT_STATIC_ASSERT(sizeof(GtF32) == 4, "Expected GtF32 to be 4 bytes.");

    /** @brief Assert GtF64 to be 8 bytes.*/
    GT_STATIC_ASSERT(sizeof(GtF64) == 8, "Expected GtF64 to be 8 bytes.");

    /**
     * @brief Any id set to this should be considered invalid,
     * and not actually pointing to a real object.
     */
    #define GT_INVALID_ID_U64 18446744073709551615UL
    #define GT_INVALID_ID 4294967295U
    #define GT_INVALID_ID_U32 GT_INVALID_ID
    #define GT_INVALID_ID_U16 65535U
    #define GT_INVALID_ID_U8 255U

    #define GT_U64_MAX 18446744073709551615UL
    #define GT_U32_MAX 4294967295U
    #define GT_U16_MAX 65535U
    #define GT_U8_MAX 255U
    #define GT_U64_MIN 0UL
    #define GT_U32_MIN 0U
    #define GT_U16_MIN 0U
    #define GT_U8_MIN 0U

    #define GT_I8_MAX 127
    #define GT_I16_MAX 32767
    #define GT_I32_MAX 2147483647
    #define GT_I64_MAX 9223372036854775807L
    #define GT_I8_MIN (-GT_I8_MAX - 1)
    #define GT_I16_MIN (-GT_I16_MAX - 1)
    #define GT_I32_MIN (-GT_I32_MAX - 1)
    #define GT_I64_MIN (-GT_I64_MAX - 1)

     // Platform detection
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    #    define GTPLATFORM_WINDOWS 1
    #    ifndef _WIN64
    #        error "64-bit is required on Windows!"
    #    endif
    #elif defined(__linux__) || defined(__gnu_linux__)
    // Linux OS
    #    define GTPLATFORM_LINUX 1
    #    if defined(__ANDROID__)
    #        define GTPLATFORM_ANDROID 1
    #    endif
    #elif defined(__unix__)
    // Catch anything not caught by the above.
    #    define GTPLATFORM_UNIX 1
    #elif defined(_POSIX_VERSION)
    // Posix
    #    define GTPLATFORM_POSIX 1
    #elif __APPLE__
    // Apple Platforms
    #    define GTPLATFORM_APPLE 1
    #    include <TargetConditionals.h>
    #    if TARGET_IPHONE_SIMULATOR
    // iOS Simulator
    #        define GTPLATFORM_IOS 1
    #        define GTPLATFORM_IOS_SIMULATOR 1
    #    elif TARGET_OS_IPHONE
    #        define GTPLATFORM_IOS 1
    // iOS device
    #    elif TARGET_OS_MAC
    // HACK: Should probably be in the Vulkan Renderer lib, not here.
    #define VK_USE_PLATFORM_MACOS_MVK
    // Other kinds of Mac OS
    #    else
    #        error "Unknown Apple Platform"
    #    endif
    #else
    #    error "Unknown Platform!"
    #endif

    #ifdef GT_EXPORT
    // Exports
    #    ifdef _MSC_VER
    #        define GTAPI __declspec(dllexport)
    #    else
    #        define GTAPI __attribute__((visibility("default")))
    #    endif
    #else
    // Imports
    #    ifdef _MSC_VER
    /** @brief Import/export qualifier */
    #        define GTAPI __declspec(dllimport)
    #    else
    /** @brief Import/export qualifier */
    #        define GTAPI
    #    endif
    #endif

    #if _DEBUG
    #    define GT_DEBUG 1
    #    define GT_RELEASE 0
    #else
    #    define GT_RELEASE 1
    #    define GT_DEBUG 0
    #endif

    /**
     * @brief Clamps value to a range of min and max (inclusive).
     * @param value The value to be clamped.
     * @param min The minimum value of the range.
     * @param max The maximum value of the range.
     * @returns The clamped value.
     */
    #define GT_CLAMP(value, min, max) ((value <= min) ? min : (value >= max) ? max : value)

     // Inlining
    #if defined(__clang__) || defined(__gcc__)
    /** @brief Inline qualifier */
    #    define GTINLINE __attribute__((always_inline)) inline

    /** @brief No-inline qualifier */
    #    define GTNOINLINE __attribute__((noinline))
    #elif defined(_MSC_VER)

    /** @brief Inline qualifier */
    #    define GTINLINE __forceinline

    /** @brief No-inline qualifier */
    #    define GTNOINLINE __declspec(noinline)
    #else

    /** @brief Inline qualifier */
    #    define GTINLINE static inline

    /** @brief No-inline qualifier */
    #    define GTNOINLINE
    #endif

    // Deprecation
    #if defined(__clang__) || defined(__gcc__)
    /** @brief Mark something (i.e. a function) as deprecated. */
    #    define GTDEPRECATED(message) __attribute__((deprecated(message)))
    #elif defined(_MSC_VER)
    /** @brief Mark something (i.e. a function) as deprecated. */
    #    define GTDEPRECATED(message) __declspec(deprecated(message))
    #else
    #    error "Unsupported compiler - don't know how to define deprecations!"
    #endif

    /** @brief Gets the number of bytes from amount of gibibytes (GiB) (1024*1024*1024) */
    #define GT_GIBIBYTES(amount) ((amount) * 1024ULL * 1024ULL * 1024ULL)
    /** @brief Gets the number of bytes from amount of mebibytes (MiB) (1024*1024) */
    #define GT_MEBIBYTES(amount) ((amount) * 1024ULL * 1024ULL)
    /** @brief Gets the number of bytes from amount of kibibytes (KiB) (1024) */
    #define GT_KIBIBYTES(amount) ((amount) * 1024ULL)

    /** @brief Gets the number of bytes from amount of gigabytes (GB) (1000*1000*1000) */
    #define GT_GIGABYTES(amount) ((amount) * 1000ULL * 1000ULL * 1000ULL)
    /** @brief Gets the number of bytes from amount of megabytes (MB) (1000*1000) */
    #define GT_MEGABYTES(amount) ((amount) * 1000ULL * 1000ULL)
    /** @brief Gets the number of bytes from amount of kilobytes (KB) (1000) */
    #define GT_KILOBYTES(amount) ((amount) * 1000ULL)

    GTINLINE GtU64 gontiGetAligned(GtU64 operand, GtU64 granularity) {
        return ((operand + (granularity - 1)) & ~(granularity - 1));
    }

    GTINLINE GtRange gontiGetAlignedRange(GtU64 offset, GtU64 size, GtU64 granularity) {
        return (GtRange) { gontiGetAligned(offset, granularity), gontiGetAligned(size, granularity) };
    }

    /**
     * @brief Indicates if the provided flag is set in the given flags int.
     */
    #define GT_FLAG_GET(flags, flag) ((flags & flag) == flag)

    /**
      * @brief Sets a flag within the flags int to enabled/disabled.
      *
      * @param flags The flags int to write to.
      * @param flag The flag to set.
      * @param enabled Indicates if the flag is enabled or not.
      */
    #define GT_FLAG_SET(flags, flag, enabled) (flags = (enabled ? (flags | flag) : (flags & ~flag)))


#ifdef __cplusplus
}
#endif

#endif
