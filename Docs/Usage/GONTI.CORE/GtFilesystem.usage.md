# GtFilesystem — Usage Guide

> **Internal mechanics:** [`GtFilesystem.how-it-works.md`](../../How-It-Works/GONTI.CORE/GtFilesystem.how-it-works.md)

---

## 1. Purpose

`GtFilesystem` provides a **portable file I/O API** for checking file existence, opening/closing files, reading/writing lines, and performing raw binary transfers.

---

## 2. Installation / Import

```c
#include "Filesystem/GtFilesystem.h"
```

Link against `GONTI.CORE.dll` / `GONTI.CORE.so`. `GtMemory` must be initialised before functions that allocate (`gontiFilesystemReadLine`, `gontiFilesystemReadAllBytes`).

---

## 3. API Reference

### 3.1 Types

```c
typedef struct GtFileHandle {
    void* handle;   // opaque FILE*
    GtB8 isValid;   // GtTrue when open
} GtFileHandle;

typedef enum GtFileModes {
    GT_FILE_MODE_READ  = 0x1,
    GT_FILE_MODE_WRITE = 0x2
} GtFileModes;
```

### 3.2 Functions

| Function | Signature | Returns | Description |
|----------|-----------|---------|-------------|
| `gontiFilesystemExist` | `GtB8 (const char* path)` | `GtTrue` if accessible | Checks if path exists |
| `gontiFilesystemOpen` | `GtB8 (const char* path, GtFileModes mode, GtB8 binary, GtFileHandle* outHandle)` | `GtTrue` on success | Opens a file |
| `gontiFilesystemClose` | `void (GtFileHandle* handle)` | — | Closes and invalidates handle |
| `gontiFilesystemReadLine` | `GtB8 (GtFileHandle* handle, char** lineBuf)` | `GtTrue` if line read | Reads one line; allocates `*lineBuf` (caller frees) |
| `gontiFilesystemWriteLine` | `GtB8 (GtFileHandle* handle, const char* text)` | `GtTrue` on success | Writes text + newline |
| `gontiFilesystemRead` | `GtB8 (GtFileHandle* handle, GtU64 dataSize, void* outData, GtU64* outBytesRead)` | `GtTrue` if full read | Raw binary read into pre-allocated buffer |
| `gontiFilesystemReadAllBytes` | `GtB8 (GtFileHandle* handle, GtU8** outBytes, GtU64* outBytesRead)` | `GtTrue` on success | Reads entire file; allocates `*outBytes` (caller frees) |
| `gontiFilesystemWrite` | `GtB8 (GtFileHandle* handle, GtU64 dataSize, const void* data, GtU64* outBytesWritten)` | `GtTrue` if full write | Raw binary write |

---

## 4. Quick Start Examples

### Check existence and open for reading

```c
#include "Filesystem/GtFilesystem.h"

if (!gontiFilesystemExist("assets/config.ini")) {
    GTERROR("Config file missing!");
    return;
}

GtFileHandle handle;
if (!gontiFilesystemOpen("assets/config.ini", GT_FILE_MODE_READ, GtFalse, &handle)) {
    return;
}
```

### Read a text file line by line

```c
char* line = 0;
while (gontiFilesystemReadLine(&handle, &line)) {
    GTINFO("Line: %s", line);
    gt_free(line);  // free each line after use
    line = 0;
}
gontiFilesystemClose(&handle);
```

### Read an entire binary file

```c
GtFileHandle binHandle;
gontiFilesystemOpen("shaders/vert.spv", GT_FILE_MODE_READ, GtTrue, &binHandle);

GtU8* bytes = 0;
GtU64 bytesRead = 0;
if (gontiFilesystemReadAllBytes(&binHandle, &bytes, &bytesRead)) {
    // process bytes[0..bytesRead-1]
    gt_free(bytes);  // caller frees
}
gontiFilesystemClose(&binHandle);
```

### Write lines to a file

```c
GtFileHandle outHandle;
gontiFilesystemOpen("log/output.txt", GT_FILE_MODE_WRITE, GtFalse, &outHandle);

gontiFilesystemWriteLine(&outHandle, "Session started.");
gontiFilesystemWriteLine(&outHandle, "Player joined.");

gontiFilesystemClose(&outHandle);
```

### Raw binary write

```c
GtU8 data[64] = { /* ... */ };
GtU64 written = 0;
GtFileHandle wHandle;
gontiFilesystemOpen("save/state.bin", GT_FILE_MODE_WRITE, GtTrue, &wHandle);
gontiFilesystemWrite(&wHandle, sizeof(data), data, &written);
gontiFilesystemClose(&wHandle);
```

---

## 5. Troubleshooting

| Problem | Likely Cause | Solution |
|---------|-------------|----------|
| `gontiFilesystemOpen` returns `GtFalse` | File not found or permission denied | Verify path and permissions; use `gontiFilesystemExist` to pre-check |
| `gontiFilesystemReadLine` returns `GtFalse` immediately | EOF reached or file opened in write-only mode | Check mode flags; ensure file has content |
| `gontiFilesystemRead` returns `GtFalse` | File shorter than `dataSize` | Check `*outBytesRead` for partial data; adjust `dataSize` |
| Memory leak for `GT_MEM_TAG_STRING` related to file reads | `*lineBuf` or `*outBytes` not freed | Always `gt_free` pointers returned by `ReadLine` and `ReadAllBytes` |
| `handle.isValid` is `GtFalse` after open | Open call failed | Check error logs; do not use the handle if `isValid == GtFalse` |