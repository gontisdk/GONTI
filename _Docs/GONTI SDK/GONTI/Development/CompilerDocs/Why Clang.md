# 🤔 Why LLVM Clang?

The GONTI engine is written in **pure C**, but built and maintained using the **LLVM Clang** compiler and toolchain.

This is not just a convenience — it's a deliberate decision driven by technical advantages.

## 🎯 Why Clang?

- **Cross-Platform compatibility**  
  Clang runs consistently across Linux, Windows, and (theoretically) macOS.

- **Modern diagnostics and tooling**  
  Despite targeting C, we use Clang's C++-oriented toolchain for static analysis and formatting.

- **Readable error messages**  
  Clang’s diagnostics are clear, detailed, and easier to understand than GCC or MSVC.

- **LLVM ecosystem integration**  
  Tools like `clang-format`, `clang-tidy`, and `llvm-symbolizer` work out of the box.

- **IR and optimization experiments**  
  Clang makes it easy to generate and inspect LLVM Intermediate Representation (IR), useful for performance debugging.

## ⚖️ Comparison with Other Compilers

| Compiler  | Status             | Notes                                                           |
| **Clang** | ✅ Preferred       | Officially tested, recommended, toolchain-integrated           |
| **GCC**   | ⚠️ Partially works | Can compile the code, but not officially tested or recommended |
| **MSVC**  | ❌ Unsupported     | Incompatible with parts of the code, not used or tested        |

## 📌 Summary

Clang is:
- modern,
- portable,
- precise,
- analysis-friendly.

It offers the best environment for building and maintaining a **robust C codebase**, especially when backed by powerful tooling.
