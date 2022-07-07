#pragma once

#include <U8x8lib.h>

#define SCREEN_CURRENT_VALUE 0xFFU


class Screen : public Print {
public:
    explicit Screen(uint8_t resetPin = U8X8_PIN_NONE, uint8_t clockPin = U8X8_PIN_NONE,
                    uint8_t dataPin = U8X8_PIN_NONE) :
            Print(), display(resetPin, clockPin, dataPin) {
    }

    ~Screen() = default;

    bool init();

    void clearLine(uint8_t y = SCREEN_CURRENT_VALUE);
    void clearRemainingLine(uint8_t y = SCREEN_CURRENT_VALUE);
    void clearToNextLine(uint8_t y = SCREEN_CURRENT_VALUE);

    void printAt(const char* text, uint8_t x = SCREEN_CURRENT_VALUE,
                 uint8_t y = SCREEN_CURRENT_VALUE);

    void printCentered(const char* text, uint8_t y = SCREEN_CURRENT_VALUE);

    void setCursor(uint8_t x = SCREEN_CURRENT_VALUE, uint8_t y = SCREEN_CURRENT_VALUE);

    void clear();

    size_t write(uint8_t uint8) override;

    size_t write(const uint8_t* buffer, size_t size) override;

    int availableForWrite() override;

    void flush() override;

private:
    bool isConnected = false;
    U8X8_SSD1306_128X64_NONAME_HW_I2C display;
};
