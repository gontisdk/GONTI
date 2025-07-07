// USER FILES
#include "game.h"

#if defined(USE_VULKAN)

// ENGINE FILES
#include "../../GONTI/Source/Core/EntryPoint/Vulkan/VulkanEntry.h"
#include "../../GONTI/Source/Core/EntryPoint/Vulkan/VulkanEntryTypes.h"

// THIS FUNCTION DEFINITION MUST HAS BEEN NAMED "gontiVkEntryPoint()", BEEN TYPE "b8" AND HAS LIKE THIS LIST OF ARGUMENTS "gontiVkEntry* outEntry"
// BEACUSE THIS FUNCTION IS CONNECTET TO THE SAME FUNCTION IN GONTI ENGINE
b8 gontiVkEntryPoint(gontiVkEntry* outEntry) {
    // Window Settings (USER DATA)
    outEntry->appConfig.appName = "GONTI Engine Testbed";
    outEntry->appConfig.className = "0A7g47";
    outEntry->appConfig.startPosX = 100;
    outEntry->appConfig.startPosY = 100;
    outEntry->appConfig.startWidth = 800;
    outEntry->appConfig.startHeight = 600;

    // gontiVkEntry functions (USER IMPLEMENTATIONS)
    outEntry->update = gameUpdate;
    outEntry->render = gameRender;
    outEntry->initialize = gameInitialize;
    outEntry->onResize = gameOnResize;

    // Events
    outEntry->registerEvents = userEventsReg;
    outEntry->unregisterEvents = userEventUnreg;

    // GONTI ENGINE FUNCTION
    outEntry->state = k_allocate(sizeof(GameState), MEMORY_TAG_ENTRY);

    return true;
}

#endif
#if defined(USE_DIRECTX)
// Add here code implementations for DirectX
#endif
#if defined(USE_OPENGL)
// Add here code implementations for OpenGL
#endif