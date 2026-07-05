#ifndef GtStringH
#define GtStringH

#ifdef __cplusplus
    extern "C" {
#endif

        #include <Defines/GtDefines.inl>
        #include <stdarg.h>

        GTAPI char* gontiStringDuplicate(const char* str);
        GTAPI GtI32 gontiStringFormat(char* desc, const char* format, ...);
        GTAPI GtI32 gontiStringFormatV(char* desc, const char* format, void* vaListp);
        GTAPI GtU64 gontiStringLength(const char* str);
        GTAPI GtB8 gontiStringEqual(const char* str1, const char* str2);

#ifdef __cplusplus
    }
#endif

#endif