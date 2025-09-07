#include "StringLib.h"

#ifdef StringLib_ON

#include <string.h>

#include "../../Core/MemorySubsystem/MemorySubsystem.h"

char* stringDuplicate(const char* str) {
    u64 length = stringLength(str);
    char* copy = k_allocate(length + 1, MEMORY_TAG_STRING);
    k_copyMemory(copy, str, length + 1);

    return copy;
}

u64 stringLength(const char* str) {
    return strlen(str);
}

b8 stringsEqual(const char* str0, const char* str1) {
    return strcmp(str0, str1) == 0;
}

#endif