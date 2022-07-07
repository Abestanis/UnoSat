#pragma once

#include <RH_RF95.h>


class LoRa {
public:
    explicit LoRa(SoftwareSerial& serial) : lora(serial) {}
    ~LoRa() = default;

    bool begin(float frequency);
    void send(void* data, size_t size);
    bool available();
    bool read(void* buffer, uint8_t& bytesRead);
    static constexpr uint8_t MAX_READ_SIZE = RH_RF95_MAX_MESSAGE_LEN;
private:
    bool isConnected = false;
    RH_RF95 lora;
};
