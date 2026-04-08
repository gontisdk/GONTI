# GtMemTypes — How It Works

> **Corresponding Usage file:** [`GtMemTypes.usage.md`](../../Usage/GONTI.CORE/GtMemTypes.usage.md)

---

## 1. Overview

`GtMemTypes.inl` is an **inline header** (`.inl`) that defines the core data types shared between the Memory subsystem (`GtMemory`) and every other module that allocates engine memory. It declares two constructs: the `GtMemTag` enumeration and the `GtAllocationHeader` structure.

This file contains **no executable code** — it is a pure type-declaration unit resolved entirely at compile time. It is included by `GtMemory.h` and must never be included directly by consumer code.

---

## 2. Architecture Overview

```
GtMemTypes.inl
├── GtMemTag  (enum)
│   ├── GT_MEM_TAG_UNKOWN          (0)
│   ├── GT_MEM_TAG_ARRAY
│   ├── GT_MEM_TAG_LINEAR_ALLOCATOR
│   ├── GT_MEM_TAG_DARRAY
│   ├── GT_MEM_TAG_DARRAYEX
│   ├── GT_MEM_TAG_DICT
│   ├── GT_MEM_TAG_RING_QUEUE
│   ├── GT_MEM_TAG_BST
│   ├── GT_MEM_TAG_STRING
│   ├── GT_MEM_TAG_APPLICATION
│   ├── GT_MEM_TAG_JOB
│   ├── GT_MEM_TAG_TEXTURE
│   ├── GT_MEM_TAG_MATERIAL_INSTANCE
│   ├── GT_MEM_TAG_WINDOW
│   ├── GT_MEM_TAG_RENDERER
│   ├── GT_MEM_TAG_ENTRY
│   ├── GT_MEM_TAG_TRANSFORM
│   ├── GT_MEM_TAG_ENTITY
│   ├── GT_MEM_TAG_ENTITY_NODE
│   ├── GT_MEM_TAG_SCENE
│   ├── GT_MEM_TAG_MATH
│   ├── GT_MEM_TAG_VECTORS
│   ├── GT_MEM_TAG_MATRICES
│   ├── GT_MEM_TAG_BIGINT
│   └── GT_MEM_TAG_MAX_TAGS        (sentinel — always last)
└── GtAllocationHeader  (struct)
    ├── size    : GtU64
    ├── next    : GtAllocationHeader*
    ├── prev    : GtAllocationHeader*
    ├── tag     : GtMemTag
    └── _magic  : GtU32
```

---

## 3. Internal Logic

### 3.1 GtMemTag Enumeration

`GtMemTag` categorises every heap allocation made through `GtMemory`. Its primary purposes are:

- **Diagnostics** — `gontiGetMemoryUsageStr()` reports per-tag memory consumption.
- **Leak detection** — `gontiMemoryCheckLeaks()` can display the tag alongside each leaked block.
- **Statistics** — `GtMemSysState` keeps a `taggedAllocations[GT_MEM_TAG_MAX_TAGS]` array indexed by tag value.

`GT_MEM_TAG_MAX_TAGS` is a **sentinel value** placed last in the enum. It serves as both:
1. The upper bound for the `taggedAllocations` array size.
2. A loop termination condition when iterating over all tags.

Adding a new subsystem tag requires inserting a new entry **before** `GT_MEM_TAG_MAX_TAGS` only; the sentinel automatically adjusts.

### 3.2 GtAllocationHeader Structure

`GtAllocationHeader` is a **metadata prefix** prepended to every allocation made through `__gontiMemoryAllocate`. The memory layout is:

```
[ GtAllocationHeader ][ user data ... ]
        ↑
   pointer returned to caller is (header + 1)
```

#### Fields

| Field | Type | Purpose |
|-------|------|---------|
| `size` | `GtU64` | Byte count of the user payload (excludes header size) |
| `next` | `GtAllocationHeader*` | Forward link in the global intrusive linked list of live allocations |
| `prev` | `GtAllocationHeader*` | Backward link (doubly-linked for O(1) removal) |
| `tag` | `GtMemTag` | Subsystem category for diagnostics and statistics |
| `_magic` | `GtU32` | Canary value `0xDEADBEEF`; detects corruption and double-free |

#### Intrusive Linked List

All live allocations are chained together through `next`/`prev` into a doubly-linked list whose head is `GtMemSysState::listHead`. This structure enables:

- **O(1) insertion** — new allocations are prepended at the head.
- **O(1) removal** — `prev`/`next` pointers allow unlinking without traversal.
- **Full leak enumeration** — at shutdown, the list is walked to report any surviving allocations.

#### Magic Number Canary

The `_magic` field is set to `0xDEADBEEF` upon allocation and cleared to `0` upon free. Before any free or reallocation, `GtMemory` checks this value:

- Mismatch → memory corruption or write overrun.
- Zero → likely double-free.

This provides a lightweight first line of defense against common memory errors without requiring a separate memory sanitizer.

---

## 4. Dependency Graph

```
GtDefines.h
     ↑
GtMemTypes.inl
     ↑
GtMemory.h   (includes GtMemTypes.inl)
     ↑
all modules using gt_allocate / gt_free
```

---

## 5. Design Rationale

- **Separation into `.inl`** — keeping type declarations out of `GtMemory.h` avoids circular inclusion when other modules (e.g., `GtLogger`) need `GtMemTag` without pulling the full memory API.
- **Intrusive linked list over external bookkeeping** — no secondary allocation is needed to track allocations; the metadata lives inside the same memory block.
- **Sentinel tag pattern** — `GT_MEM_TAG_MAX_TAGS` makes array sizing and loop bounds self-maintaining as new tags are added.