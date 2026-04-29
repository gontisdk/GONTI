# GtLinearAlloc — Usage Guide

> **Internal mechanics:** [`../../How-It-Works/LinearAllocator/GtLinearAlloc.how-it-works.md`](../../How-It-Works/LinearAllocator/GtLinearAlloc.how-it-works.md)

---

## 1. Purpose

`GtLinearAlloc` provides a **bump-pointer allocator** for scenarios where many small allocations are made in bulk and freed all at once. It is significantly faster than `GtMemory`'s tracked allocator for short-lived, frame-scoped, or phase-scoped data.

> ⚠️ Individual allocations **cannot** be freed. Use `GtMemory` (`gt_allocate` / `gt_free`) when you need per-object lifetime control.

---

## 2. Installation / Import

```c
#include "LinearAllocator/GtLinearAlloc.h"
```

Link against `GONTI.CORE.dll` / `GONTI.CORE.so`.

---

## 3. API Reference

### 3.1 GtLinearAlloc Structure

```c
typedef struct GtLinearAlloc {
    GtU64 totalSize;   // Total arena capacity in bytes
    GtU64 allocated;   // Bytes currently consumed
    void* memory;      // Pointer to the backing buffer
    GtB8  ownsMemory;  // GtTrue if the allocator allocated the buffer
} GtLinearAlloc;
```

### 3.2 Functions

| Function | Signature | Description |
|----------|-----------|-------------|
| `gontiLinearAllocatorCreate` | `void (GtU64 totalSize, void* memory, GtLinearAlloc* outAllocator)` | Initialises the allocator. Pass `NULL` for `memory` to let the allocator own the buffer. |
| `gontiLinearAllocatorDestroy` | `void (GtLinearAlloc* allocator)` | Frees the buffer (if owned) and zeroes the struct. |
| `gontiLinearAllocatorFreeAll` | `void (GtLinearAlloc* allocator)` | Resets the cursor to zero and zeroes the backing memory. Does **not** free the buffer. |
| `gontiLinearAllocatorAllocate` | `void* (GtLinearAlloc* allocator, GtU64 size)` | Bumps the cursor by `size`; returns a pointer into the arena or `NULL` on overflow. |

---

## 4. Quick Start Examples

### Self-owned arena

```c
#include "LinearAllocator/GtLinearAlloc.h"

GtLinearAlloc arena;
gontiLinearAllocatorCreate(GT_MEBIBYTES(2), NULL, &arena);  // allocates 2 MiB internally

GtU8* buf   = gontiLinearAllocatorAllocate(&arena, 512);
MyStruct* s = gontiLinearAllocatorAllocate(&arena, sizeof(MyStruct));

// ... use buf and s for this frame / phase ...

gontiLinearAllocatorFreeAll(&arena);  // reset; all pointers above are now invalid

// ... use arena again for the next frame ...

gontiLinearAllocatorDestroy(&arena);  // frees the 2 MiB backing buffer
```

### Caller-provided memory (no internal allocation)

```c
GtU8 stackBuf[4096];
GtLinearAlloc tempArena;
gontiLinearAllocatorCreate(sizeof(stackBuf), stackBuf, &tempArena);

char* tmp = gontiLinearAllocatorAllocate(&tempArena, 128);
// ... use tmp ...

gontiLinearAllocatorDestroy(&tempArena);
// stackBuf is NOT freed — it is stack-allocated; tempArena.ownsMemory == GtFalse
```

### Overflow guard

```c
void* block = gontiLinearAllocatorAllocate(&arena, requestedSize);
if (!block) {
    GTERROR("Arena out of space! Requested %llu B.", requestedSize);
    return;
}
```

---

## 5. When to Use vs. GtMemory

| Criterion | `GtLinearAlloc` | `GtMemory` (`gt_allocate`) |
|-----------|-----------------|---------------------------|
| Individual free needed | ✗ | ✓ |
| Frame-scope scratch data | ✓ | ✗ |
| Per-object lifetime | ✗ | ✓ |
| Allocation overhead | Zero (no header) | `sizeof(GtMemAllocHdr)` per block |
| Leak detection | ✗ (arena is freed as a unit) | ✓ |
| Allocation speed | O(1), branchless increment | O(1) + list prepend + stat update |

---

## 6. Troubleshooting

| Problem | Likely Cause | Solution |
|---------|-------------|----------|
| `gontiLinearAllocatorAllocate` returns `NULL` | Arena capacity exhausted | Increase `totalSize` at creation; monitor `allocator.allocated` vs `allocator.totalSize` |
| Dangling pointer after `FreeAll` | Pointer from a previous allocation used after reset | Treat all pointers from the arena as invalid after `FreeAll` |
| Data corruption after `FreeAll` | Arena zeroed; prior data overwritten | Expected — reset zeroes the entire backing buffer; copy data out before calling `FreeAll` |
| Struct misalignment crash | No alignment enforcement | Add manual padding or use `gontiGetAligned` to compute an aligned `size` before allocating |