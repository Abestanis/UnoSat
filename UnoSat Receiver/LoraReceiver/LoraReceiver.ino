/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/ttgo-lora32-sx1276-arduino-ide/
*********/

//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 433E6

//OLED pins
#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

//Define the pins for RX and TX, we can choose those which suit us
#define RX 23
#define TX 4

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);
HardwareSerial& forwardSerial = Serial2;
bool haveDisplay = false;

void setup() {
    //initialize Serial Monitor
    Serial.begin(115200);
    Serial.println("LoRa Receiver booting...");

    //reset OLED display via software
    pinMode(OLED_RST, OUTPUT);
    digitalWrite(OLED_RST, LOW);
    delay(20);
    digitalWrite(OLED_RST, HIGH);

    //initialize OLED
    Wire.begin(OLED_SDA, OLED_SCL);
    haveDisplay = display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false); // Address 0x3C for 128x32
    if (!haveDisplay) {
        Serial.println("Display initialization failed");
    } else {
        display.clearDisplay();
        display.setTextColor(WHITE, BLACK);
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.print("LORA Receiver");
        display.display();
    }

    //SPI LoRa pins
    SPI.begin(SCK, MISO, MOSI, SS);
    //setup LoRa transceiver module
    LoRa.setPins(SS, RST, DIO0);

    if (!LoRa.begin(BAND)) {
        Serial.println("Starting LoRa failed!");
        display.setCursor(0, 10);
        display.print("LoRa Init FAILED!");
        display.display();
        while (1);
    }

    forwardSerial.begin(9600, SERIAL_8N1, RX, TX);
    Serial.println("LoRa initialization OK!");
    if (haveDisplay) {
        display.setCursor(0, 10);
        display.println("LoRa Init OK!");
        display.setCursor(0, 20);
        display.print("Packages received:");
        display.setCursor(0, 30);
        display.print(0);
        display.display();
    }
}

void loop() {
    static uint32_t packageCount = 0;
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        while (LoRa.available()) {
            String buffer = LoRa.readString();
            forwardSerial.write(buffer.c_str(), buffer.length());
            if (haveDisplay) {
                display.setCursor(0, 30);
                display.print(++packageCount);
                display.display();
            }
        }
    }
}
