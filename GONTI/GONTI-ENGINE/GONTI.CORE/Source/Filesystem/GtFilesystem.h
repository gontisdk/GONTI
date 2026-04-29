#ifndef GtFilesystemH
#define GtFilesystemH

#ifdef __cplusplus
    extern "C" {
#endif

        #include <Defines/GtDefines.inl>

        typedef struct GtFileHandle {
            void* handle;
            GtB8 isValid;
        } GtFileHandle;

        typedef enum GtFileModes {
            GT_FILE_MODE_READ = 0x1,
            GT_FILE_MODE_WRITE = 0x2
        } GtFileModes;

        /* B8 */
        GTAPI GtB8 gontiFilesystemExist(const char* path);
        GTAPI GtB8 gontiFilesystemOpen(const char* path, GtFileModes mode, GtB8 binary, GtFileHandle* outHandle);
        GTAPI GtB8 gontiFilesystemReadLine(GtFileHandle* handle, char** lineBuf);
        GTAPI GtB8 gontiFilesystemWriteLine(GtFileHandle* handle, const char* text);
        GTAPI GtB8 gontiFilesystemRead(GtFileHandle* handle, GtU64 dataSize, void* outData, GtU64* outBytesRead);
        GTAPI GtB8 gontiFilesystemReadAllBytes(GtFileHandle* handle, GtU8** outBytes, GtU64* outBytesRead);
        GTAPI GtB8 gontiFilesystemWrite(GtFileHandle* handle, GtU64 dataSize, const void* data, GtU64* outBytesWritten);

        /* VOID */
        GTAPI void gontiFilesystemClose(GtFileHandle* handle);

#ifdef __cplusplus
    }
#endif

#endif