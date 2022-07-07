#include "LoRa.h"


bool LoRa::begin(float frequency) {
    isConnected = lora.init();
    if (!isConnected) {
        return false;
    }
    lora.setFrequency(frequency);
    return true;
}

void LoRa::send(void* data, size_t size) {
    lora.send(static_cast<uint8_t*>(data), size);
}

bool LoRa::available() {
    return lora.available();
}

bool LoRa::read(void* buffer, uint8_t& bytesRead) {
    return lora.recv(static_cast<uint8_t*>(buffer), &bytesRead) && bytesRead != 0;
}
