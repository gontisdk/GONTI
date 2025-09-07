#ifndef STRING_H
#define STRING_H

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../Defines/Defines.h"

        KAPI char* gontiStringDuplicate(const char* str);
        KAPI u64 gontiStringLength(const char* str);
        KAPI b8 gontiStringEqual(const char* str1, const char* str2);

#ifdef __cplusplus
    }
#endif

#endif