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
class LoRa {
public:
    /**
     * Create a new LoRa module interface.
     *
     * @param selectPin The pin to select the LoRa module.
     * @param interruptPin The pin that is used to deliver interrupts from the LoRa module.
     */
    explicit LoRa(uint8_t selectPin = SS, uint8_t interruptPin = 2) : lora(selectPin, interruptPin) {}
    
    ~LoRa() = default;
    
    /**
     * Initialize the LoRa module and set the frequency.
     *
     * @param frequency The frequency at which the LoRa module should operate.
     * @return Whether initialization was successfully.
     */
    bool begin(float frequency);
    
    /**
     * Send data via the LoRa module.
     *
     * @param data The data to send.
     * @param size The number of bytes to send.
     * @return Whether sending was successfully started.
     */
    bool send(void *data, size_t size);
    
    /**
     * @return Whether there is any unread data that was received from the LoRa module.
     */
    bool available();
    
    /**
     * Read received data from the LoRa module.
     *
     * @param buffer The buffer to read into.
     * @param bytesRead The amount of bytes to read and will be set to the amount of bytes actually read.
     * @return Whether reading was successful and at least on byte was read.
     */
    bool read(void *buffer, uint8_t &bytesRead);
    
    /**
     * The maximum number of bytes in a message.
     */
    static constexpr uint8_t MAX_READ_SIZE = RH_RF95_MAX_MESSAGE_LEN;
private:
    /** Whether the LoRa module is connected. */
    bool isConnected = false;
    /** The underlying LoRa interface. */
    RH_RF95 lora;
};

