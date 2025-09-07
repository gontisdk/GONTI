#include "String.h"

#include <string.h>
#include "../Memory/Memory.h"

char* gontiStringDuplicate(const char* str) {
    u64 length = gontiStringLength(str);
    char* copy = k_allocate(length + 1, GONTI_MEMORY_TAG_STRING);
    k_copyMemory(copy, str, length + 1);

    return copy;
}
u64 gontiStringLength(const char* str) {
    return strlen(str);
}
b8 gontiStringEqual(const char* str1, const char* str2) {
    return strcmp(str1, str2) == 0;
}