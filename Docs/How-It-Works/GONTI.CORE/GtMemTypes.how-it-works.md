# GtMemTypes — How It Works

> **Corresponding Usage file:** [`GtMemTypes.usage.md`](../../Usage/GONTI.CORE/GtMemTypes.usage.md)

---

## 1. Overview

`GtMemTypes.inl` is an **inline header** (`.inl`) that defines the core data types shared between the memory subsystem (`GtMemory`) and every other module that allocates engine memory. It declares two constructs: the `GtMemTag` enumeration and the `GtMemAllocHdr` structure, and one compile-time constant: `GT_MEM_MAGIC`.

This file contains **no executable code** — it is a pure type-declaration unit resolved entirely at compile time. It is included by `GtMemory.h` and must never be included directly by consumer code.

---

## 2. Architecture Overview

```
GtMemTypes.inl
├── GT_MEM_MAGIC  (0xDEADC0DE)
├── GtMemTag  (enum)
│   ├── GT_MEM_TAG_UNKNOWN          (0)
│   ├── GT_MEM_TAG_ARRAY
│   ├── GT_MEM_TAG_LINEAR_ALLOC
│   ├── GT_MEM_TAG_DARRAY
│   ├── GT_MEM_TAG_DARRAYEX
│   ├── GT_MEM_TAG_DICT
│   ├── GT_MEM_TAG_RING_QUEUE
│   ├── GT_MEM_TAG_BST
│   ├── GT_MEM_TAG_STRING
│   ├── GT_MEM_TAG_APPLICATION
│   ├── GT_MEM_TAG_JOB
│   ├── GT_MEM_TAG_TEXTURE
│   ├── GT_MEM_TAG_MAT_INST
│   ├── GT_MEM_TAG_WINDOW
│   ├── GT_MEM_TAG_RENDERER
│   ├── GT_MEM_TAG_ENTRY
│   ├── GT_MEM_TAG_TRANSFORM
│   ├── GT_MEM_TAG_ENTITY
│   ├── GT_MEM_TAG_ENTITY_NODE
│   ├── GT_MEM_TAG_SCENE
│   ├── GT_MEM_TAG_MATH
│   ├── GT_MEM_TAG_VECTOR
│   ├── GT_MEM_TAG_MATRIX
│   ├── GT_MEM_TAG_BIGINT
│   └── GT_MEM_TAG_MAX_TAGS        (sentinel — always last)
└── GtMemAllocHdr  (struct)
    ├── size    : GtU64
    ├── next    : GtMemAllocHdr*
    ├── prev    : GtMemAllocHdr*
    ├── tag     : GtMemTag
    └── _magic  : GtU32
```

---

## 3. Internal Logic

### 3.1 GT_MEM_MAGIC

```c
#define GT_MEM_MAGIC 0xDEADC0DE
```

A compile-time constant written into `GtMemAllocHdr::_magic` at allocation time and checked before every free or reallocation. Its purpose is to detect two classes of memory error cheaply:

- **Corruption / write overrun** — if memory adjacent to a live allocation is written past its bounds, `_magic` may be overwritten. A value other than `0xDEADC0DE` on entry to `__gontiMemoryFree` or `__gontiMemoryReallocate` signals this.
- **Double-free** — `__gontiMemoryFree` clears `_magic` to `0` before calling the platform free. A subsequent free of the same pointer will find `_magic == 0` and reject the operation with a `GTERROR`.

The value `0xDEADC0DE` is chosen for debugger legibility — it is immediately recognisable in a hex memory dump and is statistically unlikely to appear as naturally-occurring data.

### 3.2 GtMemTag Enumeration

`GtMemTag` categorises every heap allocation made through `GtMemory`. Its primary purposes are:

- **Diagnostics** — `gontiGetMemoryUsageStr()` reports per-tag memory consumption.
- **Leak detection** — `gontiMemoryCheckLeaks()` displays the tag name alongside each leaked block's address and size.
- **Statistics** — `GtMemSysState` maintains a `taggedAllocations[GT_MEM_TAG_MAX_TAGS]` array indexed directly by tag value.

`GT_MEM_TAG_MAX_TAGS` is a **sentinel value** placed last in the enum. It serves as both:
1. The compile-time size of the `taggedAllocations` array.
2. A loop termination condition when iterating over all tags.

A compile-time assertion in `GtMemory.c` verifies that the `memTagNames` string array length equals `GT_MEM_TAG_MAX_TAGS`, catching mismatches between the enum and the string table at build time.

Adding a new subsystem tag requires inserting a new entry **before** `GT_MEM_TAG_MAX_TAGS` only — the sentinel and the assertion adjust automatically.

### 3.3 GtMemAllocHdr Structure

`GtMemAllocHdr` is a **metadata prefix** prepended to every allocation made through `__gontiMemoryAllocate`. The memory layout is:

```
[ GtMemAllocHdr | user data ... ]
         ↑
  gontiPlatformAllocate returns this address;
  (header + 1) is the pointer returned to the caller.
```

#### Fields

| Field | Type | Purpose |
|-------|------|---------|
| `size` | `GtU64` | Byte count of the user payload (excludes header size) |
| `next` | `GtMemAllocHdr*` | Forward link in the global intrusive linked list of live allocations |
| `prev` | `GtMemAllocHdr*` | Backward link — enables O(1) removal without list traversal |
| `tag` | `GtMemTag` | Subsystem category for diagnostics and statistics |
| `_magic` | `GtU32` | Canary value `0xDEADC0DE` — detects corruption and double-free |

#### Intrusive Linked List

All live allocations are chained through `next` / `prev` into a doubly-linked list whose head is `GtMemSysState::listHead`. This enables:

- **O(1) insertion** — new allocations are prepended at the head via `listPrepend()`.
- **O(1) removal** — `prev` / `next` pointers allow unlinking without traversal via `listRemove()`.
- **Full leak enumeration** — at shutdown, the list is walked to report any surviving allocations with their address, size, and tag.

#### Magic Number Canary

The `_magic` field is set to `GT_MEM_MAGIC` (`0xDEADC0DE`) upon allocation and cleared to `0` upon free. Before any free or reallocation, `GtMemory` validates this field:

- **Value other than `0xDEADC0DE`** → memory corruption or write overrun.
- **Value is `0`** → likely double-free (the block was already freed and poisoned).

This provides a lightweight first line of defense against common memory errors without requiring a separate sanitizer.

---

## 4. Dependency Graph

```
GtDefines.inl
     ↑
GtMemTypes.inl
     ↑
GtMemory.h   (includes GtMemTypes.inl)
     ↑
all modules using gt_allocate / gt_free / gt_reallocate
```

---

## 5. Design Rationale

- **Separation into `.inl`** — keeping type declarations out of `GtMemory.h` avoids circular inclusion when other modules (e.g., `GtLogger`) need `GtMemTag` without pulling in the full memory API.
- **`GT_MEM_MAGIC` as a `#define`** — a preprocessor constant is a true compile-time literal with no storage, no linkage, and no type ambiguity across translation units. A `static const GtU32` would require the compiler to treat it as a compile-time constant (guaranteed in C++ but not in C), making `#define` the correct choice here.
- **Intrusive linked list over external bookkeeping** — no secondary allocation is needed to track allocations; the metadata lives inside the same memory block, keeping the allocator self-contained.
- **Sentinel tag pattern** — `GT_MEM_TAG_MAX_TAGS` makes array sizing and loop bounds self-maintaining as new tags are added, and enables a build-time assertion to enforce consistency between the enum and the string table.