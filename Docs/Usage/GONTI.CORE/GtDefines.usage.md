# GtDefines — Usage Guide

> **Internal mechanics:** [`GtDefines.inlow-it-works.md`](../../How-It-Works/GONTI.CORE/GtDefines.inlow-it-works.md)

---

## 1. Purpose

`GtDefines.inl` provides the **engine-wide type system, platform detection, compiler abstractions, and utility macros**. It must be the first engine header included in every translation unit that uses `GONTI.CORE`.

---

## 2. Installation / Import

```c
#include "Defines/GtDefines.inl"
```

No linking step is required — this is a header-only file.

---

## 3. API Reference

### 3.1 Primitive Types

| Type | Description | Size |
|------|-------------|------|
| `GtU8` | Unsigned 8-bit integer | 1 B |
| `GtU16` | Unsigned 16-bit integer | 2 B |
| `GtU32` | Unsigned 32-bit integer | 4 B |
| `GtU64` | Unsigned 64-bit integer | 8 B |
| `GtI8` | Signed 8-bit integer | 1 B |
| `GtI16` | Signed 16-bit integer | 2 B |
| `GtI32` | Signed 32-bit integer | 4 B |
| `GtI64` | Signed 64-bit integer | 8 B |
| `GtF32` | 32-bit float | 4 B |
| `GtF64` | 64-bit double | 8 B |
| `GtB8` | 8-bit boolean (`char`) | 1 B |
| `GtB32` | 32-bit boolean (`int`) | 4 B |

### 3.2 Boolean Constants

| Constant | Value |
|----------|-------|
| `GtTrue` | `1` |
| `GtFalse` | `0` |

### 3.3 Aggregate Types

```c
typedef struct GtRange {
    GtU64 offset;   // Offset in bytes
    GtU64 size;     // Size in bytes
} GtRange;

typedef struct GtRange32 {
    GtI32 offset;
    GtI32 size;
} GtRange32;
```

### 3.4 Sentinel / Limit Constants

| Macro | Value |
|-------|-------|
| `GT_INVALID_ID_U64` | `18446744073709551615UL` |
| `GT_INVALID_ID` / `GT_INVALID_ID_U32` | `4294967295U` |
| `GT_INVALID_ID_U16` | `65535U` |
| `GT_INVALID_ID_U8` | `255U` |
| `GT_U8_MAX` … `GT_U64_MAX` | Maximum unsigned values |
| `GT_I8_MIN/MAX` … `GT_I64_MIN/MAX` | Signed range bounds |

### 3.5 Platform Detection Macros

| Macro | Defined When |
|-------|-------------|
| `GTPLATFORM_WINDOWS` | Compiling on 64-bit Windows |
| `GTPLATFORM_LINUX` | Compiling on Linux |
| `GTPLATFORM_ANDROID` | Compiling on Android (sub-flag of Linux) |
| `GTPLATFORM_APPLE` | Compiling on any Apple platform |
| `GTPLATFORM_IOS` | Compiling on iOS device or simulator |

### 3.6 Compiler / Linkage Macros

| Macro | Purpose |
|-------|---------|
| `GTAPI` | DLL export (`GT_EXPORT` defined) or import (consumer) |
| `GTINLINE` | Forces function inlining |
| `GTNOINLINE` | Prevents function inlining |
| `GTDEPRECATED(msg)` | Marks a symbol as deprecated with a message |

### 3.7 Build Mode

| Macro | Value |
|-------|-------|
| `GT_DEBUG` | `1` in debug builds, `0` otherwise |
| `GT_RELEASE` | `1` in release builds, `0` otherwise |

### 3.8 Memory Size Macros

| Macro | Equivalent |
|-------|-----------|
| `GT_KIBIBYTES(n)` | `n * 1024` |
| `GT_MEBIBYTES(n)` | `n * 1024 * 1024` |
| `GT_GIBIBYTES(n)` | `n * 1024 * 1024 * 1024` |
| `GT_KILOBYTES(n)` | `n * 1000` |
| `GT_MEGABYTES(n)` | `n * 1000 * 1000` |
| `GT_GIGABYTES(n)` | `n * 1000 * 1000 * 1000` |

### 3.9 Utility Macros & Functions

| Symbol | Signature / Form | Description |
|--------|-----------------|-------------|
| `GT_CLAMP` | `GT_CLAMP(value, min, max)` | Clamps value to `[min, max]` |
| `GT_FLAG_GET` | `GT_FLAG_GET(flags, flag)` | Tests if flag bits are set |
| `GT_FLAG_SET` | `GT_FLAG_SET(flags, flag, enabled)` | Sets or clears flag bits |
| `gontiGetAligned` | `GtU64 (GtU64 operand, GtU64 granularity)` | Rounds up to alignment boundary |
| `gontiGetAlignedRange` | `GtRange (GtU64 offset, GtU64 size, GtU64 granularity)` | Returns aligned `GtRange` |

---

## 4. Quick Start Examples

### Using primitive types

```c
#include "Defines/GtDefines.inl"

GtU32 health = 100;
GtF32 speed  = 9.81f;
GtB8  isAlive = GtTrue;
```

### Platform-conditional code

```c
#include "Defines/GtDefines.inl"

#if GTPLATFORM_WINDOWS
    // Windows-specific path
#elif GTPLATFORM_LINUX
    // Linux-specific path
#endif
```

### Memory sizing

```c
GtU64 bufferSize = GT_MEBIBYTES(64);  // 67 108 864 bytes
```

### Alignment

```c
GtU64 aligned = gontiGetAligned(300, 256);  // → 512
GtRange r     = gontiGetAlignedRange(100, 300, 256);
// r.offset = 256, r.size = 512
```

### Flag manipulation

```c
GtU32 flags = 0;
GT_FLAG_SET(flags, 0x01, GtTrue);   // set bit 0
GtB8 has = GT_FLAG_GET(flags, 0x01); // GtTrue
GT_FLAG_SET(flags, 0x01, GtFalse);  // clear bit 0
```

### Marking deprecated symbols

```c
GTDEPRECATED("Use gontiNewFunction instead")
void gontiOldFunction();
```

---

## 5. Troubleshooting

| Problem | Likely Cause | Solution |
|---------|-------------|----------|
| `"Unknown Platform!"` compile error | Compiling on an unsupported OS | Add a `GTPLATFORM_*` detection branch for your target |
| `"64-bit is required on Windows!"` | Targeting Win32 (x86) | Switch project to `x64` configuration |
| `"Unsupported compiler"` on `GTDEPRECATED` | Exotic/unknown compiler | Add a compiler-specific branch or define `GTDEPRECATED` as empty |
| Type size static assertion fails | Unusual ABI or compiler flags | Verify that no `-m32` or exotic data model flag is active |