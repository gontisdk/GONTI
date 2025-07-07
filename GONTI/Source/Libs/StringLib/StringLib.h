#ifndef STRINGLIB_H
#define STRINGLIB_H

    #ifdef __cplusplus
        extern "C" {
    #endif

            #include "../../__GONTI_INCLUDES.h"

            #ifdef StringLib_ON

                KAPI char* stringDuplicate(const char* str);
                KAPI u64 stringLength(const char* str);
                KAPI b8 stringsEqual(const char* str0, const char* str1);

            #endif

    #ifdef __cplusplus
        }
    #endif

#endif