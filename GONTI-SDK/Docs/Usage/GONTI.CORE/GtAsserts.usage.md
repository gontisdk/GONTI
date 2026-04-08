# GtAsserts — Usage Guide

> **Internal mechanics:** [`GtAsserts.how-it-works.md`](../../How-It-Works/GONTI.CORE/GtAsserts.how-it-works.md)

---

## 1. Purpose

`GtAsserts` provides **compile-time-configurable assertions** that log a fatal message and trigger a debugger breakpoint when an expression evaluates to false.

---

## 2. Installation / Import

```c
#include "Asserts/GtAsserts.h"
```

Link against `GONTI.CORE.dll` / `GONTI.CORE.so`. No additional setup is required — assertions are active by default via the `GTASSERTIONS_ENABLED` macro.

---

## 3. API Reference

### 3.1 Macros

| Macro | Active Builds | Description |
|-------|--------------|-------------|
| `GTASSERT(expr)` | Debug & Release (while `GTASSERTIONS_ENABLED`) | Asserts `expr` is true; logs fatal and breaks on failure |
| `GTASSERT_MSG(expr, message)` | Debug & Release (while `GTASSERTIONS_ENABLED`) | Same as above, with a custom failure message |
| `GTASSERT_DEBUG(expr)` | Debug only (`_DEBUG` defined) | Compiled out entirely in release builds |

### 3.2 Function

| Function | Signature | Description |
|----------|-----------|-------------|
| `gontiReportAssertionFailure` | `void (const char* expression, const char* message, const char* file, GtI32 line)` | Logs a `FATAL`-level message with assertion context. Called internally by the macros. |

---

## 4. Quick Start Examples

### Basic assertion

```c
#include "Asserts/GtAsserts.h"

void processBuffer(void* buf, GtU64 size) {
    GTASSERT(buf != 0);
    GTASSERT(size > 0);
    // ... safe to use buf here
}
```

### Assertion with a descriptive message

```c
GtB8 result = gontiFilesystemOpen(path, GT_FILE_MODE_READ, GtFalse, &handle);
GTASSERT_MSG(result == GtTrue, "Failed to open required config file");
```

### Debug-only assertion (hot path guard)

```c
// This check disappears completely in release builds
GTASSERT_DEBUG(index < arrayLength);
```

### Disabling all assertions globally

To disable all assertions for an entire build, remove or undefine `GTASSERTIONS_ENABLED` before including the header:

```c
// In a build configuration header or compiler flags:
// #undef GTASSERTIONS_ENABLED
```

When disabled, `GTASSERT`, `GTASSERT_MSG`, and `GTASSERT_DEBUG` all expand to nothing.

---

## 5. Troubleshooting

| Problem | Likely Cause | Solution |
|---------|-------------|----------|
| Assertion fires but no log appears | Logger not yet initialized | Ensure `gontiLoggerInitializeLogging()` is called before any assertion-guarded code runs |
| `__debugbreak` / `__builtin_trap` not recognized | Wrong compiler detection | Check that `_MSC_VER` or compiler intrinsic support is present; update `debugBreak()` for exotic compilers |
| `GTASSERT_DEBUG` fires in release | `_DEBUG` macro unexpectedly defined | Audit build system — `_DEBUG` should not be defined in release configurations |
| Release build still evaluates assertions | `GTASSERTIONS_ENABLED` not removed | Undefine `GTASSERTIONS_ENABLED` in the release build or via a compiler flag |