#include "Screen.h"

bool Screen::init() {
    //u8x8.setBusClock(100000);  // If you breakout other modules, please enable this line
    isConnected = display.begin();
    if (!isConnected) {
        return false;
    }
    display.setFlipMode(1);
    display.setFont(u8x8_font_chroma48medium8_r);
    return true;
}

void Screen::printAt(const char* text, uint8_t x, uint8_t y) {
    setCursor(x, y);
    display.print(text);
}

void Screen::setCursor(uint8_t x, uint8_t y) {
    if (x == SCREEN_CURRENT_VALUE) {
        x = display.tx;
    }
    if (y == SCREEN_CURRENT_VALUE) {
        y = display.ty;
    }
    display.setCursor(x, y);
}

void Screen::clear() {
    display.clear();
}

size_t Screen::write(uint8_t uint8) {
    return display.write(uint8);
}

size_t Screen::write(const uint8_t* buffer, size_t size) {
    return display.write(buffer, size);
}

int Screen::availableForWrite() {
    return display.availableForWrite();
}

void Screen::flush() {
    display.flush();
}

void Screen::printCentered(const char* text, uint8_t y) {
    uint8_t width = display.getCols();
    size_t length = strlen(text);
    setCursor((width - length) / 2, y);
    write((const uint8_t*) text, length);
}

void Screen::clearLine(uint8_t y) {
    if (y == SCREEN_CURRENT_VALUE) {
        y = display.ty;
    }
    display.clearLine(y);
    display.setCursor(0, y);
}

void Screen::clearRemainingLine(uint8_t y) {
    uint8_t remainder = display.getCols() - display.tx;
    if (y == SCREEN_CURRENT_VALUE) {
        y = display.ty;
    }
    uint8_t spaces[remainder];
    memset(spaces, ' ', remainder);
    display.setCursor(display.tx, y);
    display.write(spaces, remainder);
}

void Screen::clearToNextLine(uint8_t y) {
    clearRemainingLine(y);
    display.setCursor(0, display.ty + 1);
}
