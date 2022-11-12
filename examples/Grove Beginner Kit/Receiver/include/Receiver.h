#pragma once

#include <SoftwareSerial.h>
#include "generated/units.h"
#include "generated/sharedTypes.h"
#include "CircularBufferHandler.h"
#include "Screen.h"
#include "generated/receiver.h"
#include "LoRa.h"


/**
 * A receiver to handle messages from the base Arduino.
 */
class Receiver {
public:
    /**
     * Initialize a new receiver.
     *
     * @param lora The LoRa module to read new messages from.
     * @param screen The screen to display received data on.
     */
    explicit Receiver(LoRa<HardwareSerial>& lora, Screen& screen) : lora(lora), screen(screen) {
    };

    ~Receiver() = default;

    /**
     * Check for new messages and handle them.
     */
    void handleMessages();

    /**
     * @return The time since boot of the last received message.
     */
    ms_t lastMessageTimestamp() const {
        return _lastMessageTimestamp;
    }

protected:
    /**
     * Handle a data message from the base Arduino.
     *
     * @param time The time when the message was generated.
     * @param temperature The measured temperature.
     * @param humidity The measured humidity.
     * @param pressure The measured pressure.
     */
    void onDataMessage(ms_t time, celsius_t temperature, percent_t humidity, pascal_t pressure);

    /**
     * Handle a log message from the base Arduino.
     *
     * @param time The time when the message was generated.
     * @param level The severity level of the message.
     * @param size The size of the message in bytes.
     * @param message The message content.
     */
    void onNewLogMessage(ms_t time, LogLevel level, uint16_t size, const char* message);

    /**
     * Handle a parsing error.
     *
     * @param error The type of error.
     */
    static void handleParserError(ParserError error);

    /**
     * The LoRa module to read new messages from.
     */
    LoRa<HardwareSerial>& lora;

    /**
     * The screen to display received data on.
     */
    Screen& screen;

    /**
     * A circular buffer handler to buffer incoming messages from the base Arduino.
     */
    CircularBufferHandler<256> loraMessageBuffer = {};

    /**
     * The time since boot when the last message was received.
     */
    ms_t _lastMessageTimestamp = ms_t(0);

    /**
     * An ECom message handler that delegates to this receiver.
     */
    MessageHandler handler{
            .handleMessageData = [](
                    MessageHandler* self, ms_t time, celsius_t temperature, percent_t humidity, pascal_t pressure) {
                return ((Receiver*) self->data)->onDataMessage(time, temperature, humidity, pressure);
            },
            .handleMessageLog = [](
                    MessageHandler* self, ms_t time, LogLevel level, uint16_t size, const char* message) {
                return ((Receiver*) self->data)->onNewLogMessage(time, level, size, message);
            },
            .errorHandler = [](MAYBE_UNUSED MessageHandler* self, ParserError error) {
                return Receiver::handleParserError(error);
            },
            .data = this,
    };
};
