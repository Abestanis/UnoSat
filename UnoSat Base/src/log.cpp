#include <stdarg.h>
#include <stdio.h>
#include <Arduino.h>
#include "generated/telemetry.h"
#include "log.h"


bool Logger::enableDebugMessages = false;

/**
 * Send a message as telemetry.
 *
 * @param level The message severity.
 * @param format The message format.
 * @param args Arguments for creating the message.
 */
static void sendMessage(LogLevel level, const char* format, va_list args) {
    size_t length = vsnprintf(nullptr, 0, format, args) + 1;
    char buffer[length + 1];
    vsnprintf(buffer, length, format, args);
    sendTelemetryLog(ms_t(millis()), level, length, buffer);
}

void Logger::info(const char* format, ...) {
    va_list args;
    va_start(args, format);
    sendMessage(LOG_INFO, format, args);
    va_end(args);
}

void Logger::debug(const char* format, ...) {
    if (!Logger::enableDebugMessages) {
        return;
    }
    va_list args;
    va_start(args, format);
    sendMessage(LOG_DEBUG, format, args);
    va_end(args);
}

void Logger::error(const char* format, ...) {
    va_list args;
    va_start(args, format);
    sendMessage(LOG_ERROR, format, args);
    va_end(args);
}
