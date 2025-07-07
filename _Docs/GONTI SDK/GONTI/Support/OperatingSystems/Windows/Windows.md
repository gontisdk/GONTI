# Windows Support

> Good news: Windows has been supported since... basically forever.  
> Bad news: It still is.

The GONTI engine offers **full native support for Microsoft Windows**, covering a wide range of versions and system configurations.

## ✅ Supported Versions

- 🪟 Windows 7  
- 🪟 Windows 8.1  
- 🪟 Windows 10  
- 🪟 Windows 11  
- 🧟 Even older systems, assuming they still boot and don't catch fire

## 🎮 Supported Graphics APIs

The engine supports multiple graphics backends under Windows:

| API        | Supported | Notes                                              |
|------------|-----------|-----------------------------------------------------|
| OpenGL     | ✅        | via **GLEW** or **GLAD**                           |
|------------|-----------|-----------------------------------------------------|
| DirectX 11 | ✅        | for legacy and modern Windows/Xbox compatibility   |
|------------|-----------|-----------------------------------------------------|
| DirectX 12 | ✅        | because performance matters                        |
|------------|-----------|-----------------------------------------------------|
| Vulkan     | ✅        | for cutting-edge multi-platform rendering          |
|------------|-----------|-----------------------------------------------------|
| GDI        | ✅        | fallback for basic operations and debugging        |
|------------|-----------|-----------------------------------------------------|

Whether you’re rendering particles or resurrecting Win7, we’ve got you covered.

## 📦 Build Requirements

To build the engine on Windows, you'll need:

- Visual Studio 2019 or newer  
- Windows SDK  
- Modern GPU drivers  
- At least one cup of patience

## 📝 Notes

This is Windows.  
If something doesn’t work, try:

- Turning it off and on again  
- Running as Administrator™️  
- Sacrificing RAM to Task Manager

Otherwise, you’re probably good to go.

---

*This file exists to reassure you that Windows support is as solid as the Recycle Bin is deep.*
