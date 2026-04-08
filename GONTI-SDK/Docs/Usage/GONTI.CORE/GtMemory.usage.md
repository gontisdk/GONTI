# GtMemory — Usage Guide

> **Internal mechanics:** [`GtMemory.how-it-works.md`](../../How-It-Works/GONTI.CORE/GtMemory.how-it-works.md)

> **Type definitions:** [`GtMemTypes.usage.md`](GtMemTypes.usage.md)

---

## 1. Purpose

`GtMemory` provides **tagged, tracked heap memory management** with canary-based corruption detection, automatic zero-initialisation, per-tag usage statistics, and leak reporting on shutdown.

---

## 2. Installation / Import

```c
#include "Memory/GtMemory.h"
```

Link against `GONTI.CORE.dll` / `GONTI.CORE.so`. The memory system must be initialised before any allocation macro is used.

---

## 3. Initialisation & Shutdown

```c
// Phase 1: query required state size
GtU64 memReq = 0;
gontiMemoryInitialize(&memReq, 0);

// Phase 2: provide the state buffer
// NOTE: this initial allocation must bypass GtMemory (it is not yet active)
void* memState = malloc(memReq);
gontiMemoryInitialize(&memReq, memState);

// ... application runs ...

// Shutdown — automatically checks for leaks and reports them
gontiMemoryShutdown(memState);
free(memState);
```

---

## 4. API Reference

### 4.1 Primary Macros (preferred interface)

| Macro | Description |
|-------|-------------|
| `gt_allocate(size, memTag)` | Allocates `size` bytes tagged with `memTag`; returns zero-initialised pointer or `NULL` on failure |
| `gt_free(block)` | Frees `block` and sets the variable to `NULL`; validates canary before freeing |
| `gt_reallocate(block, newSize)` | Resizes `block` to `newSize` bytes; preserves tag; returns new pointer |
| `gt_zeroMemory(block, size)` | Zeroes `size` bytes at `block` |
| `gt_copyMemory(dest, source, size)` | Copies `size` bytes from `source` to `dest` |
| `gt_setMemory(dest, value, size)` | Fills `size` bytes at `dest` with `value` |

> In debug builds, `gt_allocate`, `gt_free`, and `gt_reallocate` emit `GTDEBUG` log messages including the pointer address, size, tag, source file, and line number.

### 4.2 Lifecycle Functions

| Function | Signature | Description |
|----------|-----------|-------------|
| `gontiMemoryInitialize` | `void (GtU64* memoryRequirement, void* state)` | Two-phase init |
| `gontiMemoryShutdown` | `void (void* state)` | Shutdown; triggers leak check |

### 4.3 Diagnostic Functions

| Function | Signature | Returns | Description |
|----------|-----------|---------|-------------|
| `gontiMemoryCheckLeaks` | `GtB8 ()` | `GtTrue` if leaks detected | Walks live allocation list and logs leaked blocks |
| `gontiGetMemoryUsageStr` | `char* ()` | Heap-allocated string (caller must free) | Per-tag usage report |
| `gontiMemoryGetAllocCount` | `GtU64 ()` | Current live allocation count | |

### 4.4 Internal Functions (do not call directly)

Functions prefixed with `__gonti` are implementation details exposed via `GTAPI` for macro use only. Use the macros instead.

---

## 5. Quick Start Examples

### Allocating and freeing

```c
#include "Memory/GtMemory.h"

// Allocate a zeroed buffer tagged as renderer memory
GtU8* pixelData = gt_allocate(GT_MEBIBYTES(4), GT_MEM_TAG_RENDERER);

// Use the buffer...

// Free and NULL the pointer
gt_free(pixelData);  // pixelData == NULL after this line
```

### Reallocating a growing buffer

```c
char* buf = gt_allocate(256, GT_MEM_TAG_STRING);
// ... fill buf ...
buf = gt_reallocate(buf, 512);  // grows the buffer, tag is preserved
```

### Printing memory usage

```c
char* report = gontiGetMemoryUsageStr();
GTINFO("%s", report);
gt_free(report);  // caller owns the string
```

### Checking for leaks manually

```c
GtB8 hasLeaks = gontiMemoryCheckLeaks();
if (hasLeaks) {
    // investigate leaked allocations
}
```

---

## 6. Troubleshooting

| Problem | Likely Cause | Solution |
|---------|-------------|----------|
| `"Memory corruption or double free"` error | Writing past allocation bounds or freeing twice | Check array bounds; ensure `gt_free` is called only once per pointer |
| `gt_free` on a `NULL` pointer logs an error | Freed pointer already set to NULL then freed again | `gt_free` guards against this; check calling code logic |
| `gontiGetMemoryUsageStr` returns stale data | Called before initialisation or after shutdown | Only call between `gontiMemoryInitialize` and `gontiMemoryShutdown` |
| Leak reported but pointer looks valid | Object not freed before `gontiMemoryShutdown` | Audit shutdown order; ensure all subsystems are torn down before memory shutdown |
| `GT_MEM_TAG_UNKOWN` warnings in log | `gt_allocate` called with wrong tag | Pass the most specific `GtMemTag` value for the subsystem |