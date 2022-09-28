#include <Arduino.h>

#include "LoRa.h"

/** Forward all received telemetry via the hardware serial (UART1). */
static HardwareSerial& outputSerial = Serial;

/** The chip select pin for the LoRa module. */
static constexpr uint8_t loraChipSelectPin = 10;
/** The interrupt pin for the LoRa module. */
static constexpr uint8_t loraInterruptPin = 2;
/** The lora module to receive telemetry from and forward telecommands to. */
static LoRa lora(loraChipSelectPin, loraInterruptPin);

/**
 * The standard Arduino setup function that will be called when the Arduino boots.
 * Initialize the system.
 */
void setup() {
    outputSerial.begin(19200);
    lora.begin(433.0);
}

/**
 * The standard Arduino loop function, which will be called over and over again after boot.
 * Run the system tasks.
 */
void loop() {
    // Check if any data is available from the LoRa.
    if (lora.available()) {
        // There is data available, so read it and forward it to the output serial.
        uint8_t buffer[LoRa::MAX_READ_SIZE];
        uint8_t bufferLength = sizeof(buffer) / sizeof(buffer[0]);
        lora.read(buffer, bufferLength);
        outputSerial.write(buffer, bufferLength);
    }
    // Check if any data is available from the serial.
    if (outputSerial.available()) {
        // There is data available, so read it and forward it to the LoRa.
        uint8_t buffer[LoRa::MAX_READ_SIZE];
        uint8_t bufferLength = sizeof(buffer) / sizeof(buffer[0]);
        outputSerial.readBytes(buffer, bufferLength);
        lora.send(buffer, bufferLength);
    }
}
