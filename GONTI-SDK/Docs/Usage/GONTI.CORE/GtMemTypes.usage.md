# GtMemTypes — Usage Guide

> **Internal mechanics:** [`GtMemTypes.how-it-works.md`](../../How-It-Works/GONTI.CORE/GtMemTypes.how-it-works.md)

---

## 1. Purpose

`GtMemTypes.inl` provides the **`GtMemTag` enum and `GtAllocationHeader` struct** — the shared type contract between the memory subsystem and every module that allocates engine memory.

> ⚠️ **Do not include `GtMemTypes.inl` directly.** It is automatically included by `GtMemory.h`. Consumer code should include `GtMemory.h` instead.

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

### 3.1 GtMemTag Enumeration

Used to categorise every allocation for diagnostics, statistics, and leak reporting.

| Tag Constant | Intended Subsystem |
|---|---|
| `GT_MEM_TAG_UNKOWN` | Unclassified / fallback |
| `GT_MEM_TAG_ARRAY` | Static arrays |
| `GT_MEM_TAG_LINEAR_ALLOCATOR` | Linear allocator blocks |
| `GT_MEM_TAG_DARRAY` | Dynamic arrays (`GtDArray`) |
| `GT_MEM_TAG_DARRAYEX` | Extended dynamic arrays (`GtDArrayEx`) |
| `GT_MEM_TAG_DICT` | Dictionary / hash map |
| `GT_MEM_TAG_RING_QUEUE` | Ring / circular queues |
| `GT_MEM_TAG_BST` | Binary search trees |
| `GT_MEM_TAG_STRING` | String allocations |
| `GT_MEM_TAG_APPLICATION` | Application-level state |
| `GT_MEM_TAG_JOB` | Job system |
| `GT_MEM_TAG_TEXTURE` | Texture data |
| `GT_MEM_TAG_MATERIAL_INSTANCE` | Material instances |
| `GT_MEM_TAG_WINDOW` | Window/surface state |
| `GT_MEM_TAG_RENDERER` | Renderer resources |
| `GT_MEM_TAG_ENTRY` | Game entry / main loop state |
| `GT_MEM_TAG_TRANSFORM` | Transform components |
| `GT_MEM_TAG_ENTITY` | Entity objects |
| `GT_MEM_TAG_ENTITY_NODE` | Entity graph nodes |
| `GT_MEM_TAG_SCENE` | Scene data |
| `GT_MEM_TAG_MATH` | Math utility allocations |
| `GT_MEM_TAG_VECTORS` | Vector types |
| `GT_MEM_TAG_MATRICES` | Matrix types |
| `GT_MEM_TAG_BIGINT` | Big integer data |
| `GT_MEM_TAG_MAX_TAGS` | **Sentinel — do not use as a tag** |

### 3.2 GtAllocationHeader Structure

> ⚠️ This is an **internal type** managed exclusively by `GtMemory`. Consumer code must never read or write `GtAllocationHeader` fields directly.

```c
typedef struct GtAllocationHeader {
    GtU64 size;                     // User payload size in bytes
    struct GtAllocationHeader* next; // Next allocation in the live list
    struct GtAllocationHeader* prev; // Previous allocation in the live list
    GtMemTag tag;                   // Subsystem category
    GtU32 _magic;                   // Corruption canary (0xDEADBEEF)
} GtAllocationHeader;
```

---

## 4. Quick Start Examples

### Allocating with a tag

```c
#include "Memory/GtMemory.h"

// Always pass the most specific tag for the allocation
MyStruct* obj = gt_allocate(sizeof(MyStruct), GT_MEM_TAG_APPLICATION);
```

### Adding a new subsystem tag

Edit `GtMemTypes.inl` and insert a new entry **before** `GT_MEM_TAG_MAX_TAGS`:

```c
typedef enum GtMemTag {
    // ... existing tags ...
    GT_MEM_TAG_AUDIO,       // ← new tag
    GT_MEM_TAG_MAX_TAGS     // ← always last
} GtMemTag;
```

Then add a corresponding string entry at the same index in both `memoryTagStrings` and `memTagClearArr` arrays inside `GtMemory.c`.

---

## 5. Troubleshooting

| Problem | Likely Cause | Solution |
|---------|-------------|----------|
| `GT_MEM_TAG_MAX_TAGS` used as allocation tag | Wrong tag passed to `gt_allocate` | Use any other valid `GtMemTag` value |
| Memory stats appear off after adding a tag | String arrays in `GtMemory.c` not updated | Add matching entries to `memoryTagStrings` and `memTagClearArr` at the correct index |
| `GtAllocationHeader` fields read directly | Consumer code accesses internals | Use only the `gt_allocate` / `gt_free` / `gt_reallocate` macros from `GtMemory.h` |