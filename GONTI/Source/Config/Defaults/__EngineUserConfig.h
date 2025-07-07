#ifndef __ENGINE_USER_CONFIG_H
#define __ENGINE_USER_CONFIG_H

/*
Select exactly one Graphics Environment by uncommenting one of the following:

    Vulkan      - #define USE_VULKAN
    OpenGL      - #define USE_OPENGL
    DirectX     - #define USE_DIRECTX

For OpenGL and DirectX, additional options must be selected below.

-------------------------------------------------------------------------------
OpenGL options (only if USE_OPENGL is defined):

  - OPENGL_GLAD  : Use GLAD loader (recommended for OpenGL 3.3+)
  - OPENGL_GLEW  : Use GLEW loader (legacy, OpenGL <3.3)

* Pick exactly one of these OpenGL loaders.

-------------------------------------------------------------------------------
DirectX options (only if USE_DIRECTX is defined):

  - DIRECTX_X11  : Use DirectX 11 backend
  - DIRECTX_X12  : Use DirectX 12 backend

* Pick exactly one of these DirectX versions.

-------------------------------------------------------------------------------
Examples:

// Using Vulkan:
#define USE_VULKAN

// Using OpenGL 3.3+ with GLAD:
//#define USE_OPENGL
//#define OPENGL_GLAD

// Using legacy OpenGL with GLEW:
//#define USE_OPENGL
//#define OPENGL_GLEW

// Using DirectX 12:
//#define USE_DIRECTX
//#define DIRECTX_X12

-------------------------------------------------------------------------------
Note:
- Only one graphics environment (Vulkan/OpenGL/DirectX) can be enabled at a time.
- Within OpenGL or DirectX, pick exactly one loader/version.
- Validation of these settings is done separately (see validation module).
*/

//////////////////////////////////////////
// === User Selection ===

// Uncomment one graphics environment:

#define USE_VULKAN

//#define USE_OPENGL
#ifdef USE_OPENGL

// Uncomment exactly one OpenGL loader:
#define OPENGL_GLAD
//#define OPENGL_GLEW

#endif

//#define USE_DIRECTX
#ifdef USE_DIRECTX

// Uncomment exactly one DirectX backend:
#define DIRECTX_X11
//#define DIRECTX_X12

#endif

//////////////////////////////////////////

#endif // ENGINE_USER_CONFIG_H
