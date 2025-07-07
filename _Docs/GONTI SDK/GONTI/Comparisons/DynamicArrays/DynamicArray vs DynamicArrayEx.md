# DynamicArray vs DynamicArrayEx

The engine provides two general-purpose dynamic array containers, each tailored for different use cases and ownership models. While they may appear similar at first glance, they are structurally and semantically different. Developers can choose whichever best fits their data management strategy — or even use both in combination where appropriate.

| Feature                    | `DynamicArray`                                                         | `DynamicArrayEx`                                                           |
|----------------------------|------------------------------------------------------------------------|----------------------------------------------------------------------------|
| **Element storage**        | Stores elements directly (raw memory copy)                             | Stores `void*` pointers to user-managed objects                            |
|----------------------------|------------------------------------------------------------------------|----------------------------------------------------------------------------|
| **Element ownership**      | Owns and manages memory for stored data                                | Only stores pointers; user is responsible for memory management            |
|----------------------------|------------------------------------------------------------------------|----------------------------------------------------------------------------|
| **Data access**            | Fast access to raw elements                                            | Requires dereferencing pointers                                            |
|----------------------------|------------------------------------------------------------------------|----------------------------------------------------------------------------|
| **Flexibility**            | Less flexible — requires known type size on allocation                 | Very flexible — supports any data type via pointers                        |
|----------------------------|------------------------------------------------------------------------|----------------------------------------------------------------------------|
| **Resizing behavior**      | Resizes internal buffer and copies elements                            | Resizes pointer array; stored data remains untouched                       |
|----------------------------|------------------------------------------------------------------------|----------------------------------------------------------------------------|
| **Clearing contents**      | Frees internal buffer, data is destroyed                               | Clears pointer array only; pointed data must be freed manually             |
|----------------------------|------------------------------------------------------------------------|----------------------------------------------------------------------------|
| **Memory overhead**        | Slightly lower — stores data inline                                    | Slightly higher — stores pointers only                                     |
|----------------------------|------------------------------------------------------------------------|----------------------------------------------------------------------------|
| **Use case**               | Suitable for simple POD types or structs without external dependencies | Ideal for objects requiring custom allocation, polymorphism, or resource   |
|                            |                                                                        | control                                                                    |
|----------------------------|------------------------------------------------------------------------|----------------------------------------------------------------------------|
| **Performance (copy ops)** | Higher cost on resize (copies full objects)                            | Lower cost on resize (copies pointers only)                                |
|----------------------------|------------------------------------------------------------------------|----------------------------------------------------------------------------|
| **Type safety**            | Type-safe through macros or internal API                               | Type-unsafe (void pointers), requires manual casting                       |
|----------------------------|------------------------------------------------------------------------|----------------------------------------------------------------------------|
| **Inspiration for name**   | Generic dynamic array                                                  | Named after Windows API convention (`Ex` for extended version)             | 
|----------------------------|------------------------------------------------------------------------|----------------------------------------------------------------------------|

## Recommendations

- Use `DynamicArray` when working with small, value-like structures where performance of raw memory copies is acceptable.
- Use `DynamicArrayEx` when managing polymorphic or heap-allocated objects, or when pointer-level control is required.
- Both types are maintained in parallel and intended to complement each other. Proper usage depends on context and ownership expectations.