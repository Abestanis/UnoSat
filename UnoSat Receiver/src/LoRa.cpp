#include <SoftwareSerial.h>
#include "LoRa.h"


template<typename T>
bool LoRa<T>::begin(float frequency) {
    isConnected = lora.init();
    if (!isConnected) {
        return false;
    }
    lora.setFrequency(frequency);
    return true;
}

template<typename T>
bool LoRa<T>::send(void* data, size_t size) {
    if (!isConnected || size > RH_RF95_MAX_MESSAGE_LEN) {
        return false;
    }
    return lora.send(static_cast<uint8_t*>(data), size);
}

template<typename T>
bool LoRa<T>::available() {
    return isConnected && lora.available();
}

template<typename T>
bool LoRa<T>::read(void* buffer, uint8_t& bytesRead) {
    return isConnected && lora.recv(static_cast<uint8_t*>(buffer), &bytesRead) && bytesRead != 0;
}

template class LoRa<HardwareSerial>;
template class LoRa<SoftwareSerial>;
