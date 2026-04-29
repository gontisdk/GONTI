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
gontiMemoryInitialize(&memReq, NULL);

// Phase 2: provide the state buffer
// NOTE: this bootstrap allocation must bypass GtMemory — the system is not yet active
void* memState = malloc(memReq);
gontiMemoryInitialize(&memReq, memState);

// ... application runs ...

// Shutdown — automatically checks for leaks and reports them via GTERROR
gontiMemoryShutdown(memState);
free(memState);
```

---

## 4. API Reference

### 4.1 Primary Macros (preferred interface)

| Macro | Description |
|-------|-------------|
| `gt_allocate(size, memTag)` | Allocates `size` zero-initialised bytes tagged with `memTag`; returns a valid pointer or `NULL` on failure |
| `gt_free(block)` | Validates the canary, frees `block`, and sets the variable to `NULL`; no-op with error log if `block` is `NULL` |
| `gt_reallocate(block, newSize)` | Resizes `block` to `newSize` bytes; preserves the original tag; newly appended bytes are zero-initialised; returns the new pointer |
| `gt_zeroMemory(block, size)` | Zeroes `size` bytes starting at `block` |
| `gt_copyMemory(dest, source, size)` | Copies `size` bytes from `source` to `dest` |
| `gt_setMemory(dest, value, size)` | Fills `size` bytes at `dest` with `value` |

> In debug builds, `gt_allocate`, `gt_free`, and `gt_reallocate` emit `GTDEBUG` log messages including the pointer address, size, tag name, source file, and line number.

> **`gt_free` sets the pointer to `NULL` after freeing.** The variable itself is modified — do not pass a pointer cast or a non-lvalue expression.

### 4.2 Lifecycle Functions

| Function | Signature | Description |
|----------|-----------|-------------|
| `gontiMemoryInitialize` | `void (GtU64* memoryRequirement, void* state)` | Two-phase init: first call with `state = NULL` to query size; second call with the allocated buffer to activate |
| `gontiMemoryShutdown` | `void (void* state)` | Flush stats, run leak check, clear internal pointer |

### 4.3 Diagnostic Functions

| Function | Signature | Returns | Description |
|----------|-----------|---------|-------------|
| `gontiMemoryCheckLeaks` | `GtB8 (void)` | `GtTrue` if leaks detected | Walks the live allocation list and logs each leaked block (address, size, tag) |
| `gontiGetMemoryUsageStr` | `char* (void)` | Heap-allocated string — **caller must free via `gt_free()`** | Per-tag usage report formatted for logging |
| `gontiMemoryGetAllocCount` | `GtU64 (void)` | Current live allocation count | Safe to call with `NULL` state (returns `0`) |

### 4.4 Internal Functions (do not call directly)

Functions prefixed with `__gonti` are implementation details exposed via `GTAPI` solely for use by the macros defined in `GtMemory.h`. Always use the macros instead.

---

## 5. Quick Start Examples

### Allocating and freeing

```c
#include "Memory/GtMemory.h"

// Allocate a zero-initialised buffer tagged as renderer memory
GtU8* pixelData = gt_allocate(GT_MEBIBYTES(4), GT_MEM_TAG_RENDERER);

// Use the buffer ...

// Free and NULL the pointer in one step
gt_free(pixelData);
// pixelData == NULL here
```

### Reallocating a growing buffer

```c
char* buf = gt_allocate(256, GT_MEM_TAG_STRING);
// ... fill buf ...

// Grow the buffer — tag is preserved, new bytes are zero-initialised
buf = gt_reallocate(buf, 512);
```

### Printing memory usage

```c
char* report = gontiGetMemoryUsageStr();
GTINFO("%s", report);
gt_free(report);  // caller owns the returned string
```

### Checking for leaks manually

```c
GtB8 hasLeaks = gontiMemoryCheckLeaks();
if (hasLeaks) {
    // investigate leaked allocations logged above
}
```

---

## 6. Troubleshooting

| Symptom | Likely Cause | Solution |
|---------|-------------|----------|
| `"corruption or double-free"` logged by `__gontiMemoryFree` | Write past allocation bounds, or `gt_free` called twice on the same pointer | Audit array bounds; ensure `gt_free` is called exactly once per allocation |
| `"LIST CORRUPT"` logged by `listRemove` | Linked list head pointer inconsistency — severe corruption or double-free | Treat as a critical error; use a memory sanitizer (ASan) to locate the root cause |
| `gt_free` logs `"NULL PTR"` | Freeing a pointer already set to `NULL` | `gt_free` sets the variable to `NULL` after the first call — check calling code for redundant frees |
| `gontiGetMemoryUsageStr` returns stale or zero data | Called before initialisation or after shutdown | Only call between `gontiMemoryInitialize` and `gontiMemoryShutdown` |
| Leak reported at shutdown but pointer looks valid | Object not freed before `gontiMemoryShutdown` | Audit subsystem teardown order; all systems must shut down before the memory system |
| `GT_MEM_TAG_UNKNOWN` warnings in log | `gt_allocate` called without a specific tag | Pass the most specific `GtMemTag` value for the allocating subsystem |
| Stats appear incorrect after adding a new tag | `memTagNames` array in `GtMemory.c` not updated | Add the matching string entry at the same index as the new enum value; the compile-time assertion will catch size mismatches |