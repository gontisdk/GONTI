# GtDefines — How It Works

> **Corresponding Usage file:** [`GtDefines.usage.md`](../../Usage/GONTI.CORE/GtDefines.usage.md)

---

## 1. Overview

`GtDefines.inl` is the foundational header of the entire `GONTI.CORE` library. It is a **header-only** file included by virtually every other module. Its purpose is to establish a stable, portable, and consistent type system, platform detection layer, compiler abstraction, and a set of utility macros used throughout the engine.

This file has **no corresponding `.c` implementation** — all definitions are either preprocessor macros, `typedef` declarations, `static_assert` checks, or `inline` functions resolved entirely at compile time.

---

## 2. Architecture Overview

```
GtDefines.inl
├── NULL redefinition (C-only guard)
├── Boolean constants (GtTrue / GtFalse)
├── Primitive type aliases (GtU8 … GtI64, GtF32, GtF64, GtB8, GtB32)
├── Aggregate types (GtRange, GtRange32)
├── Static assertions (compile-time size checks)
├── Sentinel / limit constants (GT_INVALID_ID_*, GT_U*_MAX, GT_I*_MIN/MAX)
├── Platform detection macros (GTPLATFORM_WINDOWS, GTPLATFORM_LINUX, …)
├── DLL export/import macro (GTAPI)
├── Build mode macros (GT_DEBUG, GT_RELEASE)
├── Utility macros (GT_CLAMP, GT_FLAG_GET, GT_FLAG_SET)
├── Inline compiler abstractions (GTINLINE, GTNOINLINE)
├── Deprecation macro (GTDEPRECATED)
├── Memory size macros (GT_KIBIBYTES, GT_MEBIBYTES, …)
└── Inline utility functions (gontiGetAligned, gontiGetAlignedRange)
```

---

## 3. Internal Logic

### 3.1 NULL Redefinition

Inside a `#ifndef __cplusplus` guard, any pre-existing `NULL` macro is first `#undef`-ed and then redefined as `((void*)0)`. This ensures a semantically correct C definition regardless of what standard headers may have set.

### 3.2 Primitive Type Aliases

All engine types are `typedef`-aliases over standard C integer and floating-point types:

| Engine Type | Underlying Type | Size |
|-------------|-----------------|------|
| `GtU8`      | `unsigned char`  | 1 B |
| `GtU16`     | `unsigned short` | 2 B |
| `GtU32`     | `unsigned int`   | 4 B |
| `GtU64`     | `unsigned long long` | 8 B |
| `GtI8`      | `signed char`    | 1 B |
| `GtI16`     | `signed short`   | 2 B |
| `GtI32`     | `signed int`     | 4 B |
| `GtI64`     | `signed long long` | 8 B |
| `GtF32`     | `float`          | 4 B |
| `GtF64`     | `double`         | 8 B |
| `GtB8`      | `char`           | 1 B |
| `GtB32`     | `int`            | 4 B |

All sizes are verified at compile time using `GT_STATIC_ASSERT`.

### 3.3 Static Assertions

`GT_STATIC_ASSERT` is mapped to `_Static_assert` (Clang/GCC) or `static_assert` (MSVC). Every primitive type alias has a corresponding static assertion that fires a descriptive error message if the size assumption is violated on an exotic target.

### 3.4 Platform Detection

A cascade of `#if` / `#elif` blocks examines compiler-provided predefined macros to set exactly one primary `GTPLATFORM_*` flag:

| Macro | Condition |
|-------|-----------|
| `GTPLATFORM_WINDOWS` | `WIN32` / `_WIN32` / `__WIN32__` — requires `_WIN64` |
| `GTPLATFORM_LINUX`   | `__linux__` / `__gnu_linux__` |
| `GTPLATFORM_ANDROID` | sub-flag when `__ANDROID__` is defined inside Linux branch |
| `GTPLATFORM_UNIX`    | `__unix__` catch-all |
| `GTPLATFORM_POSIX`   | `_POSIX_VERSION` catch-all |
| `GTPLATFORM_APPLE`   | `__APPLE__`, further split into iOS/iOS Simulator/macOS |

If no known platform is detected the file emits `#error "Unknown Platform!"`, making unsupported targets fail loudly at the preprocessing stage.

### 3.5 GTAPI — DLL Visibility Macro

`GTAPI` controls symbol visibility for shared library builds:

- When `GT_EXPORT` is defined (library build): uses `__declspec(dllexport)` (MSVC) or `__attribute__((visibility("default")))` (GCC/Clang).
- Otherwise (consumer build): uses `__declspec(dllimport)` (MSVC) or nothing (GCC/Clang, symbols visible by default).

### 3.6 Build Mode

`GT_DEBUG` and `GT_RELEASE` are integer flags (`0` or `1`) derived from the compiler's `_DEBUG` preprocessor symbol. They are used by other modules (e.g., `GtAsserts`, `GtLogger`) to conditionally compile debug-only code paths.

### 3.7 Inlining and Deprecation Abstractions

| Macro | MSVC | GCC/Clang | Fallback |
|-------|------|-----------|----------|
| `GTINLINE` | `__forceinline` | `__attribute__((always_inline)) inline` | `static inline` |
| `GTNOINLINE` | `__declspec(noinline)` | `__attribute__((noinline))` | _(empty)_ |
| `GTDEPRECATED(msg)` | `__declspec(deprecated(msg))` | `__attribute__((deprecated(msg)))` | compile error |

### 3.8 Memory Size Macros

Two sets of byte-conversion macros are provided:

- **IEC binary prefixes** (`GT_KIBIBYTES`, `GT_MEBIBYTES`, `GT_GIBIBYTES`) — powers of 1024.
- **SI decimal prefixes** (`GT_KILOBYTES`, `GT_MEGABYTES`, `GT_GIGABYTES`) — powers of 1000.

All use `ULL` suffix to guarantee 64-bit arithmetic and prevent overflow.

### 3.9 Alignment Utility Functions

Two `GTINLINE` functions handle aligned-memory arithmetic:

- **`gontiGetAligned(operand, granularity)`** — rounds `operand` up to the next multiple of `granularity` using the standard bitmask trick: `(operand + granularity - 1) & ~(granularity - 1)`. Granularity must be a power of two.
- **`gontiGetAlignedRange(offset, size, granularity)`** — returns a `GtRange` where both `offset` and `size` fields are individually aligned.

### 3.10 Flag Manipulation Macros

- `GT_FLAG_GET(flags, flag)` — tests whether all bits of `flag` are set in `flags`.
- `GT_FLAG_SET(flags, flag, enabled)` — sets or clears bits of `flag` in `flags` depending on the boolean `enabled`.

---

## 4. Dependency Graph

`GtDefines.inl` has **no dependencies** on any other engine header. It is the root node of the entire `GONTI.CORE` include graph.

```
GtDefines.inl   ←   (no engine dependencies)
     ↑
  all other GONTI.CORE modules
```

---

## 5. Design Rationale

- **Single inclusion point** — having one root header eliminates scattered `typedef` re-declarations across modules.
- **Fail-fast philosophy** — `GT_STATIC_ASSERT` and `#error` directives convert silent runtime bugs (wrong type sizes, unsupported platforms) into hard compile-time errors.
- **Zero runtime cost** — every construct in this file is either a macro, a typedef, or an `inline` function; the file generates no object code of its own.