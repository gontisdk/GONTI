#include "Asserts.h"
#include "../Utilities/Logging/Scripts/Logger.h"

void reportAssertionFailure(const char* expression, const char* message, const char* file, i32 line) {
        logger_logOutput(LOG_LEVEL_FATAL, "Assertion Failure: %s, Message: '%s', In File: %s, In Line: %d\n", expression, message, file, line);
}