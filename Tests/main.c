<<<<<<< HEAD
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
=======
// Source/
#include "../GONTI/Source/__GONTI_INCLUDES.h"
#include "../GONTI/Source/Utilities/Logging/Scripts/Logger.h"
#include "../GONTI/Source/Asserts/Asserts.h"
#include "../GONTI/Source/Environments/Vulkan/Application/VulkanApplication.h"
#include "../GONTI/Source/Core/EntryPoint/Vulkan/VulkanEntry.h"
#include "../GONTI/Source/Core/EntryPoint/Vulkan/VulkanEntryTypes.h"
#include "../GONTI/Source/Math/Maths.h"
#include "../GONTI/Source/Math/Vectors/Vectors.h"
#include "../GONTI/Source/Math/Matrices/Matrices.h"
#include "../GONTI/Source/Math/Algebra/BigInteger/BigInteger.h"
#include "../GONTI/Source/Containers/DynamicArrayEx/DynamicArrayEx.h"

// Tests/
#include "Integration/game.h"

int main(void) {
    
    //

    initializeMemory();

    //===============================================================================================



    //===============================================================================================

    gontiVkEntry gameInst;
    initEngine(gameInst);

    shutdownMemory();

    return 0; 
>>>>>>> 475c679c815ea5f049414def8adf10b6b1ed375a
}