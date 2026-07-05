#include <GtFilesystem.h>

#include <GONTI-CORE/GONTI.CORE/Source/Logging/GtLogger.h>
#include <GONTI-CORE/GONTI.CORE/Source/Memory/GtMemory.h>
#include <GONTI-CORE/GONTI.CORE/Source/CStringTools/GtCStrTools.h>

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

/* B8 */
GtB8 gontiFilesystemExist(const char* path) {
    struct stat buffer;
    return stat(path, &buffer) == 0;
}
GtB8 gontiFilesystemOpen(const char* path, GtFileModes mode, GtB8 binary, GtFileHandle* outHandle) {
    outHandle->isValid = GtFalse;
    outHandle->handle = 0;
    const char* modeStr;

    if ((mode & GT_FILE_MODE_READ) != 0 && (mode & GT_FILE_MODE_WRITE) != 0) modeStr = binary ? "w+b" : "w+";
    else if (((mode & GT_FILE_MODE_READ) != 0 && (mode & GT_FILE_MODE_WRITE) == 0)) modeStr = binary ? "rb" : "r";
    else if ((mode & GT_FILE_MODE_READ) == 0 && (mode & GT_FILE_MODE_WRITE) != 0) modeStr = binary ? "wb" : "w";
    else {
        GTERROR("Invalid mode passed while trying to open file: '%s'", path);
        return GtFalse;
    }

    FILE* file = fopen(path, modeStr);
    if (!file) {
        GTERROR("Error opening file: '%s'", path);
        return GtFalse;
    }

    outHandle->handle = file;
    outHandle->isValid = GtTrue;

    return GtTrue;
}
GtB8 gontiFilesystemReadLine(GtFileHandle* handle, char** lineBuf) {
    if (handle->handle) {
        char buffer[32000];
        if (fgets(buffer, 32000, (FILE*)handle->handle) != 0) {
            GtU64 lenght = gontiStringLength(buffer);

            *lineBuf = gt_allocate((sizeof(char) * lenght) + 1, GT_MEM_TAG_STRING);
            strcpy(*lineBuf, buffer);

            return GtTrue;
        }
    }

    return GtFalse;
}
GtB8 gontiFilesystemWriteLine(GtFileHandle* handle, const char* text) {
    if (handle->handle) {
        GtI32 result = fputs(text, (FILE*)handle->handle);
        if (result != EOF) result = fputc('\n', (FILE*)handle->handle);

        fflush((FILE*)handle->handle);
        return result != EOF;
    }

    return GtFalse;
}
GtB8 gontiFilesystemRead(GtFileHandle* handle, GtU64 dataSize, void* outData, GtU64* outBytesRead) {
    if (handle->handle && outData) {
        *outBytesRead = fread(outData, 1, dataSize, (FILE*)handle->handle);
        if (*outBytesRead != dataSize) return GtFalse;

        return GtTrue;
    }

    return GtFalse;
}
GtB8 gontiFilesystemReadAllBytes(GtFileHandle* handle, GtU8** outBytes, GtU64* outBytesRead) {
    if (handle->handle) {
        fseek((FILE*)handle->handle, 0, SEEK_END);
        GtU64 size = ftell((FILE*)handle->handle);
        rewind((FILE*)handle->handle);

        *outBytes = gt_allocate(sizeof(GtU8) * size, GT_MEM_TAG_STRING);
        *outBytesRead = fread(*outBytes, 1, size, (FILE*)handle->handle);
        if (*outBytesRead != size) return GtFalse;

        return GtTrue;
    }

    return GtFalse;
}
GtB8 gontiFilesystemWrite(GtFileHandle* handle, GtU64 dataSize, const void* data, GtU64* outBytesWritten) {
    if (handle->handle) {
        *outBytesWritten = fwrite(data, 1, dataSize, (FILE*)handle->handle);
        if (*outBytesWritten != dataSize) return GtFalse;

        fflush((FILE*)handle->handle);
        return GtTrue;
    }

    return GtFalse;
}

/* VOID */
void gontiFilesystemClose(GtFileHandle* handle) {
    if (handle->handle) {
        fclose((FILE*)handle->handle);
        handle->handle = 0;
        handle->isValid = GtFalse;
    }
}