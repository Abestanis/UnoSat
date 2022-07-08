#pragma once

#define RH_RF95_MAX_MESSAGE_LEN 128
#include <RH_RF95.h>


template<typename T>
class LoRa {
public:
    explicit LoRa(T& serial) : lora(serial) {}
    ~LoRa() = default;

    bool begin(float frequency);
    bool send(void* data, size_t size);
    bool available();
    bool read(void* buffer, uint8_t& bytesRead);
    static constexpr uint8_t MAX_READ_SIZE = RH_RF95_MAX_MESSAGE_LEN;
private:
    bool isConnected = false;
    RH_RF95<T> lora;
};
