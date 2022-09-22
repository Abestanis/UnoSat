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
    loraMessageBuffer.copyFrom(buffer, receivedBytes);
    while (parseMessage(*loraMessageBuffer, &handler)) {
    }
}

void Receiver::onDataMessage(ms_t time, celsius_t temperature, percent_t humidity, pascal_t pressure) {
    _lastMessageTimestamp = ms_t(millis());
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
    screen.clearRemainingLine();
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
