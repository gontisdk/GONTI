# GtMemory — How It Works

> **Corresponding Usage file:** [`GtMemory.usage.md`](../../Usage/GONTI.CORE/GtMemory.usage.md)

---

## 1. Overview

`GtMemory` is the **heap memory management subsystem** of `GONTI.CORE`. It wraps platform allocations with tagged tracking, corruption detection via a magic canary, a doubly-linked live-allocation list for leak detection, and per-tag usage statistics. Consumer code accesses it exclusively through macros: `gt_allocate`, `gt_free`, `gt_reallocate`, `gt_zeroMemory`, `gt_copyMemory`, and `gt_setMemory`.

---

## 2. Architecture Overview

```
GtMemory.h
├── Macro interface (gt_allocate, gt_free, gt_reallocate, gt_zeroMemory, gt_copyMemory, gt_setMemory)
└── GTAPI declarations

GtMemory.c
├── memTagNames[GT_MEM_TAG_MAX_TAGS]        — plain tag name strings (single source of truth)
├── GtMemStats { totalAllocatedCount, taggedAllocations[] }
├── GtMemSysState { stats, listHead*, allocCount }  ← file-scope singleton
│
├── listRemove()                 ← unlink a header node from the live-allocation list
├── listPrepend()                ← insert a header node at the head of the list
├── formatBytes()                ← convert raw byte count to human-readable unit
├── gontiMemoryInitialize()
├── gontiMemoryShutdown()        → gontiMemoryCheckLeaks()
├── gontiMemoryCheckLeaks()      ← walk listHead linked list
├── gontiGetMemoryUsageStr()     ← format per-tag stats into a heap-allocated string
├── gontiMemoryGetAllocCount()
├── __gontiMemoryAllocate()      ← prepend GtMemAllocHdr, insert into list
├── __gontiMemoryFree()          ← validate magic, unlink from list, platform free
├── __gontiMemoryReallocate()    ← validate magic, platform realloc, fix list links, zero growth
├── __gontiMemoryZero/Copy/Set() ← thin delegation to GtPlatform
└── __gontiGetMemoryTagStringsArr()
```

---

## 3. Internal Logic

### 3.1 State Singleton

```c
typedef struct GtMemSysState {
    GtMemStats          stats;
    GtMemAllocHdr* listHead;
    GtU64               allocCount;
} GtMemSysState;

static GtMemSysState* s_state = NULL;
```

`s_state` is a pointer to externally-managed memory provided during `gontiMemoryInitialize`. This two-phase pattern allows the engine to control placement of subsystem state (e.g., in a dedicated arena). If `s_state` is `NULL`, allocation and free functions still perform platform calls but skip all bookkeeping — this allows safe use before full initialisation.

### 3.2 Allocation Header Layout

Every allocation prepends a `GtMemAllocHdr` to the user payload:

```
┌──────────────────────────────────┐
│        GtMemAllocHdr         │  ← returned by gontiPlatformAllocate
│  size   next   prev   tag  _magic │
├──────────────────────────────────┤
│          user payload             │  ← pointer returned to caller
└──────────────────────────────────┘
```

The pointer arithmetic `(void*)(header + 1)` advances past the header to yield the user-visible address. The inverse, `((GtMemAllocHdr*)block) - 1`, recovers the header from a user pointer.

### 3.3 Allocation Path (`__gontiMemoryAllocate`)

1. Warn if `GT_MEM_TAG_UNKNOWN` is used.
2. Compute `total = size + sizeof(GtMemAllocHdr)`.
3. Call `gontiPlatformAllocate(total, GtFalse)`.
4. Populate header fields: `size`, `tag`, `_magic = GT_MEM_MAGIC` (`0xDEADC0DE`).
5. **Prepend to linked list** via `listPrepend()`: new node becomes `listHead`; old head's `prev` is updated.
6. Update `stats.totalAllocatedCount`, `stats.taggedAllocations[tag]`, `allocCount`.
7. Zero the user payload via `gontiPlatformZeroMemory`.
8. Return `(void*)(header + 1)`.

### 3.4 Free Path (`__gontiMemoryFree`)

1. Guard against `NULL` block.
2. Recover header: `header = ((GtMemAllocHdr*)block) - 1`.
3. **Canary check**: if `header->_magic != GT_MEM_MAGIC` (`0xDEADC0DE`) → log error and return without freeing (prevents double-free).
4. **Unlink from list** via `listRemove()`:
   - If the node has a `prev`, patch `prev->next`.
   - If the node has no `prev`, it must be the list head — assert this and advance `listHead`. If the assertion fails (node has no `prev` but is not the head), a `GTERROR` is emitted and the operation is aborted to avoid deepening list corruption.
5. Update `stats.totalAllocatedCount`, `stats.taggedAllocations[tag]`, `allocCount`.
6. Poison the header: clear `_magic` to `0`, null out `next` and `prev` to catch future use-after-free.
7. Call `gontiPlatformFree(header, GtFalse)`.

The `gt_free` macro additionally:
- Logs a `GTERROR` and skips the call if `block` is `NULL`.
- Reads the header before calling the function (for logging `size` and `tag` with file/line context).
- Sets the caller's pointer variable to `NULL` after free (via `do { ... } while(0)` pattern).

### 3.5 Reallocate Path (`__gontiMemoryReallocate`)

1. `NULL` block → delegates to `__gontiMemoryAllocate(newSize, GT_MEM_TAG_UNKNOWN)`.
2. `newSize == 0` → delegates to `__gontiMemoryFree`, returns `NULL`.
3. Recover header and validate `_magic` (`0xDEADC0DE`).
4. Call `gontiPlatformReallocate(oldHeader, newSize + sizeof(GtMemAllocHdr))`.
5. Update `newHdr->size`.
6. If the returned pointer differs from the old one (memory moved), fix `prev->next` and `next->prev` links in the live-allocation list.
7. Update stats: subtract `oldSize` from the tag bucket, add `newSize`.
8. **Zero growth region**: if `newSize > oldSize`, zero-initialise the newly appended bytes via `gontiPlatformZeroMemory`. This upholds the same guarantee as `__gontiMemoryAllocate` — all memory returned to the caller is always zero-initialised.

### 3.6 List Management (`listPrepend` / `listRemove`)

Both functions operate directly on `s_state->listHead` and are `static` — not part of the public API.

**`listPrepend`** — O(1) insert at head:
- Sets new node's `prev` to `NULL`, `next` to current head.
- If the list was non-empty, patches old head's `prev` to the new node.
- Advances `listHead` to the new node.

**`listRemove`** — O(1) unlink:
- If `prev` exists: patches `prev->next = hdr->next`.
- If `prev` is `NULL`: the node must be the head. If `s_state->listHead != hdr`, emits `GTERROR` and returns early — modifying a corrupt list would deepen the damage.
- If `next` exists: patches `next->prev = hdr->prev`.

### 3.7 Leak Detection (`gontiMemoryCheckLeaks`)

Called automatically by `gontiMemoryShutdown`. Walks the `listHead` linked list:
- If `listHead == NULL` → logs "All clear. No leaks detected."
- Otherwise → logs each surviving block's address, size, and tag name, sums total leaked bytes, and reports the total in KiB.

Returns `GtTrue` if leaks were detected, `GtFalse` if clean.

### 3.8 Usage Statistics (`gontiGetMemoryUsageStr`)

Iterates all `GT_MEM_TAG_MAX_TAGS` entries. For each tag:
- Calls `formatBytes()` to auto-select the most appropriate unit (B, KiB, MiB, GiB).
- Formats a row using `"%-20s %.2f %s\n"` — the tag name is left-aligned in a 20-character field, providing consistent column layout without any hardcoded padding in the source data.

The formatted result is written into a fixed-size stack buffer (`8192` bytes, defined via `#define GT_MEM_USAGE_BUF_SIZE`) then duplicated to the heap via `gontiStringDuplicate`.

**The caller is responsible for freeing the returned string via `gt_free()`.**

---

## 4. Dependency Graph

```
GtDefines.inl  ←  GtMemory.h
GtLogger.h     ←  GtMemory.h  (GTDEBUG / GTERROR / GTWARN macros)
GtMemTypes.inl ←  GtMemory.h
GtPlatform.h   ←  GtMemory.c  (platform alloc / free / zero / copy / set / realloc)
GtCStrTools.h  ←  GtMemory.c  (gontiStringDuplicate)
```

---

## 5. Design Rationale

- **Intrusive linked list** — no secondary allocation needed to track live blocks; the metadata is embedded within the same allocation, keeping overhead minimal and keeping the allocator self-contained.
- **Magic canary `0xDEADC0DE`** — provides lightweight corruption and double-free detection without the overhead of a full memory sanitizer. The value is distinctive enough to be unlikely to appear as natural data, and is clearly recognisable in a debugger's memory view.
- **Zero-on-allocate** — all allocations (including the growth region on realloc) are zero-initialised, eliminating a class of bugs from uninitialised reads at the cost of one extra `memset`-equivalent per call.
- **Tag-based statistics** — per-subsystem tracking enables targeted memory budget analysis and helps identify which system is responsible for unexpected growth.
- **Dynamic column formatting** — `gontiGetMemoryUsageStr` uses `snprintf` with `"%-20s"` instead of dot-padded string literals. This eliminates a separate `memTagDisplayStrings` array and makes the output resilient to future tag name changes.
- **Macro interface with logging** — `gt_allocate`, `gt_free`, and `gt_reallocate` emit `GTDEBUG` messages with file/line context, providing a full allocation trace in debug builds at zero runtime cost in release.
- **NOT thread-safe** — all operations on `s_state` are unsynchronised. External locking is required if the allocator is used from multiple threads concurrently. See `GtMemory.h` for the full warning.