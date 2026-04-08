# GtAsserts — How It Works

> **Corresponding Usage file:** [GtAsserts.usage.md`](../../Usage/GONTI.CORE/GtAsserts.usage.md)

---

## 1. Overview

`GtAsserts` provides a lightweight, cross-platform **assertion framework** for `GONTI.CORE`. It consists of a single reporting function and three preprocessor macros that wrap it. All assertions are governed by the `GTASSERTIONS_ENABLED` compile-time switch.

---

## 2. Architecture Overview

```
GtAsserts.h
├── GTASSERTIONS_ENABLED guard
├── debugBreak() — platform-specific debug trap
├── gontiReportAssertionFailure() — declaration (GTAPI)
├── GTASSERT(expr)        — always-on assertion
├── GTASSERT_MSG(expr, message) — always-on assertion with message
└── GTASSERT_DEBUG(expr)  — debug-build-only assertion

GtAsserts.c
└── gontiReportAssertionFailure() — implementation
    └── calls gontiLoggerLogOutput(GT_LOG_LEVEL_FATAL, ...)
```

---

## 3. Internal Logic

### 3.1 Compile-time Enable/Disable

The macro `GTASSERTIONS_ENABLED` is defined unconditionally in the current codebase. When it is **not** defined, all three assertion macros expand to nothing, producing zero overhead in production builds.

### 3.2 Platform Debug Break

`debugBreak()` is mapped to:

| Compiler / Platform | Expansion |
|---------------------|-----------|
| MSVC (`_MSC_VER`) | `__debugbreak()` (via `<intrin.h>`) |
| GCC / Clang | `__builtin_trap()` |

Both intrinsics cause an immediate hardware breakpoint / trap signal, which halts execution in an attached debugger at the exact call site.

### 3.3 Assertion Macros

Each macro follows the same pattern:

1. Evaluate `expr`.
2. If **true** — do nothing (branch optimised away by the compiler).
3. If **false** — call `gontiReportAssertionFailure`, then invoke `debugBreak()`.

The `if (expr) {} else { ... }` idiom (rather than `if (!expr)`) avoids an extra logical negation and is idiomatic in engine assertion systems.

| Macro | Active When | Message Support |
|-------|-------------|----------------|
| `GTASSERT(expr)` | `GTASSERTIONS_ENABLED` | No (empty string passed) |
| `GTASSERT_MSG(expr, message)` | `GTASSERTIONS_ENABLED` | Yes |
| `GTASSERT_DEBUG(expr)` | `GTASSERTIONS_ENABLED` **and** `_DEBUG` | No |

`GTASSERT_DEBUG` is compiled out entirely in release builds (`_DEBUG` undefined), making it safe to use in hot paths that must not incur any check overhead in production.

### 3.4 gontiReportAssertionFailure

```c
void gontiReportAssertionFailure(
    const char* expression,
    const char* message,
    const char* file,
    GtI32 line);
```

The function delegates entirely to `gontiLoggerLogOutput` at the `GT_LOG_LEVEL_FATAL` level, formatting:

```
Assertion Failure: <expression>, Message: '<message>', In File: <file>, In Line: <line>
```

It does **not** call `abort()`, `exit()`, or any OS termination routine — the actual program halt is caused by the `debugBreak()` call in the invoking macro, which gives the debugger an opportunity to intercept before process teardown.

---

## 4. Dependency Graph

```
GtDefines.h  ←  GtAsserts.h
GtLogger.h   ←  GtAsserts.c
```

- `GtAsserts.h` depends only on `GtDefines.h` (for `GtI32` and `GTAPI`).
- `GtAsserts.c` depends on `GtLogger.h` (for `gontiLoggerLogOutput`).

---

## 5. Design Rationale

- **Macro-based assertions** ensure zero overhead when disabled, and preserve `__FILE__` / `__LINE__` at the call site without requiring non-standard compiler extensions.
- **Fatal log before trap** ensures the failure is recorded in the log output before the process is interrupted, which is essential when running without an interactive debugger (e.g., in CI or headless test environments).
- **Separation of reporting and trapping** — `gontiReportAssertionFailure` is a regular exported function (not a macro), which means it can be linked into test harnesses and its behaviour can be observed or overridden.