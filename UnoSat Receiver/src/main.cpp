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
    bool loraConnectionOk = lora.begin(868.0);
//    Serial.println("Boot complete");
    screen.printCentered("Boot complete", 2);
    screen.printAt("LoRa: ", 0, 2);
    screen.print(loraConnectionOk ? "Ok" : "Missing");
    screen.clearToNextLine();
    screen.print("Listening...");
    screen.clearToNextLine();
}

void loop() {
    static bool isNoConnectionShown = false;
    loraReceiver.handleMessages();
    if (ms_t(millis()) - loraReceiver.lastMessageTimestamp() < 10000_ms) {
        isNoConnectionShown = false;
    } else if (!isNoConnectionShown) {
        screen.printCentered("<No connection>", 3);
        screen.clearToNextLine();
        screen.clearToNextLine();
        screen.clearToNextLine();
        screen.clearToNextLine();
        isNoConnectionShown = true;
    }
}
