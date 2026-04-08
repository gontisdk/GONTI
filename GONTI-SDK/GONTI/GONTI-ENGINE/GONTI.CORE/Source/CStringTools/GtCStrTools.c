#include "GtCStrTools.h"

#include <string.h>
#include <stdio.h>
#include "../Memory/GtMemory.h"

char* gontiStringDuplicate(const char* str) {
    GtU64 length = gontiStringLength(str);
    char* copy = gt_allocate(length + 1, GT_MEM_TAG_STRING);
    gt_copyMemory(copy, str, length + 1);

    return copy;
}
GtI32 gontiStringFormat(char* desc, const char* format, ...) {
    if (desc) {
        __builtin_va_list argPtr;
        va_start(argPtr, format);

        GtI32 written = gontiStringFormatV(desc, format, argPtr);
        va_end(argPtr);
        return written;
    }

    return -1;
}
GtI32 gontiStringFormatV(char* desc, const char* format, void* vaListp) {
    if (desc) {
        char buffer[32000];
        GtU32 written = vsnprintf(buffer, 32000, format, vaListp);
        buffer[written] = 0;

        gt_copyMemory(desc, buffer, written + 1);
        return written;
    }

    return -1;
}
GtU64 gontiStringLength(const char* str) {
    return strlen(str);
}
GtB8 gontiStringEqual(const char* str1, const char* str2) {
    return strcmp(str1, str2) == 0;
}