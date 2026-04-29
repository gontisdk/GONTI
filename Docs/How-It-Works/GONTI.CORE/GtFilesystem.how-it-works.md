# GtFilesystem — How It Works

> **Corresponding Usage file:** [GtFilesystem.usage.md`](../../Usage/GONTI.CORE/GtFilesystem.usage.md)

---

## 1. Overview

`GtFilesystem` is a **portable file I/O abstraction layer** built over the standard C `<stdio.h>` API. It provides existence checks, file open/close, text-line read/write, raw binary read/write, and a full-file byte-read helper, all integrated with the engine's memory system and logger.

---

## 2. Architecture Overview

```
GtFilesystem.h
├── GtFileHandle { void* handle, GtB8 isValid }
├── GtFileModes enum { GT_FILE_MODE_READ = 0x1, GT_FILE_MODE_WRITE = 0x2 }
└── GTAPI declarations (8 functions)

GtFilesystem.c
├── #include <stdio.h>      — fopen, fclose, fgets, fputs, fread, fwrite, fseek, ftell, rewind, fflush
├── #include <string.h>     — strcpy
├── #include <sys/stat.h>   — stat()
├── #include GtLogger.h     — GTERROR
├── #include GtMemory.h     — gt_allocate
├── #include GtCStrTools.h  — gontiStringLength
│
├── gontiFilesystemExist()
├── gontiFilesystemOpen()
├── gontiFilesystemClose()
├── gontiFilesystemReadLine()
├── gontiFilesystemWriteLine()
├── gontiFilesystemRead()
├── gontiFilesystemReadAllBytes()
└── gontiFilesystemWrite()
```

---

## 3. Internal Logic

### 3.1 GtFileHandle

```c
typedef struct GtFileHandle {
    void* handle;   // opaque FILE* pointer
    GtB8 isValid;   // GtTrue when file is successfully opened
} GtFileHandle;
```

The `handle` field stores a `FILE*` cast to `void*`, keeping the type opaque across the public API. All functions cast it back to `FILE*` internally. `isValid` is set to `GtFalse` by `gontiFilesystemOpen` on any error and by `gontiFilesystemClose` on close.

### 3.2 Mode Resolution (`gontiFilesystemOpen`)

The `GtFileModes` bitmask is translated to a C mode string:

| READ bit | WRITE bit | Binary flag | Mode string |
|----------|-----------|-------------|------------|
| ✓ | ✓ | `GtTrue` | `"w+b"` |
| ✓ | ✓ | `GtFalse` | `"w+"` |
| ✓ | ✗ | `GtTrue` | `"rb"` |
| ✓ | ✗ | `GtFalse` | `"r"` |
| ✗ | ✓ | `GtTrue` | `"wb"` |
| ✗ | ✓ | `GtFalse` | `"w"` |
| ✗ | ✗ | any | error — logs and returns `GtFalse` |

`fopen` is called with the resolved mode string. On failure, `GTERROR` is logged and `outHandle->isValid` remains `GtFalse`.

### 3.3 Existence Check (`gontiFilesystemExist`)

Uses POSIX `stat()` from `<sys/stat.h>`. Returns `GtTrue` if `stat` returns `0` (success), meaning the path exists and is accessible. This is cross-platform at the source level but requires the POSIX `stat` header, available on both Windows (via the CRT) and Linux.

### 3.4 Line Reading (`gontiFilesystemReadLine`)

1. Calls `fgets(buffer, 32000, file)` into a 32 000-byte stack buffer.
2. On success, measures the result length via `gontiStringLength`.
3. Allocates `length + 1` bytes tagged `GT_MEM_TAG_STRING` via `gt_allocate`.
4. Copies the buffer into the heap allocation via `strcpy`.
5. Writes the heap pointer to `*lineBuf`.

**Caller is responsible for freeing** `*lineBuf`.

### 3.5 Line Writing (`gontiFilesystemWriteLine`)

1. Calls `fputs(text, file)` to write the text.
2. Appends a `'\n'` via `fputc`.
3. Flushes the file buffer with `fflush`.
4. Returns `GtFalse` if either `fputs` or `fputc` returns `EOF`.

### 3.6 Raw Read (`gontiFilesystemRead`)

Calls `fread(outData, 1, dataSize, file)` and compares `*outBytesRead` to `dataSize`. Returns `GtFalse` on a short read (EOF before `dataSize` bytes).

### 3.7 Full-File Read (`gontiFilesystemReadAllBytes`)

1. `fseek(..., SEEK_END)` + `ftell` to determine file size.
2. `rewind` to reset position.
3. Allocates `size` bytes tagged `GT_MEM_TAG_STRING` via `gt_allocate`.
4. `fread` into the buffer; returns `GtFalse` on short read.

**Caller is responsible for freeing** `*outBytes`.

### 3.8 Raw Write (`gontiFilesystemWrite`)

Calls `fwrite(data, 1, dataSize, file)`. Flushes with `fflush` on success. Returns `GtFalse` on short write.

---

## 4. Dependency Graph

```
GtDefines.inl    ←  GtFilesystem.h
GtLogger.h     ←  GtFilesystem.c  (GTERROR)
GtMemory.h     ←  GtFilesystem.c  (gt_allocate)
GtCStrTools.h  ←  GtFilesystem.c  (gontiStringLength)
<stdio.h>      ←  GtFilesystem.c
<sys/stat.h>   ←  GtFilesystem.c
```

---

## 5. Design Rationale

- **`void*` handle** — hides `FILE*` from the public header, avoiding a `<stdio.h>` dependency in consumer code and leaving room to swap the underlying implementation.
- **Stack buffer in `gontiFilesystemReadLine`** — limits a single line to 32 000 bytes, matching the log and string-format buffer sizes used elsewhere in the engine.
- **`fflush` after every write** — ensures data is committed to the OS buffer immediately, which is important for log files and asset serialisation where a crash after a write should not result in lost data.
- **`GT_MEM_TAG_STRING` for file buffers** — file content read into heap memory is categorised as string data; a dedicated `GT_MEM_TAG_FILE` could be introduced in a future tag expansion.