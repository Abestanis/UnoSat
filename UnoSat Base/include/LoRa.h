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
private:
    bool isConnected = false;
    RH_RF95 lora;
};
