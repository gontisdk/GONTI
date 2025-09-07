#pragma once

#include "../../GONTI/Source/__GONTI_INCLUDES.h"
#include "../../GONTI/Source/Core/MemorySubsystem/MemorySubsystem.h"
#include "../../GONTI/Source/Events/Events.h"

typedef struct GameState {
    f32 deltaTime;
}GameState;


#if defined(USE_VULKAN)

#include "../../GONTI/Source/Core/EntryPoint/Vulkan/VulkanEntry.h"
#include "../../GONTI/Source/Core/EntryPoint/Vulkan/VulkanEntryTypes.h"
#include "../../GONTI/Source/Environments/Vulkan/Application/VulkanApplication.h"
#include "../../GONTI/Source/Events/InputSystem/Vulkan/VulkanInputs.h"

b8 gameInitialize(gontiVkEntry* gameInst);
b8 gameUpdate(gontiVkEntry* gameInst, f32 deltaTime);
b8 gameRender(gontiVkEntry* gameInst, f32 deltaTime);

void gameOnResize(gontiVkEntry* gameInst, u32 width, u32 height);
void userEventsReg();
void userEventUnreg();

int initEngine(gontiVkEntry gameInst);

#endif
#if defined(USE_DIRECTX)
// Add here code declarations for DirectX
#endif
#if defined(USE_OPENGL)
// Add here code declarations for OpenGL
#endif