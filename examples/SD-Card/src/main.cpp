#include <Arduino.h>

#include "generated/telemetry.h"
#include "log.h"
#include "fileSystem.h"

/** The frequency at which the main loop will be running */
static constexpr ms_t MAIN_LOOP_FREQUENCY = 1000_ms;

// Send our telemetry via the hardware serial (UART1).
static HardwareSerial& outputSerial = Serial;

/**
 * Block execution until the given time since boot has been reached.
 *
 * @param wakeTime The time since boot when to resume execution.
 */
static void delayUntil(ms_t wakeTime) {
    ms_t currentTime = ms_t(millis());
    if (currentTime > wakeTime) {
        return;
    }
    delay((wakeTime - currentTime).ms);
}

/**
 * Send a serialized communication package to send it to the base.
 *
 * @note This will be called by the generated communication code when sending any telemetry.
 * @param data The serialized communication package.
 * @param size The size of the serialized communication package in bytes.
 */
extern "C" bool writeTelemetry(void* data, size_t size) {
    bool serialOutputSucceeded = outputSerial.write((uint8_t*) data, size) == size;
    bool logWriteSucceeded = FileSystem::writeToLogFile(data, size);
    return serialOutputSucceeded && logWriteSucceeded;
}

/**
 * The standard Arduino setup function that will be called when the Arduino boots.
 * Initialize the system.
 */
void setup() {
    Logger::sendDebugMessages(true);
    outputSerial.begin(19200);
    Logger::info("Booting...");

    // Do initialization tasks
    Logger::info("Initializing SD card...");
    if (!FileSystem::ensureInitialized()) {
        Logger::error("SD-Card initialization failed. Things to check:\n"
                      "* is a card is inserted?\n"
                      "* Is your wiring correct?\n"
                      "* Is the CS pin correctly specified?");
    } else {
        Logger::info("Wiring is correct and a card is present.");
        FileSystem::logFileSystemInfo();
    }

    Logger::info("Boot complete");
}

/**
 * The standard Arduino loop function, which will be called over and over again after boot.
 * Run the system tasks.
 */
void loop() {
    // Get the time at the start of this loop iteration.
    ms_t loopStart = ms_t(millis());

    // Do main task, e.g. gather data and send telemetry.
    // For the template we just send a heartbeat telemetry package.
    if (!FileSystem::isInitialized() && FileSystem::ensureInitialized()) {
        Logger::info("SD-Card connected");
        FileSystem::logFileSystemInfo();
    }

    sendTelemetryHeartbeat(loopStart);

    // Execute the main loop at a constant rate.
    delayUntil(loopStart + MAIN_LOOP_FREQUENCY);
}
