#pragma once

#include <U8x8lib.h>


/**
 * A LCD screen.
 * All x coordinates go from left to right and y coordinated from top to bottom.
 */
class Screen : public Print {
public:
    /**
     * Initialize a new LCD screen on the given pins.
     *
     * @param resetPin The reset pin.
     * @param clockPin The pin for the clock.
     * @param dataPin The pin for data communication.
     */
    explicit Screen(uint8_t resetPin = U8X8_PIN_NONE, uint8_t clockPin = U8X8_PIN_NONE,
                    uint8_t dataPin = U8X8_PIN_NONE) :
            Print(), display(resetPin, clockPin, dataPin) {
    }

    ~Screen() = default;

    /**
     * Initialize the screen.
     *
     * @return Whether screen initialization was successful.
     */
    bool init();

    /**
     * Clear the given line.
     *
     * @param y The y position of the line to clear or empty for the current line.
     */
    void clearLine(uint8_t y = SCREEN_CURRENT_VALUE);

    /**
     * Clear the remainder of the given line starting at the current x position.
     *
     * @param y The y position of the line to clear or empty for the current line.
     */
    void clearRemainingLine(uint8_t y = SCREEN_CURRENT_VALUE);

    /**
     * Clear the remainder of the given line starting at the current x position
     * and advance the cursor to the beginning of the next line.
     *
     * @param y The y position of the line to clear or empty for the current line.
     */
    void clearToNextLine(uint8_t y = SCREEN_CURRENT_VALUE);

    /**
     * Print the text at the given position.
     *
     * @param text The null terminated text string to print.
     * @param x The x position to print at or empty for the current x position.
     * @param y The y position to print at or empty for the current y position.
     */
    void printAt(const char* text, uint8_t x = SCREEN_CURRENT_VALUE,
                 uint8_t y = SCREEN_CURRENT_VALUE);

    /**
     * Print the text centered on the given line.
     *
     * @param text The null terminated text string to print.
     * @param y The y position to print at or empty for the current y position.
     */
    void printCentered(const char* text, uint8_t y = SCREEN_CURRENT_VALUE);

    /**
     * Set the cursor position.
     *
     * @param x The x position to change the curser to.
     * @param y The y position to change the curser to or empty for the current y position.
     */
    void setCursor(uint8_t x, uint8_t y = SCREEN_CURRENT_VALUE);

    /**
     * Erase the entire screen.
     */
    void clear();

    /**
     * Write a character at the current cursor, advances the cursor by 1 in the x direction.
     * When the end of the line is reached, the cursor will go to the beginning of the next line.
     *
     * @param character The character to write.
     * @return 1 if the character was written successfully, 0 otherwise.
     */
    size_t write(uint8_t character) override;

    /**
     * Write the text from the buffer to the screen, advancing the cursor while writing the individual characters.
     * When the end of the line is reached, the cursor will go to the beginning of the next line.
     *
     * @param buffer The buffer containing the text to write to the screen.
     * @param size The number of characters in the buffer.
     * @return The number of characters that were written to the screen.
     */
    size_t write(const uint8_t* buffer, size_t size) override;

    /**
     * @return The number of characters that can be written without blocking and waiting for the screen.
     */
    int availableForWrite() override;

    /**
     * Push pending changes to the screen.
     */
    void flush() override;

private:
    /** A value that indicates the current value. */
    static constexpr uint8_t SCREEN_CURRENT_VALUE = 0xFFU;

    /** Whether the screen is connected successfully. */
    bool isConnected = false;

    /** The underlying screen interface. */
    U8X8_SSD1306_128X64_NONAME_HW_I2C display;
};
