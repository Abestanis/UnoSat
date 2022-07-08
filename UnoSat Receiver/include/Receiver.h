#pragma once

#include <SoftwareSerial.h>
#include "generated/units.h"
#include "generated/sharedTypes.h"
#include "CircularBufferHandler.h"
#include "Screen.h"
#include "generated/receiver.h"
#include "LoRa.h"


class Receiver {
public:
    explicit Receiver(LoRa<HardwareSerial>& lora, Screen& screen) : lora(lora), screen(screen) {
    };

    ~Receiver() = default;

    void handleMessages();


protected:
    void onDataMessage(ms_t time, deg_t latitude, deg_t longitude, mm_t attitude, uint8_t visibleSatellites, celsius_t temperature, percent_t humidity, pascal_t pressure);

    static void onNewLogMessage(ms_t time, LogLevel level, uint16_t size, const char* message);

    static void handleParserError(ParserError error);

    LoRa<HardwareSerial>& lora;
    Screen& screen;
    CircularBufferHandler<uint8_t, 256> loraMessageBuffer = {};

    MessageHandler handler {
            .handleMessageData = [](
                    MessageHandler* self, ms_t time, deg_t latitude, deg_t longitude, mm_t attitude, uint8_t visibleSatellites, celsius_t temperature, percent_t humidity, pascal_t pressure) {
                return ((Receiver*) self->data)->onDataMessage(time, latitude, longitude, attitude, visibleSatellites, temperature, humidity, pressure);
            },
            .handleMessageLog = [](MessageHandler* self, ms_t time, LogLevel level, uint16_t size, const char* message) {
                return Receiver::onNewLogMessage(time, level, size, message);
            },
            .errorHandler = [](MessageHandler* self, ParserError error) {
                return Receiver::handleParserError(error);
            },
            .data = this,
    };
};

