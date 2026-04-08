# GtPlatform — How It Works

> **Corresponding Usage file:** [`GtPlatform.usage.md`](../../Usage/GONTI.CORE/GtPlatform.usage.md)

---

## 1. Overview

`GtPlatform` is the **hardware and OS abstraction layer (PAL)** of `GONTI.CORE`. It exposes a single, unified C API (`GtPlatform.h`) backed by two independent translation units: `GtPlatform_win32.c` (Windows) and `GtPlatform_linux.c` (Linux/XCB). Only one implementation file is compiled per target, selected at the preprocessor level via `GTPLATFORM_WINDOWS` / `GTPLATFORM_LINUX` guards defined in `GtDefines.h`.

---

## 2. Architecture Overview

```
GtPlatform.h          — unified public API
├── GtPlatform_win32.c   (compiled only when GTPLATFORM_WINDOWS == 1)
│   ├── <Windows.h>, <windowsx.h>
│   ├── static GtF64 clockFrequency
│   └── QueryPerformanceCounter / QueryPerformanceFrequency
└── GtPlatform_linux.c   (compiled only when GTPLATFORM_LINUX == 1)
    ├── <xcb/xcb.h>, <X11/...>, <sys/time.h>, <time.h>/<unistd.h>
    └── clock_gettime(CLOCK_MONOTONIC, ...)
```

---

## 3. Internal Logic

### 3.1 Console Output

Both implementations maintain a **6-element colour table** indexed by `GtU8 colour`, which maps directly onto `GtLogLvl` ordinal values:

| Index | Level | Windows Attribute | Linux ANSI Code |
|-------|-------|-------------------|-----------------|
| 0 | FATAL | `64` (red background) | `0;41` |
| 1 | ERROR | `4` (red foreground) | `1;31` |
| 2 | WARN  | `6` (yellow) | `1;33` |
| 3 | INFO  | `1` (blue) | `1;34` |
| 4 | DEBUG | `2` (green) | `1;32` |
| 5 | TRACE | `8` (grey) | `1;30` |

**Windows path** (`gontiPlatformConsoleWrite` / `gontiPlatformConsoleWriteError`):
1. Obtains the console handle (`STD_OUTPUT_HANDLE` or `STD_ERROR_HANDLE`).
2. Reads and saves current `CONSOLE_SCREEN_BUFFER_INFO` attributes.
3. Sets the colour attribute, writes via `WriteConsoleA`, then restores saved attributes.

**Linux path**: wraps the message in ANSI escape sequences (`\033[<code>m ... \033[0m`) and outputs via `printf`. Both `gontiPlatformConsoleWrite` and `gontiPlatformConsoleWriteError` write to `stdout` (identical behaviour in this implementation).

### 3.2 Sleep

**Windows**: delegates directly to `Sleep(ms)`.

**Linux**: branches on `_POSIX_C_SOURCE >= 199309L`:
- If POSIX.1b available: uses `nanosleep` with a `timespec` computed from milliseconds.
- Otherwise: falls back to `sleep` + `usleep` combination.

### 3.3 Memory Operations

Both platforms delegate to the C standard library:

| Function | CRT call |
|----------|----------|
| `gontiPlatformAllocate` | `malloc` |
| `gontiPlatformFree` | `free` (the `aligned` flag is accepted but currently unused) |
| `gontiPlatformZeroMemory` | `memset(..., 0, ...)` |
| `gontiPlatformCopyMemory` | `memcpy` |
| `gontiPlatformSetMemory` | `memset` |
| `gontiPlatformReallocate` | `realloc` |

The `aligned` parameter on `gontiPlatformAllocate` / `gontiPlatformFree` is reserved for future aligned-allocation support (e.g., `_aligned_malloc` / `posix_memalign`).

### 3.4 High-Resolution Timer

**Windows**:
- `clockFrequency` is a file-scope `static GtF64` that must be initialised externally by calling `gontiPlatformSetClockFrequency` with the result of `QueryPerformanceFrequency` converted to seconds-per-tick (`1.0 / freq.QuadPart`).
- `gontiPlatformGetAbsoluteTime` calls `QueryPerformanceCounter` and multiplies `QuadPart` by `clockFrequency`, yielding seconds.

**Linux**:
- No initialisation required.
- `gontiPlatformGetAbsoluteTime` calls `clock_gettime(CLOCK_MONOTONIC, ...)` and returns `tv_sec + tv_nsec * 1e-9`, yielding seconds since an arbitrary epoch.

> **Note:** The Windows timer requires the caller to invoke `gontiPlatformSetClockFrequency` once at startup (before any call to `gontiPlatformGetAbsoluteTime`) or the returned time will be zero or nonsensical.

---

## 4. Dependency Graph

```
GtDefines.h  ←  GtPlatform.h
GtCStrTools.h ←  GtPlatform_win32.c  (gontiStringLength)
GtCStrTools.h ←  GtPlatform_linux.c  (included but not called in current code)
```

---

## 5. Design Rationale

- **Separate translation units per platform** — avoids `#ifdef` spaghetti inside function bodies and allows the linker to include only the relevant object file.
- **Thin wrappers over CRT** — memory functions delegate to `malloc`/`free`/`memset`/`memcpy`, keeping the abstraction overhead minimal while providing a hook point for future custom allocators or alignment strategies.
- **Colour indexing tied to log level ordinals** — the `colour` parameter accepted by console functions is intentionally designed to match `GtLogLvl` values, allowing `GtLogger` to pass the level directly without any conversion.