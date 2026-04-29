# GtMemTypes — Usage Guide

> **Internal mechanics:** [`GtMemTypes.how-it-works.md`](../../How-It-Works/GONTI.CORE/GtMemTypes.how-it-works.md)

---

## 1. Purpose

`GtMemTypes.inl` provides the **`GT_MEM_MAGIC` constant, `GtMemTag` enum, and `GtMemAllocHdr` struct** — the shared type contract between the memory subsystem and every module that allocates engine memory.

> ⚠️ **Do not include `GtMemTypes.inl` directly.** It is automatically included by `GtMemory.h`. Consumer code must include `GtMemory.h` instead.

---

## 2. Installation / Import

```c
// Correct — include through GtMemory.h
#include "Memory/GtMemory.h"

// Incorrect — never include directly
// #include "Memory/GtMemTypes.inl"
```

---

## 3. API Reference

### 3.1 GT_MEM_MAGIC

```c
#define GT_MEM_MAGIC 0xDEADC0DE
```

The canary value written into `GtMemAllocHdr::_magic` at allocation time. Checked by `__gontiMemoryFree` and `__gontiMemoryReallocate` before operating on any block:

| Observed value | Interpretation |
|---|---|
| `0xDEADC0DE` | Block is live and intact — proceed |
| `0` | Block was already freed and poisoned — likely double-free |
| Anything else | Header corruption — likely a write overrun from a neighbouring allocation |

> This constant is defined here and not in `GtMemory.c` so that `GtMemory.h` macros (specifically `gt_free`) can perform the canary check inline without calling into the `.c` implementation.

### 3.2 GtMemTag Enumeration

Used to categorise every allocation for diagnostics, statistics, and leak reporting.

| Tag Constant | Intended Subsystem |
|---|---|
| `GT_MEM_TAG_UNKNOWN` | Unclassified / fallback — triggers a `GTWARN` on allocation |
| `GT_MEM_TAG_ARRAY` | Static arrays |
| `GT_MEM_TAG_LINEAR_ALLOC` | Linear allocator blocks |
| `GT_MEM_TAG_DARRAY` | Dynamic arrays (`GtDArray`) |
| `GT_MEM_TAG_DARRAYEX` | Extended dynamic arrays (`GtDArrayEx`) |
| `GT_MEM_TAG_DICT` | Dictionary / hash map |
| `GT_MEM_TAG_RING_QUEUE` | Ring / circular queues |
| `GT_MEM_TAG_BST` | Binary search trees |
| `GT_MEM_TAG_STRING` | String allocations |
| `GT_MEM_TAG_APPLICATION` | Application-level state |
| `GT_MEM_TAG_JOB` | Job system |
| `GT_MEM_TAG_TEXTURE` | Texture data |
| `GT_MEM_TAG_MAT_INST` | Material instances |
| `GT_MEM_TAG_WINDOW` | Window / surface state |
| `GT_MEM_TAG_RENDERER` | Renderer resources |
| `GT_MEM_TAG_ENTRY` | Game entry / main loop state |
| `GT_MEM_TAG_TRANSFORM` | Transform components |
| `GT_MEM_TAG_ENTITY` | Entity objects |
| `GT_MEM_TAG_ENTITY_NODE` | Entity graph nodes |
| `GT_MEM_TAG_SCENE` | Scene data |
| `GT_MEM_TAG_MATH` | Math utility allocations |
| `GT_MEM_TAG_VECTOR` | Vector types |
| `GT_MEM_TAG_MATRIX` | Matrix types |
| `GT_MEM_TAG_BIGINT` | Big integer data |
| `GT_MEM_TAG_MAX_TAGS` | **Sentinel — do not use as an allocation tag** |

### 3.3 GtMemAllocHdr Structure

> ⚠️ This is an **internal type** managed exclusively by `GtMemory`. Consumer code must never read or write `GtMemAllocHdr` fields directly.

```c
typedef struct GtMemAllocHdr {
    GtU64                size;   // User payload size in bytes (excludes header)
    GtMemAllocHdr*  next;   // Forward link in the live-allocation list
    GtMemAllocHdr*  prev;   // Backward link — enables O(1) removal
    GtMemTag             tag;    // Subsystem category
    GtU32                _magic; // Canary: 0xDEADC0DE when live, 0 when freed
} GtMemAllocHdr;
```

---

## 4. Quick Start Examples

### Allocating with a tag

```c
#include "Memory/GtMemory.h"

// Always pass the most specific tag for the allocating subsystem
MyStruct* obj = gt_allocate(sizeof(MyStruct), GT_MEM_TAG_APPLICATION);
```

### Adding a new subsystem tag

1. Open `GtMemTypes.inl` and insert a new entry **before** `GT_MEM_TAG_MAX_TAGS`:

```c
typedef enum GtMemTag {
    // ... existing tags ...
    GT_MEM_TAG_AUDIO,       // ← new tag inserted here
    GT_MEM_TAG_MAX_TAGS     // ← always last
} GtMemTag;
```

2. Open `GtMemory.c` and add the matching string at the **same index** in `memTagNames`:

```c
static const char* const memTagNames[GT_MEM_TAG_MAX_TAGS] = {
    // ... existing entries ...
    "AUDIO",        // ← must match position of GT_MEM_TAG_AUDIO
};
```

The compile-time assertion in `GtMemory.c` will produce a build error if the array length does not match `GT_MEM_TAG_MAX_TAGS`, catching the mismatch immediately.

---

## 5. Troubleshooting

| Problem | Likely Cause | Solution |
|---------|-------------|----------|
| `GT_MEM_TAG_MAX_TAGS` passed to `gt_allocate` | Sentinel used as a real tag | Use any valid `GtMemTag` value other than `GT_MEM_TAG_MAX_TAGS` |
| Build error: array size mismatch | New tag added to enum but `memTagNames` not updated | Add the matching string entry to `memTagNames` in `GtMemory.c` at the correct index |
| Memory stats appear incorrect after adding a tag | `memTagNames` entry added at wrong index | Ensure the string position in `memTagNames` matches the enum value's numeric position |
| `GtMemAllocHdr` fields accessed directly in consumer code | Bypassing the macro API | Use only `gt_allocate`, `gt_free`, and `gt_reallocate` from `GtMemory.h` |