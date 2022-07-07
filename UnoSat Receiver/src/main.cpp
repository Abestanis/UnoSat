#include <Arduino.h>
#include <SoftwareSerial.h>
#include "generated/receiver.h"
#include "Screen.h"
#include "Receiver.h"

Screen screen;

SoftwareSerial loraSerial(2, 3);
Receiver loraReceiver(loraSerial, screen);

static void delayUntil(ms_t wakeTime) {
    ms_t currentTime = ms_t(millis());
    if (currentTime.ms > wakeTime.ms) {
        return;
    }
    delay(wakeTime.ms - currentTime.ms);
}

void setup() {
    Serial.begin(115200);
    Serial.println("Booting...");
    if (!screen.init()) {
        Serial.println("Screen is not available");
    }
    screen.printCentered("UnoSat Receiver", 0);
    screen.printCentered("Booting...", 2);
    loraSerial.begin(9600);
    Serial.println("Boot complete");
    screen.printCentered("Boot complete", 2);
}

void loop() {
    ms_t loopStart = ms_t(millis());

    loraReceiver.handleMessages();

    // Sleep to keep a constant rate
    delayUntil(ms_t(loopStart.ms + 100));
}