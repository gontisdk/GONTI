#include <Clock/GtClock.h>

#include <GONTI-CORE/GONTI.CORE/Source/Platform/GtPlatform.h>

void gontiClockUpdate(GtClock* clock) {
    if (clock->startTime != 0) {
        clock->elapsed = gontiPlatformGetAbsoluteTime() - clock->startTime;
    }
}
void gontiClockStart(GtClock* clock) {
    clock->startTime = gontiPlatformGetAbsoluteTime();
    clock->elapsed = 0;
}
void gontiClockStop(GtClock* clock) {
    clock->startTime = 0;
}
