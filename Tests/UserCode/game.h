#pragma once

#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Logging/Logger.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/Defines.h>
#include <GONTI/GONTI-ENGINE/GONTI.RUNTIME/Source/EntryPoint/Vulkan/VulkanEntry.h>

typedef struct GameState {
    f32 deltaTime;
}GameState;

b8 gameInitialize(GontiVulkanEntry* gameInst);
b8 gameUpdate(GontiVulkanEntry* gameInst, f32 deltaTime);
b8 gameRender(GontiVulkanEntry* gameInst, f32 deltaTime);
void gameOnResize(GontiVulkanEntry* gameInst, u32 width, u32 height);
void gameShutdown(GontiVulkanEntry* gameInst);