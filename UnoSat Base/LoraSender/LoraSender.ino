#include <SPI.h>
#include <LoRa.h>
#include <stdlib.h>

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
#define I02 2

//Frequencies in MHz
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

//variables needed
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);
HardwareSerial& inputSerial = Serial2;
bool haveDisplay = false;

void setup() {
    Serial.begin(115200);
    Serial.println("LoRa Sender booting...");

    // reset OLED display via software
    pinMode(OLED_RST, OUTPUT);
    digitalWrite(OLED_RST, LOW);
    delay(20);
    digitalWrite(OLED_RST, HIGH);

    // Initialize OLED
    Wire.begin(OLED_SDA, OLED_SCL);
    haveDisplay = display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false); // Address 0x3C for 128x32
    if (!haveDisplay) {
        Serial.println("Display initialization failed");
    } else {
        display.clearDisplay();
        display.setTextColor(WHITE, BLACK);
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.print("LORA Sender");
        display.display();
    }

    // Initializing Serial to UNO connection
    inputSerial.begin(19200, SERIAL_8N1, RX, TX);

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
    Serial.println("LoRa initialization OK!");
    if (haveDisplay) {
        display.setCursor(0, 10);
        display.print("LoRa Init OK!");
        display.setCursor(0, 20);
        display.print("Packages sent:");
        display.setCursor(0, 30);
        display.print(0);
        display.display();
    }
}

void loop() {
    static uint32_t sendCounter = 0;
    if (inputSerial.available()) {
        String buffer = inputSerial.readString();
        LoRa.beginPacket();
        LoRa.print(buffer);
        LoRa.endPacket();
        if (haveDisplay) {
            display.setCursor(0, 30);
            display.print(++sendCounter);
            display.display();
        }
    }
}
