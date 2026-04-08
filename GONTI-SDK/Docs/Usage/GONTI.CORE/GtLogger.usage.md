# GtLogger — Usage Guide

> **Internal mechanics:** [`GtLogger.how-it-works.md`](../../How-It-Works/GONTI.CORE/GtLogger.how-it-works.md)

---

## 1. Purpose

`GtLogger` provides **six-level, colour-coded, compile-time-filterable console logging** for `GONTI.CORE`. Consumer code uses lightweight macros that expand to zero instructions when a level is disabled.

---

## 2. Installation / Import

```c
#include "Logging/GtLogger.h"
```

Link against `GONTI.CORE.dll` / `GONTI.CORE.so`. The logger must be initialised before use.

---

## 3. Initialisation & Shutdown

`GtLogger` follows the two-phase initialisation pattern used across `GONTI.CORE`.

```c
// Phase 1: query memory requirement
GtU64 memReq = 0;
gontiLoggerInitializeLogging(&memReq, 0);

// Phase 2: allocate and initialise
void* loggerState = gt_allocate(memReq, GT_MEM_TAG_APPLICATION);
gontiLoggerInitializeLogging(&memReq, loggerState);

// ... application runs ...

// Shutdown
gontiLoggerShutdown(loggerState);
```

---

## 4. API Reference

### 4.1 Log Level Macros (preferred interface)

| Macro | Level | Default | Release Build |
|-------|-------|---------|--------------|
| `GTFATAL(msg, ...)` | Fatal | Always on | Always on |
| `GTERROR(msg, ...)` | Error | Always on | Always on |
| `GTWARN(msg, ...)` | Warning | On | On |
| `GTINFO(msg, ...)` | Info | On | On |
| `GTDEBUG(msg, ...)` | Debug | On | **Off** |
| `GTTRACE(msg, ...)` | Trace | On | **Off** |

All macros accept `printf`-style format strings and variadic arguments.

### 4.2 GtLogLvl Enum

```c
typedef enum GtLogLvl {
    GT_LOG_LEVEL_FATAL = 0,
    GT_LOG_LEVEL_ERROR = 1,
    GT_LOG_LEVEL_WARN  = 2,
    GT_LOG_LEVEL_INFO  = 3,
    GT_LOG_LEVEL_DEBUG = 4,
    GT_LOG_LEVEL_TRACE = 5
} GtLogLvl;
```

### 4.3 Functions

| Function | Signature | Description |
|----------|-----------|-------------|
| `gontiLoggerInitializeLogging` | `GtB8 (GtU64* memoryRequirement, void* state)` | Phase-1/2 initialisation |
| `gontiLoggerShutdown` | `void (void* state)` | Tears down the logger |
| `gontiLoggerLogOutput` | `void (GtLogLvl level, const char* message, ...)` | Low-level log function; prefer macros |

---

## 5. Quick Start Examples

### Basic logging

```c
#include "Logging/GtLogger.h"

GTINFO("Engine started. Version: %d.%d", major, minor);
GTWARN("Texture '%s' not found, using fallback.", textureName);
GTERROR("Failed to open socket on port %d.", port);
GTFATAL("Out of memory — cannot continue.");
```

### Debug-only information

```c
// Compiled out automatically in release builds
GTDEBUG("Frame time: %.4f ms", deltaTime * 1000.0f);
GTTRACE("Entering function: %s", __func__);
```

### Disabling a specific level at compile time

```c
// In a build config header, before including GtLogger.h:
#define GT_LOG_WARN_ENABLED 0  // suppress all warnings
#include "Logging/GtLogger.h"
```

---

## 6. Troubleshooting

| Problem | Likely Cause | Solution |
|---------|-------------|----------|
| No output visible | Logger not initialised | Call `gontiLoggerInitializeLogging` before any log macro |
| `GTDEBUG` / `GTTRACE` produce no output | Running a release build | Expected behaviour — rebuild in debug mode |
| Log messages truncated | Message exceeds 32 000 bytes | Split the message into multiple log calls |
| Colours not displayed on Windows terminal | Legacy console mode | Enable virtual terminal processing or use Windows Terminal |