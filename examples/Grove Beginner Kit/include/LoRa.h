#pragma once

#ifndef RH_RF95_MAX_MESSAGE_LEN
#  define RH_RF95_MAX_MESSAGE_LEN 128
#endif /* RH_RF95_MAX_MESSAGE_LEN */

#include <RH_RF95.h>


/**
 * An interface to a LoRa module.
 *
 * @tparam T The type of serial that the LoRa module is connected to.
 */
template<typename T>
class LoRa {
public:
    /**
     * Create a new LoRa module interface.
     *
     * @param serial The serial that the LoRa module is connected to.
     */
    explicit LoRa(T& serial) : lora(serial) {}

    ~LoRa() = default;

    /**
     * Initialize the LoRa module and set the frequency.
     *
     * @param frequency The frequency at which the LoRa module should operate.
     * @return Whether initialization was successfully.
     */
    bool begin(float frequency) {
        isConnected = lora.init();
        if (!isConnected) {
            return false;
        }
        lora.setFrequency(frequency);
        return true;
    }

    /**
     * Send data via the LoRa module.
     *
     * @param data The data to send.
     * @param size The number of bytes to send.
     * @return Whether sending was successfully started.
     */
    bool send(void* data, size_t size) {
        if (!isConnected || size > MAX_READ_SIZE) {
            return false;
        }
        return lora.send(static_cast<uint8_t*>(data), size);
    }

    /**
     * @return Whether there is any unread data that was received from the LoRa module.
     */
    bool available() {
        return isConnected && lora.available();
    }

    /**
     * Read received data from the LoRa module.
     *
     * @param buffer The buffer to read into.
     * @param bytesRead The amount of bytes to read and will be set to the amount of bytes actually read.
     * @return Whether reading was successful and at least on byte was read.
     */
    bool read(void* buffer, uint8_t& bytesRead) {
        return isConnected && lora.recv(static_cast<uint8_t*>(buffer), &bytesRead) && bytesRead != 0;
    }

    /**
     * The maximum number of bytes in a message.
     */
    static constexpr uint8_t MAX_READ_SIZE = RH_RF95_MAX_MESSAGE_LEN;
private:
    /** Whether the LoRa module is connected. */
    bool isConnected = false;
    /** The underlying LoRa interface. */
    RH_RF95<T> lora;
};
