# GtMemory — How It Works

> **Corresponding Usage file:** [`GtMemory.usage.md`](../../Usage/GONTI.CORE/GtMemory.usage.md)

---

## 1. Overview

`GtMemory` is the **heap memory management subsystem** of `GONTI.CORE`. It wraps platform allocations with tagged tracking, corruption detection via a magic canary, a doubly-linked live-allocation list for leak detection, and per-tag usage statistics. Consumer code accesses it exclusively through three macros: `gt_allocate`, `gt_free`, and `gt_reallocate`.

---

## 2. Architecture Overview

```
GtMemory.h
├── Macro interface (gt_allocate, gt_free, gt_reallocate, gt_zeroMemory, gt_copyMemory, gt_setMemory)
└── GTAPI declarations

GtMemory.c
├── memoryTagStrings[GT_MEM_TAG_MAX_TAGS]   — padded display strings
├── memTagClearArr[GT_MEM_TAG_MAX_TAGS]     — plain tag name strings
├── GtMemStats { totalAllocatedCount, taggedAllocations[] }
├── GtMemSysState { stats, listHead*, allocCount }  ← file-scope singleton
│
├── gontiMemoryInitialize()
├── gontiMemoryShutdown()        → gontiMemoryCheckLeaks()
├── __gontiMemoryAllocate()      ← prepend GtAllocationHeader, insert into list
├── __gontiMemoryFree()          ← validate magic, unlink from list, platform free
├── __gontiMemoryReallocate()    ← validate magic, platform realloc, fix list links
├── __gontiMemoryZero/Copy/Set() ← thin delegation to GtPlatform
├── gontiMemoryCheckLeaks()      ← walk listHead linked list
├── gontiGetMemoryUsageStr()     ← format per-tag stats into a string
└── __gontiGetMemoryTagStringsArr()
```

---

## 3. Internal Logic

### 3.1 State Singleton

```c
typedef struct GtMemSysState {
    struct GtMemStats stats;
    GtAllocationHeader* listHead;
    GtU64 allocCount;
} GtMemSysState;
static GtMemSysState* statePtr;
```

`statePtr` is a pointer to externally-managed memory provided during `gontiMemoryInitialize`. This two-phase pattern allows the engine to control placement of subsystem state (e.g., in a dedicated arena). If `statePtr` is `NULL`, allocation and free functions still perform platform calls but skip all bookkeeping — this allows safe use before full initialisation.

### 3.2 Allocation Header Layout

Every allocation prepends a `GtAllocationHeader` to the user payload:

```
┌─────────────────────────────────┐
│        GtAllocationHeader        │  ← returned by gontiPlatformAllocate
│  size   next  prev  tag  _magic │
├─────────────────────────────────┤
│         user payload            │  ← pointer returned to caller
└─────────────────────────────────┘
```

The pointer arithmetic `(void*)(header + 1)` advances past the header to yield the user-visible address.

### 3.3 Allocation Path (`__gontiMemoryAllocate`)

1. Warn if `GT_MEM_TAG_UNKOWN` is used.
2. Compute `total = size + sizeof(GtAllocationHeader)`.
3. Call `gontiPlatformAllocate(total, GtFalse)`.
4. Populate header fields: `size`, `tag`, `_magic = 0xDEADBEEF`.
5. **Prepend to linked list**: new node becomes `listHead`; old head's `prev` is updated.
6. Update `stats.totalAllocatedCount`, `stats.taggedAllocations[tag]`, `allocCount`.
7. Zero the user payload via `gontiPlatformZeroMemory`.
8. Return `(void*)(header + 1)`.

### 3.4 Free Path (`__gontiMemoryFree`)

1. Guard against `NULL` block.
2. Recover header: `header = ((GtAllocationHeader*)block) - 1`.
3. **Canary check**: if `header->_magic != 0xDEADBEEF` → log error and return (do not double-free).
4. **Unlink from list**: update `prev->next` and `next->prev`; update `listHead` if unlinking the head node.
5. Update stats.
6. Clear `_magic`, `next`, `prev` to prevent use-after-free reuse.
7. Call `gontiPlatformFree(header, GtFalse)`.

The `gt_free` macro additionally:
- Skips `NULL` pointers with an error log.
- Reads the header before calling the function (for logging size/tag).
- Sets the caller's pointer variable to `NULL` after free (via `do { ... } while(0)` pattern).

### 3.5 Reallocate Path (`__gontiMemoryReallocate`)

1. `NULL` block → delegates to `__gontiMemoryAllocate(newSize, GT_MEM_TAG_UNKOWN)`.
2. `newSize == 0` → delegates to `__gontiMemoryFree`, returns `NULL`.
3. Recover and validate header magic.
4. Call `gontiPlatformReallocate(oldHeader, newSize + sizeof(GtAllocationHeader))`.
5. If the returned pointer differs from the old one (memory moved), fix `prev->next` and `next->prev` links.
6. Update stats: subtract `oldSize` from tag bucket, add `newSize`.

### 3.6 Leak Detection (`gontiMemoryCheckLeaks`)

Called automatically by `gontiMemoryShutdown`. Walks the `listHead` linked list:
- If `listHead == NULL` → logs "All clear."
- Otherwise → logs each surviving block's address, size, and tag, then totals leaked bytes.

Returns `GtTrue` if leaks were detected, `GtFalse` if clean.

### 3.7 Usage Statistics (`gontiGetMemoryUsageStr`)

Iterates all `GT_MEM_TAG_MAX_TAGS` entries, auto-selects the most appropriate unit (B, KiB, MiB, GiB), formats into a stack buffer, then duplicates it to heap via `gontiStringDuplicate`. **Caller is responsible for freeing the returned string.**

---

## 4. Dependency Graph

```
GtDefines.h    ←  GtMemory.h
GtLogger.h     ←  GtMemory.h  (GTDEBUG/GTERROR/GTWARN macros)
GtMemTypes.inl ←  GtMemory.h
GtPlatform.h   ←  GtMemory.c  (platform alloc/free/zero/copy/set/realloc)
GtCStrTools.h  ←  GtMemory.c  (gontiStringLength, gontiStringDuplicate)
```

---

## 5. Design Rationale

- **Intrusive linked list** — no secondary allocation needed to track live blocks; the metadata is embedded within the same allocation, keeping overhead minimal.
- **Magic canary** — `0xDEADBEEF` provides lightweight corruption and double-free detection without the overhead of a full memory sanitizer.
- **Zero-on-allocate** — all allocations are zero-initialised, eliminating a class of bugs from uninitialised reads at the cost of one extra `memset` per allocation.
- **Tag-based statistics** — per-subsystem tracking enables targeted memory budget analysis and helps identify which system is responsible for unexpected growth.
- **Macro interface with logging** — `gt_allocate`, `gt_free`, and `gt_reallocate` emit `GTDEBUG` messages with file/line context, providing an allocation trace in debug builds at zero cost in release.