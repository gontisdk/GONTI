#include "GtAsserts.h"
#include "../Logging/GtLogger.h"

void gontiReportAssertionFailure(const char* expression, const char* message, const char* file, GtI32 line) {
        gontiLoggerLogOutput(GT_LOG_LEVEL_FATAL, "Assertion Failure: %s, Message: '%s', In File: %s, In Line: %d\n", expression, message, file, line);
}