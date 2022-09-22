#include <stdarg.h>
#include <stdio.h>
#include <Arduino.h>

#include "generated/telemetry.h"
#include "log.h"


/** Whether debug messages should be send. */
static bool enableDebugMessages = false;


/**
 * Send a message without formatting as telemetry.
 *
 * @param level The message severity.
 * @param length The size of the message in bytes not including the terminating null character.
 * @param message The message.
 */
static void sendMessage(LogLevel level, uint16_t length, const char* message) {
    sendTelemetryLog(ms_t(millis()), level, length, message);
}

/**
 * Send a formatted message as telemetry.
 *
 * @param level The message severity.
 * @param format The message format.
 * @param args Arguments for creating the message.
 */
static void sendFormattedMessage(LogLevel level, const char* format, va_list args) {
    // This is optimized for memory efficiency, at the cost of speed:
    // We have to call the formatter function twice, but we only exactly use as much memory as necessary.
    va_list argsCopy;
    va_copy(argsCopy, args);
    size_t length = vsnprintf(nullptr, 0, format, argsCopy);
    if (length > UINT16_MAX) {
        length = UINT16_MAX;
    }
    char buffer[length + 1];
    vsnprintf(buffer, length + 1, format, args);
    sendMessage(level, length, buffer);
}

void Logger::info(const char* message) {
    sendMessage(LOG_INFO, strlen(message), message);
}

void Logger::infoFormatted(const char* format, ...) {
    va_list args;
    va_start(args, format);
    sendFormattedMessage(LOG_INFO, format, args);
    va_end(args);
}

void Logger::debug(const char* message) {
    sendMessage(LOG_DEBUG, strlen(message), message);
}

void Logger::debugFormatted(const char* format, ...) {
    if (!enableDebugMessages) {
        return;
    }
    va_list args;
    va_start(args, format);
    sendFormattedMessage(LOG_DEBUG, format, args);
    va_end(args);
}

void Logger::error(const char* message) {
    sendMessage(LOG_ERROR, strlen(message), message);
}

void Logger::errorFormatted(const char* format, ...) {
    va_list args;
    va_start(args, format);
    sendFormattedMessage(LOG_ERROR, format, args);
    va_end(args);
}

void Logger::sendDebugMessages(bool enabled) {
    enableDebugMessages = enabled;
}
