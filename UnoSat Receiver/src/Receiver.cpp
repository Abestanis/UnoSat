#include "Receiver.h"


void Receiver::handleMessages() {
    if (!lora.available()) {
        return;
    }
    uint8_t buffer[LoRa<HardwareSerial>::MAX_READ_SIZE];
    uint8_t receivedBytes = LoRa<HardwareSerial>::MAX_READ_SIZE;
    if (!lora.read(buffer, receivedBytes)) {
        return;
    }
    loraMessageBuffer.copy(buffer, receivedBytes);
    while (parseMessage(*loraMessageBuffer, &handler)) {
    }
}

void Receiver::onDataMessage(
        ms_t time, deg_t latitude, deg_t longitude, mm_t attitude, uint8_t visibleSatellites,
        celsius_t temperature, percent_t humidity, pascal_t pressure) {
    _lastMessageTimestamp = ms_t(millis());
    // Serial.print("[DATA ] (");
    // Serial.print(time.ms);
    // Serial.print(") ");
    screen.printAt("Temp: ", 0, 3);
    screen.print(temperature.celsius);
    screen.print(" C");
    // Serial.print("Temperature: ");
    // Serial.print(temperature.celsius);
    screen.clearToNextLine();
    screen.print("Hum: ");
    screen.print(humidity.percent);
    screen.print(" %");
    // Serial.print(", Humidity: ");
    // Serial.print(humidity.percent);
    screen.clearToNextLine();
    screen.printAt("Press: ");
    screen.print(pressure.pascal);
    screen.print(" Pa");
    // Serial.print(", Pressure: ");
    // Serial.print(pressure.pascal);
    screen.clearToNextLine();
    screen.printAt("GPS SIV: ");
    screen.print(visibleSatellites);
    screen.clearRemainingLine();
    // Serial.print(", latitude: ");
    // Serial.print(latitude.deg);
    // Serial.print(", longitude: ");
    // Serial.print(longitude.deg);
    // Serial.print(", attitude: ");
    // Serial.print(attitude.mm);
    // Serial.print(", SIV: ");
    // Serial.print(visibleSatellites);
    // Serial.println();
}

void Receiver::onNewLogMessage(ms_t time, LogLevel level, uint16_t size, const char* message) {
    _lastMessageTimestamp = ms_t(millis());
    switch (level) {
    case LOG_DEBUG:
        // Serial.print("[DEBUG]");
        break;
    case LOG_INFO:
        // Serial.print("[INFO ]");
        break;
    case LOG_ERROR:
        // Serial.print("[ERROR]");
        break;
    default:
        // Serial.print("[?: ");
        // Serial.print((int) level);
        // Serial.print(']');
        break;
    }
    // Serial.print(" (");
    // Serial.print(time.ms);
    // Serial.print(") ");
    // Serial.write(message, size);
    // Serial.println();
}

void Receiver::handleParserError(ParserError error) {
    switch (error) {
    case INVALID_MESSAGE:
        // Serial.println("Invalid message type!");
        break;
    case CHECKSUM_ERROR:
        // Serial.println("Checksum Error!");
        break;
    case DYNAMIC_SIZE_TOO_LARGE:
        // Serial.println("Dynamic size was too large!");
        break;
    default:
        // Serial.println("Unknown error!");
        break;
    }
}
