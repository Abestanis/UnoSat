#include <Arduino.h>
#include <SoftwareSerial.h>
#include "Screen.h"
#include "Receiver.h"

/** The LCD screen to print output to. */
static Screen screen;

/** The serial connection to the LoRa module from which to receive telemetry. */
static HardwareSerial& loraSerial = Serial;

/** The LoRa module with which to receive telemetry. */
static LoRa<HardwareSerial> lora(loraSerial);

/** The telemetry receiver. */
static Receiver loraReceiver(lora, screen);

void setup() {
    screen.init();
    screen.printCentered("UnoSat Receiver", 0);
    screen.printCentered("Booting...", 2);

    loraSerial.begin(9600);
    bool loraConnectionOk = lora.begin(868.0);

    screen.printCentered("Boot complete", 2);
    screen.printAt("LoRa: ", 0, 2);
    screen.print(loraConnectionOk ? "Ok" : "Missing");
    screen.clearToNextLine();
    screen.print("Listening...");
    screen.clearToNextLine();
}

void loop() {
    static bool isNoConnectionShown = false;
    // Handle telemetry messages received from the LoRa module.
    loraReceiver.handleMessages();
    if (ms_t(millis()) - loraReceiver.lastMessageTimestamp() < 10000_ms) {
        // If we received a message in the last 10 seconds, don't show the "No connection" banner.
        isNoConnectionShown = false;
    } else if (!isNoConnectionShown) {
        // We haven't received a message in 10 seconds, show that we are not connected.
        screen.printCentered("<No connection>", 3);
        for (int i = 0; i < 4; i++) {
            screen.clearToNextLine();
        }
        isNoConnectionShown = true;
    }
}
