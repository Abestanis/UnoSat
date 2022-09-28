#include <Arduino.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include <SoftwareSerial.h>
#include "generated/telemetry.h"

/** The frequency at which the main loop will be running. */
static constexpr ms_t MAIN_LOOP_FREQUENCY = 1000_ms;

/** The Rx pin for the communication with the base Arduino. */
static constexpr uint8_t outputReceiverPin = 2;

/** The Tx pin for the communication with the base Arduino. */
static constexpr uint8_t outputTransmitPin = 3;

/** The serial connected to the base Arduino to send output telemetry towards. */
static SoftwareSerial outputSerial(outputReceiverPin, outputTransmitPin);

/** The RTK GPS module connection. */
static SFE_UBLOX_GNSS myGNSS;

/** The serial connected to the RTK GPS module. */
static HardwareSerial& rtkSerial = Serial;

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
 * Send a log message to the base Arduino.
 *
 * @param message A null terminated string containing the message.
 */
static void print(const char* message) {
    sendTelemetryLog(strlen(message), message);
}

/**
 * The standard Arduino setup function that will be called when the Arduino boots.
 * Initialize the system.
 */
void setup() {
    outputSerial.begin(COMMUNICATION_BAUD_RATE);
    print("Booting...");

    // Connect to the RTK GPS module.
    rtkSerial.begin(115200);
    while (!myGNSS.begin(rtkSerial)) {
        //myGNSS.factoryReset();
        delay(1000);
    }
    print("GNSS serial connected");

    // Configure the RTK GPS module
    myGNSS.setUART1Output(COM_TYPE_UBX); // Set the UART port to output UBX only
    myGNSS.saveConfiguration(); // Save the current settings to flash and BBR
    print("Boot complete");
}

/**
 * The standard Arduino loop function, which will be called over and over again after boot.
 * Run the system tasks.
 */
void loop() {
    // Get the time at the start of this loop iteration.
    ms_t loopStart = ms_t(millis());

    // Gather and send location information.
    deg_t latitude = deg_t((float) (myGNSS.getLatitude() * 1e-7));
    deg_t longitude = deg_t((float) (myGNSS.getLongitude() * 1e-7));
    mm_t altitude = mm_t(myGNSS.getAltitude());
    uint8_t visibleSatellites = myGNSS.getSIV();
    sendTelemetryGps(latitude, longitude, altitude, visibleSatellites);

    // Execute the main loop at a constant rate.
    delayUntil(loopStart + MAIN_LOOP_FREQUENCY);
}
