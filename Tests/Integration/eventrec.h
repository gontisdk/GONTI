#pragma once

#include "game.h"

#if defined(USE_VULKAN)

b8 applicationOnEvent(u16 code, void* sender, void* listenerInst, EventContext context);
b8 applicationOnKey(u16 code, void* sender, void* listenerInst, EventContext context);
b8 applicationOnButton(u16 code, void* sender, void* listenerInst, EventContext context);
b8 applicationOnMouseWheel(u16 code, void* sender, void* listenerInst, EventContext context);
b8 applicationOnMouseMove(u16 code, void* sender, void* listenerInst, EventContext context);

#endif
#if defined(USE_DIRECTX)
// Add here callback declarations for DirectX
#endif
#if defined(USE_OPENGL)
// Add here callback declarations for OpenGL
#endif