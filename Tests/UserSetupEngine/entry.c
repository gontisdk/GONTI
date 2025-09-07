#include "entry.h"
#include "../UserCode/game.h"

b8 gontiVkEntry(GontiVulkanEntry* outEntry) {
    outEntry->windowConfig.windowName = "AAA";
    outEntry->windowConfig.className = "BBB";
    outEntry->windowConfig.startPosX = 100;
    outEntry->windowConfig.startPosY = 100;
    outEntry->windowConfig.startWidth = 800;
    outEntry->windowConfig.startHeight = 600;

    // GontiVulkanEntry functions (USER IMPLEMENTATIONS)
    outEntry->update = gameUpdate;
    outEntry->render = gameRender;
    outEntry->initialize = gameInitialize;
    outEntry->onResize = gameOnResize;
    outEntry->shutdown = gameShutdown;

    // GONTI ENGINE FUNCTION
    outEntry->state = k_allocate(sizeof(GameState), GONTI_MEMORY_TAG_ENTRY);

    return true;
}