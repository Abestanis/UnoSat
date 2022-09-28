#include <Arduino.h>
#include <SoftwareSerial.h>

#include "generated/telemetry.h"

namespace Rtk {

#include "generated/rtk/sharedTypes.h"

}

#include "log.h"
#include "RtkGps.h"

/** The frequency at which the main loop will be running. */
static constexpr ms_t MAIN_LOOP_FREQUENCY = 800_ms;
/** The frequency at which telemetry should be send. */
static constexpr ms_t TELEMETRY_SEND_FREQUENCY = ms_t(MAIN_LOOP_FREQUENCY.ms * 2 - 10);

/** Send our telemetry via the hardware serial (UART1). */
static HardwareSerial& outputSerial = Serial;

static SoftwareSerial rtkSerial(7, 8);
static RtkGps<(sizeof(TelemetryMessageHeader) + 64) * 2> rtkGps(rtkSerial);

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
    return outputSerial.write((uint8_t*) data, size);
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
    rtkSerial.begin(COMMUNICATION_BAUD_RATE);

    Logger::info("Boot complete");
}

/**
 * The standard Arduino loop function, which will be called over and over again after boot.
 * Run the system tasks.
 */
void loop() {
    // Get the time at the start of this loop iteration.
    ms_t loopStart = ms_t(millis());

    // Gather and send location updates.
    rtkGps.handleMessages();
    static ms_t lastTelemetrySend = 0_ms;
    if (rtkGps.getLastGpsTimestamp() != 0 && ms_t(millis()) - lastTelemetrySend >= TELEMETRY_SEND_FREQUENCY) {
        deg_t latitude = rtkGps.getLastLatitude();
        deg_t longitude = rtkGps.getLastLongitude();
        mm_t altitude = rtkGps.getLastAltitude();
        uint8_t visibleSatellites = rtkGps.getLastVisibleSatellites();
        if (!sendTelemetryLocation(ms_t(millis()), latitude, longitude, altitude, visibleSatellites)) {
            Logger::error("Failed to send telemetry data");
        }
        lastTelemetrySend = ms_t(millis());
    } else { // If we don't send a location, send a heartbeat instead.
        sendTelemetryHeartbeat(loopStart);
    }

    // Execute the main loop at a constant rate.
    delayUntil(loopStart + MAIN_LOOP_FREQUENCY);
}
