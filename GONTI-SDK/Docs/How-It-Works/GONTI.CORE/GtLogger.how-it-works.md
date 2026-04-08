# GtLogger — How It Works

> **Corresponding Usage file:** [`GtLogger.usage.md`](../../Usage/GONTI.CORE/GtLogger.usage.md)

---

## 1. Overview

`GtLogger` is the **centralised logging subsystem** of `GONTI.CORE`. It provides six log severity levels, compile-time level filtering, and platform-abstracted coloured console output. Its state is minimal — a single `GtB8 initialized` flag — because all formatting and output occurs synchronously within `gontiLoggerLogOutput`.

---

## 2. Architecture Overview

```
GtLogger.h
├── Level enable guards (GT_LOG_WARN/INFO/DEBUG/TRACE_ENABLED)
├── Release-mode suppression (DEBUG and TRACE disabled)
├── Macro wrappers (GTFATAL, GTERROR, GTWARN, GTINFO, GTDEBUG, GTTRACE)
├── GtLogLvl enum
└── API declarations

GtLogger.c
├── GtLogSysState { GtB8 initialized }   ← file-scope singleton
├── gontiLoggerInitializeLogging()
├── gontiLoggerShutdown()
└── gontiLoggerLogOutput()
    ├── levelStrings[6] — prefix table
    ├── vsnprintf — variadic formatting into outMessage[32000]
    ├── sprintf   — prepend level prefix into outMessage2[32000]
    └── gontiPlatformConsoleWrite / gontiPlatformConsoleWriteError
```

---

## 3. Internal Logic

### 3.1 Log Level Filtering

Level visibility is controlled by four compile-time flags in `GtLogger.h`:

| Flag | Default | Release override |
|------|---------|-----------------|
| `GT_LOG_WARN_ENABLED` | `1` | kept `1` |
| `GT_LOG_INFO_ENABLED` | `1` | kept `1` |
| `GT_LOG_DEBUG_ENABLED` | `1` | forced to `0` |
| `GT_LOG_TRACE_ENABLED` | `1` | forced to `0` |

`FATAL` and `ERROR` are unconditional — their macros always call `gontiLoggerLogOutput` regardless of any flag.

The conditional macros expand to either a `gontiLoggerLogOutput` call or an empty statement, producing no code in the disabled case.

### 3.2 GtLogSysState Singleton

```c
typedef struct GtLogSysState {
    GtB8 initialized;
} GtLogSysState;
static GtLogSysState* statePtr;
```

State is allocated externally (two-phase init pattern shared with `GtMemory`): the caller first queries the required size, then provides a pre-allocated buffer via `gontiLoggerInitializeLogging`. This allows the engine to manage subsystem memory centrally.

### 3.3 gontiLoggerLogOutput

The function executes three steps:

1. **Variadic formatting** — `vsnprintf` writes the caller's `message` + arguments into a 32 000-byte stack buffer `outMessage`.
2. **Prefix injection** — `sprintf` prepends the level string (e.g., `"[ERROR]: "`) into a second buffer `outMessage2`, then appends `\n`.
3. **Platform output** — levels `< GT_LOG_LEVEL_WARN` (i.e., FATAL and ERROR) are routed to `gontiPlatformConsoleWriteError`; all others go to `gontiPlatformConsoleWrite`. The `level` value is passed directly as the `colour` parameter, exploiting the fact that `GtLogLvl` ordinals match the platform colour table indices.

### 3.4 Error Classification

```c
GtB8 isError = level < GT_LOG_LEVEL_WARN;
```

Only `GT_LOG_LEVEL_FATAL (0)` and `GT_LOG_LEVEL_ERROR (1)` are classified as errors. This single boolean determines the output channel (stdout vs. stderr).

---

## 4. Dependency Graph

```
GtDefines.h  ←  GtLogger.h
GtPlatform.h ←  GtLogger.c   (console write)
GtMemory.h   ←  GtLogger.c   (gt_setMemory for buffer zeroing)
```

> ⚠️ `GtMemory.h` includes `GtLogger.h` for its own diagnostic macros. This creates a mutual dependency: `GtLogger.c` uses `GtMemory` (`gt_setMemory`), and `GtMemory.h` uses `GtLogger` macros. This is resolved by inclusion order — `GtLogger.h` must be initialised before the memory system uses its macros.

---

## 5. Design Rationale

- **Stack buffers (32 000 bytes)** — avoids any heap allocation in the log path, making logging safe to call even before or during memory system initialisation/shutdown.
- **Two-phase init pattern** — consistent with other `GONTI.CORE` subsystems; allows the application layer to control memory placement of all subsystem states.
- **Level ordinal = colour index** — eliminates a conversion table between `GtLogLvl` and platform colour codes, keeping the platform layer's colour API intentionally aligned.
- **TODO markers** — log file output and log entry queuing are noted as future work in both `gontiLoggerInitializeLogging` and `gontiLoggerShutdown`.