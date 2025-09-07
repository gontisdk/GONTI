#include "VulkanClock.h"

#if GONTI_USE_VULKAN

#include <GONTI/GONTI-ENGINE/GONTI.RENDER/Source/Platform/Vulkan/VulkanPlatform.h>

void gontiVkClockUpdate(Clock* clock) {
    if (clock->startTime != 0) {
        clock->elapsed = gontiVkPlatformGetAbsoluteTime() - clock->startTime;
    }
}
void gontiVkClockStart(Clock* clock) {
    clock->startTime = gontiVkPlatformGetAbsoluteTime();
    clock->elapsed = 0;
}
void gontiVkClockStop(Clock* clock) {
    clock->startTime = 0;
}

#endif