#include <Arduino.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include <SoftwareSerial.h>
#include "generated/telemetry.h"

static SFE_UBLOX_GNSS myGNSS;
static SoftwareSerial outputSerial(2, 3);
static HardwareSerial& rtkSerial = Serial;

extern "C" bool writeTelemetry(void* data, size_t size) {
    return outputSerial.write((uint8_t*) data, size) == size;
}

void print(const char* message) {
    sendTelemetryLog(strlen(message), message);
}

static void delayUntil(ms_t wakeTime) {
    ms_t currentTime = ms_t(millis());
    if (currentTime.ms > wakeTime.ms) {
        return;
    }
    delay(wakeTime.ms - currentTime.ms);
}

void setup() {
    outputSerial.begin(COMMUNICATION_BAUD_RATE);
    print("SparkFun u-blox Example");

    rtkSerial.begin(115200);
    do {
        print("GNSS: trying 115200 baud");
        //        myGNSS.enableDebugging(Serial);
        if (myGNSS.begin(rtkSerial)) {
            break;
        } else {
            //myGNSS.factoryReset();
            delay(2000); //Wait a bit before trying again to limit the Serial output
        }
    } while (true);
    print("GNSS serial connected");

    myGNSS.setUART1Output(COM_TYPE_UBX); //Set the UART port to output UBX only
    //    myGNSS.setUART1Output(COM_TYPE_NMEA); //Set the UART port to output NMEA only
    myGNSS.saveConfiguration(); //Save the current settings to flash and BBR
}

void loop() {
    ms_t loopStart = ms_t(millis());

    deg_t latitude = deg_t((float) (myGNSS.getLatitude() * 1e-7));
    deg_t longitude = deg_t((float) (myGNSS.getLongitude() * 1e-7));
    mm_t altitude = mm_t(myGNSS.getAltitude());
    uint8_t SIV = myGNSS.getSIV();

    sendTelemetryGps(latitude, longitude, altitude, SIV);

    delayUntil(ms_t(loopStart.ms + 1000));
}
