# Linux Support

The GONTI engine provides **partial Linux support**, with a strong focus on **Ubuntu-based distributions**. While cross-distribution compatibility is possible, support is tailored toward environments where the developer has full control over libraries and dependencies.

## ✅ Currently Supported

### 🐧 Distributions
- **Ubuntu 20.04 LTS and newer**
- Other **Debian-based** distributions  
  *(with manual dependency installation)*

> ⚠️ Arch, Fedora, or others *may* work — but you're on your own.

### 🖼️ Windowing Systems
The engine supports two X11-based systems for window creation and event handling:

- [X11/Xlib](https://en.wikipedia.org/wiki/Xlib)
- [XCB](https://xcb.freedesktop.org/)

You can choose either backend depending on your needs. Both are maintained and integrated via a modular abstraction layer.

### 🎮 Supported Graphics APIs

The following graphics APIs are available under Linux:

| API       | Supported | Notes                                            |
<<<<<<< HEAD
| OpenGL    | ✅        | via system libraries + GLAD/GLEW                |
| Vulkan    | ✅        | requires Vulkan SDK (`libgontiVk-dev`, etc.)     |
| DirectX   | ❌        | Not available on Linux (X11 and X12)            |
=======
|-----------|-----------|--------------------------------------------------|
| OpenGL    | ✅        | via system libraries + GLAD/GLEW                |
|-----------|-----------|--------------------------------------------------|
| Vulkan    | ✅        | requires Vulkan SDK (`libvulkan-dev`, etc.)     |
|-----------|-----------|--------------------------------------------------|
| DirectX   | ❌        | Not available on Linux (X11 and X12)            |
|-----------|-----------|--------------------------------------------------|
>>>>>>> 475c679c815ea5f049414def8adf10b6b1ed375a


## ⚠️ Limitations

- **Wayland** is currently **not supported**.
- Compatibility with non-Ubuntu distros may require additional tweaking.
- No official support for Flatpak or Snap environments.

## 🧱 Dependencies

To compile and run the engine on a supported Linux system, ensure the following packages are installed:

```bash
sudo apt install libx11-dev libxcb1-dev libx11-xcb-dev libxcb-keysyms1-dev
```

```bash
sudo apt install libgl1-mesa-dev
```

```bash
<<<<<<< HEAD
sudo apt install libgontiVk-dev gontiVk-tools
=======
sudo apt install libvulkan-dev vulkan-tools
>>>>>>> 475c679c815ea5f049414def8adf10b6b1ed375a
```