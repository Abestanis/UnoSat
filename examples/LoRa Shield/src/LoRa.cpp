#include "LoRa.h"


bool LoRa::begin(float frequency) {
    isConnected = lora.init();
    if (!isConnected) {
        return false;
    }
    lora.setFrequency(frequency);
    return true;
}

bool LoRa::send(void *data, size_t size) {
    if (!isConnected || size > MAX_READ_SIZE) {
        return false;
    }
    return lora.send(static_cast<uint8_t *>(data), size);
}

bool LoRa::available() {
    return isConnected && lora.available();
}

bool LoRa::read(void *buffer, uint8_t &bytesRead) {
    return isConnected && lora.recv(static_cast<uint8_t *>(buffer), &bytesRead) && bytesRead != 0;
}
