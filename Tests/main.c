#include <stdio.h>
#include <GONTI/GONTI-ENGINE/GONTI.RENDER/Source/Platform/Vulkan/VulkanPlatform.h>
#include <GONTI/GONTI-ENGINE/GONTI.RUNTIME/Source/Application/Vulkan/VulkanApplication.h>
#include "UserCode/events.h"
#include "UserCode/game.h"
#include "UserSetupEngine/entry.h"

int main(void) {
    gontiInitializeMemory();

    GontiVulkanEntry entry;
    gontiVkEntry(&entry);
    gontiVkApplicationCreate(&entry);

    gontiVkApplicationRun(30, true);

    gontiShutdownMemory();

    return 0;
}