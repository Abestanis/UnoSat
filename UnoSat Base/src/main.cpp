#include <Arduino.h>
#include "generated/telemetry.h"
#include "sensor/RtkGps.tpp"
#include "sensor/Temperature.h"
#include "sensor/Pressure.h"
#include "Screen.h"

#include "log.h"
#include "LoRa.h"

static Screen screen;
static HardwareSerial& loraSerial = Serial;

static SoftwareSerial rtkSerial(7, 8);
static LoRa<HardwareSerial> lora(loraSerial);
static RtkGps<(sizeof(TelemetryMessageHeader) + 64) * 2> rtkGps(rtkSerial);
static Sensor::Temperature temperatureSensor(3);
static Sensor::Pressure pressureSensor;

static void delayUntil(ms_t wakeTime) {
    ms_t currentTime = ms_t(millis());
    if (currentTime.ms > wakeTime.ms) {
        return;
    }
    delay(wakeTime.ms - currentTime.ms);
}

extern "C" bool writeTelemetry(void* data, size_t size) {
    return lora.send(data, size);
}


void setup() {
    Logger::enableDebugMessages = true;
    bool loraConnectionOk = lora.begin(868.0);
    Logger::info("Booting...");
    if (!screen.init()) {
        Logger::error("Screen is not available");
    }
    screen.printCentered("UnoSat Base", 0);
    screen.printCentered("Booting...", 2);
    rtkSerial.begin(COMMUNICATION_BAUD_RATE);
    if (!temperatureSensor.connect()) {
        Logger::error("Temperature sensor is not available");
    }
    if (!pressureSensor.connect()) {
        Logger::error("Pressure sensor is not available");
    }
    Logger::info("Boot complete");
    screen.printCentered("Boot complete", 2);
}

void loop() {
    ms_t loopStart = ms_t(millis());
    static bool hasLastSend = false;

    // Gather data
    rtkGps.handleMessages();
    celsius_t temperature = celsius_t(NAN);
    // Prefer temperature from the pressure sensor, it's more accurate.
    if (!pressureSensor.readTemperature(temperature)) {
        temperatureSensor.readTemperature(temperature);
    }
    percent_t humidity = percent_t(NAN);
    temperatureSensor.readHumidity(humidity);
    pascal_t pressure = pascal_t(0);
    pressureSensor.readPressure(pressure);

    deg_t latitude = deg_t(rtkGps.getLastLatitude().deg);
    deg_t longitude = deg_t(rtkGps.getLastLongitude().deg);
    mm_t altitude = mm_t(rtkGps.getLastAttitude().mm);
    uint8_t visibleSatellites = rtkGps.getLastVisibleSatellites();

    if (!hasLastSend) {
        if (!sendTelemetryData(ms_t(millis()), latitude, longitude, altitude,
                visibleSatellites, temperature, humidity, pressure)) {
            Logger::error("Failed to send telemetry data");
        }
    }
    hasLastSend = !hasLastSend;
    screen.printAt("Temp: ", 0, 2);
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
    screen.printAt("GPS SIV: ");
    screen.print(visibleSatellites);
    screen.clearRemainingLine();

    // Sleep to keep a constant rate
    delayUntil(ms_t(loopStart.ms + 800));
}