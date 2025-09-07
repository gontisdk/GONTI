#include "Clock.h"

#ifdef USE_VULKAN
#ifdef DeltaTime_ON

#include "../../../Core/Platform/Platform.h"
#include "../../../Core/Platform/Vulkan/VulkanPlatform.h"

void clockUpdate(Clock* clock) {
    if (clock->startTime != 0) {
        clock->elapsed = platformGetAbsoluteTime() - clock->startTime;
    }
}
void clockStart(Clock* clock) {
    clock->startTime = platformGetAbsoluteTime();
    clock->elapsed = 0;
}
void clockStop(Clock* clock) {
    clock->startTime = 0;
}

#endif
#endif