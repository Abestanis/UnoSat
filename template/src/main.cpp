#include <Arduino.h>

#include "generated/telemetry.h"
#include "log.h"

/** The frequency at which the main loop will be running. */
static constexpr ms_t MAIN_LOOP_FREQUENCY = 1000_ms;

/** Send our telemetry via the hardware serial (UART1). */
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
 * Send a serialized communication package to the base.
 *
 * @note This will be called by the generated communication code when sending any telemetry.
 * @param data The serialized communication package.
 * @param size The size of the serialized communication package in bytes.
 * @return Whether there were any errors during sending.
 */
extern "C" bool writeTelemetry(void* data, size_t size) {
    return outputSerial.write((uint8_t*) data, size) == size;
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
    sendTelemetryHeartbeat(loopStart);

    // Execute the main loop at a constant rate.
    delayUntil(loopStart + MAIN_LOOP_FREQUENCY);
}
