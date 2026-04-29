# GtLinearAlloc — How It Works

> **Corresponding Usage file:** [GtLinearAlloc.usage.md`](../../Usage/GONTI.CORE/GtLinearAlloc.usage.md)

---

## 1. Overview

`GtLinearAlloc` implements a **linear (bump-pointer) allocator** — one of the simplest and fastest memory allocation strategies. It maintains a contiguous memory block and a single offset cursor. Allocation advances the cursor; individual frees are not supported. The entire arena is reclaimed at once via `gontiLinearAllocatorFreeAll` or when the allocator is destroyed.

---

## 2. Architecture Overview

```
GtLinearAlloc.h
├── GtLinearAlloc struct { totalSize, allocated, memory*, ownsMemory }
└── GTAPI declarations (4 functions)

GtLinearAlloc.c
├── #include GtMemory.h  — gt_allocate, gt_free, gt_zeroMemory
├── #include GtLogger.h  — GTERROR
│
├── gontiLinearAllocatorCreate()    — initialise struct, optionally allocate backing memory
├── gontiLinearAllocatorDestroy()   — free backing memory if owned, zero struct
├── gontiLinearAllocatorFreeAll()   — reset cursor to 0, zero backing memory
└── gontiLinearAllocatorAllocate()  — bump cursor, return pointer into arena
```

---

## 3. Internal Logic

### 3.1 GtLinearAlloc Structure

```c
typedef struct GtLinearAlloc {
    GtU64  totalSize;   // Total arena capacity in bytes
    GtU64  allocated;   // Current bump cursor (bytes consumed)
    void*  memory;      // Pointer to the backing buffer
    GtB8   ownsMemory;  // GtTrue if this allocator allocated the buffer itself
} GtLinearAlloc;
```

The `ownsMemory` flag determines whether `gontiLinearAllocatorDestroy` will `gt_free` the backing buffer. If the caller provides an external memory block (e.g., a stack array or a sub-range of a larger arena), ownership remains with the caller.

### 3.2 Create (`gontiLinearAllocatorCreate`)

```
memory == NULL  →  ownsMemory = GtTrue,  gt_allocate(totalSize, GT_MEM_TAG_LINEAR_ALLOC)
memory != NULL  →  ownsMemory = GtFalse, use provided pointer directly
```

In both cases `allocated` is reset to `0`.

### 3.3 Allocation (`gontiLinearAllocatorAllocate`)

```
if (allocated + size > totalSize) → GTERROR, return NULL
block = memory + allocated
allocated += size
return block
```

This is a **bump-pointer** (also called *stack allocator* or *arena allocator*) pattern:

```
┌──────────────────────────────────────────────────────────┐
│  used (allocated)  │         free space                  │
└──────────────────────────────────────────────────────────┘
0                    ↑ cursor                              totalSize
```

Each call advances the cursor by `size`. The returned pointer is directly into the arena — no header, no metadata. Individual allocations cannot be freed independently.

### 3.4 FreeAll (`gontiLinearAllocatorFreeAll`)

Resets `allocated` to `0` and zeroes the entire backing buffer via `gt_zeroMemory`. This reclaims the full arena capacity for reuse in a single O(n) pass over the memory.

### 3.5 Destroy (`gontiLinearAllocatorDestroy`)

If `ownsMemory == GtTrue`, calls `gt_free(allocator->memory)`. Zeroes all struct fields regardless of ownership.

---

## 4. Dependency Graph

```
GtDefines.inl  ←  GtLinearAlloc.h
GtMemory.h   ←  GtLinearAlloc.c  (gt_allocate, gt_free, gt_zeroMemory)
GtLogger.h   ←  GtLinearAlloc.c  (GTERROR)
```

---

## 5. Design Rationale

- **Zero per-allocation overhead** — unlike `GtMemory`'s tracked allocations, the linear allocator stores no header per block. The only overhead is the four fields in `GtLinearAlloc` itself.
- **External memory support** — the `memory` parameter in `gontiLinearAllocatorCreate` allows the allocator to operate over a caller-supplied buffer (e.g., a sub-region of a global arena, a stack-allocated array for frame-local data, or a memory-mapped file region).
- **Bulk reset over individual free** — frame-scope data, per-level scratch allocations, and temporary parsing buffers are all natural fits: allocate freely during a phase, reset once the phase is complete.
- **No alignment** — the current implementation does not align returned pointers. Callers requiring aligned access to SIMD types or structs with specific alignment requirements must add padding manually or use `gontiGetAligned` from `GtDefines.inl`.