#include <Arduino.h>

#include "generated/telemetry.h"
#include "sensor/Temperature.h"
#include "sensor/Pressure.h"
#include "Screen.h"
#include "log.h"
#include "LoRa.h"

/** The frequency at which the main loop will be running */
static constexpr ms_t MAIN_LOOP_FREQUENCY = 800_ms;
/** The frequency at which telemetry should be send. */
static constexpr ms_t TELEMETRY_SEND_FREQUENCY = ms_t(MAIN_LOOP_FREQUENCY.ms * 2);

/** The LCD screen to print output to. */
static Screen screen;
// Send our telemetry via the hardware serial (UART1) to the LoRa module.
static HardwareSerial& loraSerial = Serial;

static LoRa<HardwareSerial> lora(loraSerial);
static Sensor::Temperature temperatureSensor(3);
static Sensor::Pressure pressureSensor;

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
extern "C" bool writeTelemetry(void *data, size_t size) {
    return lora.send(data, size);
}

/**
 * The standard Arduino setup function that will be called when the Arduino boots.
 * Initialize the system.
 */
void setup() {
    Logger::sendDebugMessages(true);
    bool loraConnectionOk = lora.begin(868.0);
    Logger::info("Booting...");

    // Do initialization tasks
    if (!screen.init()) {
        Logger::error("Screen is not available");
    }
    screen.printCentered("UnoSat Base", 0);
    screen.printCentered("Booting...", 2);
    if (!temperatureSensor.connect()) {
        Logger::error("Temperature sensor is not available");
    }
    if (!pressureSensor.connect()) {
        Logger::error("Pressure sensor is not available");
    }
    Logger::info("Boot complete");
    screen.printCentered("Boot complete", 2);
    screen.printAt("LoRa: ", 0, 2);
    screen.print(loraConnectionOk ? "Ok" : "Missing");
    screen.clearToNextLine();
}

/**
 * The standard Arduino loop function, which will be called over and over again after boot.
 * Run the system tasks.
 */
void loop() {
    // Get the time at the start of this loop iteration.
    ms_t loopStart = ms_t(millis());

    // Read the senors and gather data.
    celsius_t temperature = celsius_t(NAN);
    // Prefer temperature from the pressure sensor, it's more accurate.
    if (!pressureSensor.readTemperature(temperature)) {
        temperatureSensor.readTemperature(temperature);
    }
    percent_t humidity = percent_t(NAN);
    temperatureSensor.readHumidity(humidity);
    pascal_t pressure = pascal_t(0);
    pressureSensor.readPressure(pressure);

    static ms_t lastTelemetrySend = 0_ms;
    if (ms_t(millis()) - lastTelemetrySend >= TELEMETRY_SEND_FREQUENCY) {
        if (!sendTelemetryData(ms_t(millis()), temperature, humidity, pressure)) {
            Logger::error("Failed to send telemetry data");
        }
        lastTelemetrySend = ms_t(millis());
    }
    screen.printAt("Temp: ", 0, 3);
    screen.print(temperature.celsius);
    screen.print(" C");
    screen.clearToNextLine();
    screen.print("Hum: ");
    screen.print(humidity.percent);
    screen.print(" %");
    screen.clearToNextLine();
    screen.printAt("Press: ");
    screen.print(pressure.pascal);
    screen.print(" Pa");
    screen.clearToNextLine();
    screen.clearRemainingLine();

    // Execute the main loop at a constant rate.
    delayUntil(loopStart + MAIN_LOOP_FREQUENCY);
}
