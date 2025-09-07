# 🧩 Compiler Compatibility

GONTI is written in **C**, and is built using **LLVM Clang** as the official and primary compiler.

## ✅ Officially Supported

### LLVM Clang

- **Minimum version:** `Clang 12`
- **Tested versions:** 12, 13, 14, 15, 16
- **Language standard:** `C17`
<<<<<<< HEAD
- **Supported Platforms:** Linux and Windows (via MinGW or clang-cl)
=======
- **Supported platforms:** Linux and Windows (via MinGW or clang-cl)
>>>>>>> 475c679c815ea5f049414def8adf10b6b1ed375a

> Note: Clang is used in **C mode**, but we benefit from its full toolchain — originally designed for C++ — for analysis and formatting.

---

## ⚠️ Partially Compatible / Untested

### GCC

- Generally compiles the code using `-std=c17`,
- Not tested or officially supported,
- May produce different warnings or behave differently with extensions/macros.

---

## ❌ Unsupported

### MSVC (Microsoft Visual C)

- Not compatible with some core parts of the codebase,
- Not tested, not supported,
- Lacks compatibility with Clang-based diagnostics and tooling.

---

## 🔁 Language Standard

- **Target language:** C  
- **Minimum required standard:** `C17`  
- No C++ code is used — only Clang's **tooling** originates from the C++ ecosystem.

> 💡 The codebase is pure C, but modern.  
> That means: no `<iostream>`, but yes to readable build errors and modern static analysis.
