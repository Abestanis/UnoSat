#include <Arduino.h>
#include <SoftwareSerial.h>
#include "Screen.h"
#include "Receiver.h"

static Screen screen;
static HardwareSerial& loraSerial = Serial;
static LoRa<HardwareSerial> lora(loraSerial);
static Receiver loraReceiver(lora, screen);

void setup() {
//    Serial.begin(115200);
//    Serial.println("Booting...");
    if (!screen.init()) {
//        Serial.println("Screen is not available");
    }
    screen.printCentered("UnoSat Receiver", 0);
    screen.printCentered("Booting...", 2);
    loraSerial.begin(9600);
    lora.begin(868.0);
//    Serial.println("Boot complete");
    screen.printCentered("Boot complete", 2);
}

void loop() {
    loraReceiver.handleMessages();
}
