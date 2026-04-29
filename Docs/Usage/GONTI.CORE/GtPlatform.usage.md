# GtPlatform — Usage Guide

> **Internal mechanics:** [`../../How-It-Works/Platform/GtPlatform.how-it-works.md`](../../How-It-Works/Platform/GtPlatform.how-it-works.md)

---

## 1. Purpose

`GtPlatform` is the **OS abstraction layer** of `GONTI.CORE`. It provides a platform-agnostic API for console I/O, memory operations, thread sleeping, and high-resolution timing. Consumer code should call these functions instead of OS-specific APIs directly.

---

## 2. Installation / Import

```c
#include "Platform/GtPlatform.h"
```

Exactly **one** implementation file must be compiled depending on the target OS:

| Target | File to compile |
|--------|----------------|
| Windows (64-bit) | `GtPlatform_win32.c` |
| Linux | `GtPlatform_linux.c` |

Linux build requires the following packages:

```bash
sudo apt-get install libx11-dev libxkbcommon-x11-dev
```

---

## 3. API Reference

### 3.1 Console Output

| Function | Signature | Description |
|----------|-----------|-------------|
| `gontiPlatformConsoleWrite` | `void (const char* message, GtU8 colour)` | Writes a coloured message to standard output |
| `gontiPlatformConsoleWriteError` | `void (const char* message, GtU8 colour)` | Writes a coloured message to standard error |

The `colour` parameter maps directly to `GtLogLvl` ordinals:

| Value | Level |
|-------|-------|
| `0` | FATAL |
| `1` | ERROR |
| `2` | WARN |
| `3` | INFO |
| `4` | DEBUG |
| `5` | TRACE |

> These functions are intended to be called by `GtLogger`, not by consumer code directly.

### 3.2 Threading

| Function | Signature | Description |
|----------|-----------|-------------|
| `gontiPlatformSleep` | `void (GtU64 ms)` | Suspends the calling thread for `ms` milliseconds |

### 3.3 Memory

| Function | Signature | Description |
|----------|-----------|-------------|
| `gontiPlatformAllocate` | `void* (GtU64 size, GtB8 aligned)` | Allocates `size` bytes (uninitialized) |
| `gontiPlatformFree` | `void (void* block, GtB8 aligned)` | Frees a previously allocated block |
| `gontiPlatformZeroMemory` | `void* (void* block, GtU64 size)` | Zeroes `size` bytes starting at `block`; returns `block` |
| `gontiPlatformCopyMemory` | `void* (void* dest, const void* source, GtU64 size)` | Copies `size` bytes from `source` to `dest`; returns `dest` |
| `gontiPlatformSetMemory` | `void* (void* dest, GtI32 value, GtU64 size)` | Fills `size` bytes at `dest` with `value`; returns `dest` |
| `gontiPlatformReallocate` | `void* (void* block, GtU64 size)` | Resizes allocation to `size` bytes; returns new pointer |

> These functions are wrapped by `GtMemory`. Prefer `gt_allocate` / `gt_free` macros in application code.

### 3.4 High-Resolution Timer

| Function | Signature | Description |
|----------|-----------|-------------|
| `gontiPlatformGetAbsoluteTime` | `GtF64 ()` | Returns monotonic time in **seconds** |
| `gontiPlatformGetClockFrequency` | `GtF64 ()` | *(Windows only)* Returns stored clock frequency |
| `gontiPlatformSetClockFrequency` | `void (GtF64 newClockFrequency)` | *(Windows only)* Sets the QPC multiplier |

---

## 4. Quick Start Examples

### Timing a code section

```c
#include "Platform/GtPlatform.h"

GtF64 start = gontiPlatformGetAbsoluteTime();
// ... work ...
GtF64 elapsed = gontiPlatformGetAbsoluteTime() - start;
```

### Windows timer initialisation (required once at startup)

```c
#if GTPLATFORM_WINDOWS
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    gontiPlatformSetClockFrequency(1.0 / (GtF64)frequency.QuadPart);
#endif
```

### Sleeping

```c
gontiPlatformSleep(16); // sleep ~16 ms (≈60 fps frame cap)
```

### Low-level memory usage

```c
// Prefer GtMemory macros in normal code.
// Direct platform calls are for the memory subsystem only.
void* buf = gontiPlatformAllocate(1024, GtFalse);
gontiPlatformZeroMemory(buf, 1024);
gontiPlatformFree(buf, GtFalse);
```

---

## 5. Troubleshooting

| Problem | Likely Cause | Solution |
|---------|-------------|----------|
| `gontiPlatformGetAbsoluteTime` always returns `0` on Windows | `gontiPlatformSetClockFrequency` not called | Call it at engine startup with the QPC frequency reciprocal |
| Linker error: undefined reference to `gontiPlatformConsoleWrite` | Wrong platform `.c` compiled | Ensure exactly one of `GtPlatform_win32.c` or `GtPlatform_linux.c` is in the build |
| Linux build fails on XCB headers | Missing development packages | Run `sudo apt-get install libx11-dev libxkbcommon-x11-dev` |
| `aligned` parameter has no effect | Feature not yet implemented | Currently `aligned` is reserved; all allocations use `malloc`/`free` |