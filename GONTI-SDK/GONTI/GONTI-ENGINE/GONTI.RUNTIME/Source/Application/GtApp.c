#include "GtApp.h"

#include <GONTI/GONTI-ENGINE/GONTI.RENDER/Source/Config/SetupGraphicBackend/GtUsrSetGraphBackend.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Memory/GtMemory.h>

#if defined(GONTI_USE_VK)
    #include "Vulkan/GtVkApp.h"
#elif defined(GONTI_USE_GL)
#elif defined(GONTI_USE_DX)
#endif

/* 
* B8 
*/
GtB8 gontiApplicationCreate(GtEntry* entry) {
    #if defined(GONTI_USE_VK)
        return gontiVkApplicationCreate(entry);
    #endif

    return GtFalse;
}
GtB8 gontiApplicationRun(GtU64 fpsLimit, GtB8 isFpsLimit) {
    #if defined(GONTI_USE_VK)
        return gontiVkApplicationRun(fpsLimit, isFpsLimit);
    #endif

    return GtFalse;
}

/* 
* VOID 
*/
void gontiApplicationShutdown() {
    #if defined(GONTI_USE_VK)
        gontiVkApplicationShutdown();
    #endif
}
void gontiApplicationPause() {
    #if defined(GONTI_USE_VK)
        gontiVkApplicationPause();
    #endif
}
void gontiApplicationResume() {
    #if defined(GONTI_USE_VK)
        gontiVkApplicationResume();
    #endif
}
void gontiApplicationSuspend() {
    #if defined(GONTI_USE_VK)
        gontiVkApplicationSuspend();
    #endif
}
void gontiApplicationUnSuspend() {
    #if defined(GONTI_USE_VK)
        gontiVkApplicationUnSuspend();
    #endif
}
void gontiApplicationSetWidth(GtU32 width) {
#if defined(GONTI_USE_VK)
        gontiVkApplicationSetWidth(width);
    #endif
}
void gontiApplicationSetHeight(GtU32 height) {
    #if defined(GONTI_USE_VK)
        gontiVkApplicationSetHeight(height);
    #endif
}
void gontiApplicationOnResized(GtU32 width, GtU32 height) {
    #if defined(GONTI_USE_VK)
        gontiVkApplicationOnResized(width, height);
    #endif
}

/* 
* GtI16 
*/
GtI16 gontiApplicationGetWidth() {
    #if defined(GONTI_USE_VK)
        return gontiVkApplicationGetWidth();
    #endif

    return 0;
}
GtI16 gontiApplicationGetHeight() {
    #if defined(GONTI_USE_VK)
        return gontiVkApplicationGetHeight();
    #endif

    return 0;
}

/* BOOL/B8 */
GtB8 gontiApplicationIsSuspended() {
    #if defined(GONTI_USE_VK)
        return gontiVkApplicationIsSuspended();
    #endif

    return GtFalse;
}
GtB8 gontiApplicationIsPaused() {
    #if defined(GONTI_USE_VK)
        return gontiVkApplicationIsPaused();
    #endif

    return GtFalse;
}