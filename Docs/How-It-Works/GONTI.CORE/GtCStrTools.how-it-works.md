# GtCStrTools — How It Works

> **Corresponding Usage file:** [`GtCStrTools.usage.md`](../../Usage/GONTI.CORE/GtCStrTools.usage.md)

---

## 1. Overview

`GtCStrTools` is a **thin C-string utility layer** that wraps the standard C library (`<string.h>`, `<stdio.h>`) behind the engine's type system and memory management. It provides string duplication, length measurement, equality comparison, and `printf`-style formatting using both direct and `va_list`-based variadic APIs.

---

## 2. Architecture Overview

```
GtCStrTools.h
└── GTAPI declarations (5 functions)

GtCStrTools.c
├── #include <string.h>   — strlen, strcmp, strcpy
├── #include <stdio.h>    — vsnprintf
├── #include GtMemory.h   — gt_allocate, gt_copyMemory
│
├── gontiStringLength()        → strlen
├── gontiStringEqual()         → strcmp == 0
├── gontiStringDuplicate()     → gt_allocate + gt_copyMemory
├── gontiStringFormat()        → va_start/va_end + gontiStringFormatV
└── gontiStringFormatV()       → vsnprintf into 32 000-byte stack buffer + gt_copyMemory
```

---

## 3. Internal Logic

### 3.1 gontiStringLength

```c
GtU64 gontiStringLength(const char* str) {
    return strlen(str);
}
```

A one-to-one alias over `strlen`. Returns the number of bytes before the null terminator as `GtU64`. Used throughout the engine (including `GtPlatform_win32.c`) to avoid a direct `<string.h>` dependency in platform code.

### 3.2 gontiStringEqual

```c
GtB8 gontiStringEqual(const char* str1, const char* str2) {
    return strcmp(str1, str2) == 0;
}
```

Returns `GtTrue` (`1`) when strings are byte-identical, `GtFalse` (`0`) otherwise. Delegates entirely to `strcmp`.

### 3.3 gontiStringDuplicate

1. Measures length with `gontiStringLength`.
2. Allocates `length + 1` bytes tagged `GT_MEM_TAG_STRING` via `gt_allocate` (zero-initialised).
3. Copies the source string including the null terminator via `gt_copyMemory`.
4. Returns the new heap pointer.

**Caller is responsible for freeing** the returned pointer with `gt_free`.

### 3.4 gontiStringFormat

The variadic entry point:

1. Guards against a `NULL` destination buffer.
2. Opens a `__builtin_va_list` with `va_start`.
3. Delegates to `gontiStringFormatV`, which performs the actual formatting.
4. Closes the list with `va_end`.
5. Returns the byte count written, or `-1` on `NULL` destination.

### 3.5 gontiStringFormatV

The core formatting function:

1. Guards against `NULL` destination.
2. Formats into a **32 000-byte stack buffer** using `vsnprintf`.
3. Manually null-terminates at the written offset (`buffer[written] = 0`).
4. Copies the result into `desc` via `gt_copyMemory(desc, buffer, written + 1)`.
5. Returns the byte count written.

The intermediate stack buffer ensures `desc` is never written beyond `written + 1` bytes, preventing output buffer overflows if `desc` is smaller than the maximum. The caller is responsible for ensuring `desc` is large enough to receive the formatted result.

---

## 4. Dependency Graph

```
GtDefines.inl  ←  GtCStrTools.h
GtMemory.h   ←  GtCStrTools.c  (gt_allocate, gt_copyMemory)
<string.h>   ←  GtCStrTools.c  (strlen, strcmp, strcpy)
<stdio.h>    ←  GtCStrTools.c  (vsnprintf)
<stdarg.h>   ←  GtCStrTools.h  (va_list)
```

---

## 5. Design Rationale

- **Wrapping CRT calls** — centralises the dependency on `<string.h>` and `<stdio.h>`, making it easier to swap implementations for platforms without a standard C library.
- **Stack buffer in `gontiStringFormatV`** — avoids a heap allocation in the formatting path, which is frequently called from `GtLogger` in error handling contexts where allocation might be unsafe.
- **`GT_MEM_TAG_STRING` on duplication** — integrates heap-allocated strings into the memory tracking system, making string leaks visible in `gontiGetMemoryUsageStr` output.
- **Separation of `Format` / `FormatV`** — allows both direct variadic call sites and `va_list`-forwarding call sites (e.g., inside `GtLogger`) to share the same formatting logic.