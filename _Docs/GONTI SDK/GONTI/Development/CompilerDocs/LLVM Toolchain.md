# ⚙️ LLVM Toolchain: Tools and Workflow

While GONTI is written in **C**, we rely on the broader LLVM ecosystem to improve code quality, diagnostics, and consistency.

This page lists the key tools used during development.

## 🛠️ Tools in Use

### `clang`

- The official compiler for all builds.
- Used in C mode (`-std=c17`).
- Works on Linux and Windows.

### `clang-format`

- Automatic code formatter.
- Applied to `.c`, `.h`, `.inl`, `.hpp` files.
- Ensures consistent style across the project.

```bash
clang-format -i src/*.c include/*.h
```

### `clang-tidy`

- Static analyzer.

- Warns about unsafe patterns, undefined behavior, bad practices, etc.

- Helps catch subtle bugs early, especially in pointer-heavy code.

## `llvm-symbolizer`

- Translates crash addresses into human-readable stack traces.

- Can be integrated with GONTI's logging system.

## 🔍 Intermediate Representation (IR)

### Clang can emit LLVM IR, useful for:

- Debugging generated code,

- Verifying optimizations,

- Studying compiler behavior under the hood.

```bash
clang -S -emit-llvm -std=c17 -o out.ll src/core.c
```

## 📌 Summary

LLVM is more than just a compiler — it's a full development suite.
We use Clang to compile pure C code, but also to analyze, format, and optimize it — all without leaving the LLVM ecosystem.

---