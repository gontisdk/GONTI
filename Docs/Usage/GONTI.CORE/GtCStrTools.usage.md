# GtCStrTools — Usage Guide

> **Internal mechanics:** [`GtCStrTools.how-it-works.md`](../../How-It-Works/GONTI.CORE/GtCStrTools.how-it-works.md)

---

## 1. Purpose

`GtCStrTools` provides **engine-standard C-string utilities** — length, equality, duplication, and `printf`-style formatting — integrated with `GONTI.CORE`'s type system and memory tracker.

---

## 2. Installation / Import

```c
#include "CStringTools/GtCStrTools.h"
```

Link against `GONTI.CORE.dll` / `GONTI.CORE.so`. The memory system (`GtMemory`) must be initialised before calling `gontiStringDuplicate`, as it allocates heap memory.

---

## 3. API Reference

| Function | Signature | Returns | Description |
|----------|-----------|---------|-------------|
| `gontiStringLength` | `GtU64 (const char* str)` | Byte count (excl. `\0`) | Wrapper over `strlen` |
| `gontiStringEqual` | `GtB8 (const char* str1, const char* str2)` | `GtTrue` / `GtFalse` | Byte-exact comparison |
| `gontiStringDuplicate` | `char* (const char* str)` | Heap-allocated copy | Caller must `gt_free` the result |
| `gontiStringFormat` | `GtI32 (char* desc, const char* format, ...)` | Bytes written or `-1` | `printf`-style format into `desc` |
| `gontiStringFormatV` | `GtI32 (char* desc, const char* format, void* vaListp)` | Bytes written or `-1` | `vprintf`-style; accepts a `va_list` |

---

## 4. Quick Start Examples

### String length and comparison

```c
#include "CStringTools/GtCStrTools.h"

GtU64 len = gontiStringLength("Hello");          // → 5
GtB8 eq   = gontiStringEqual("abc", "abc");      // → GtTrue
GtB8 neq  = gontiStringEqual("abc", "ABC");      // → GtFalse
```

### Duplicating a string (heap allocation)

```c
char* copy = gontiStringDuplicate(originalName);
// ... use copy ...
gt_free(copy);  // caller must free
```

### Formatting into a pre-allocated buffer

```c
char buf[256];
GtI32 written = gontiStringFormat(buf, "Player %s has %d HP", playerName, health);
// buf now contains the formatted string
// written == number of characters written
```

### Using gontiStringFormatV in a wrapper function

```c
#include <stdarg.h>
#include "CStringTools/GtCStrTools.h"

void myLog(const char* fmt, ...) {
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    gontiStringFormatV(buf, fmt, args);
    va_end(args);
    // ... output buf ...
}
```

---

## 5. Troubleshooting

| Problem | Likely Cause | Solution |
|---------|-------------|----------|
| `gontiStringDuplicate` returns `NULL` | Memory system not initialised or out of memory | Ensure `gontiMemoryInitialize` has been called; check available heap |
| Formatted output is truncated | Result exceeds 32 000 bytes | Break into shorter format strings or pre-process data |
| Memory leak reported for `GT_MEM_TAG_STRING` | `gontiStringDuplicate` result not freed | Call `gt_free` on every string returned by `gontiStringDuplicate` |
| `gontiStringFormat` returns `-1` | `NULL` passed as `desc` | Always provide a valid destination buffer |