#pragma once

#include <SoftwareSerial.h>
#include "generated/units.h"
#include "generated/sharedTypes.h"
#include "CircularBufferHandler.h"

namespace Rtk {

#include "generated/rtk/receiver.h"

}


#ifdef MAX_TELEMETRY_DATA_SIZE
template<size_t S = (sizeof(TelemetryMessageHeader) + MAX_TELEMETRY_DATA_SIZE) * 2>
#else /* There are dynamically sized messages in the communication database. */

template<size_t S>
#endif /* MAX_TELEMETRY_DATA_SIZE */
class RtkGps {
public:
    explicit RtkGps(Stream& stream) : rtkStream(stream) {
    };

    ~RtkGps() = default;

    void handleMessages() {
        if (rtkStream.available()) {
            rtkMessageBuffer.copyFrom(rtkStream);
            while (parseMessage(*rtkMessageBuffer, &handler)) {
            }
        }
    }

    uint32_t getLastGpsTimestamp() const {
        return lastGpsTimestamp;
    }

    Rtk::deg_t getLastLatitude() const {
        return lastLatitude;
    }

    Rtk::deg_t getLastLongitude() const {
        return lastLongitude;
    }

    Rtk::mm_t getLastAttitude() const {
        return lastAttitude;
    }

    uint8_t getLastVisibleSatellites() const {
        return lastVisibleSatellites;
    }

protected:
    void onNewGpsMessage(Rtk::deg_t latitude, Rtk::deg_t longitude, Rtk::mm_t attitude,
                         uint8_t visibleSatellites) {
        lastGpsTimestamp = millis();
        lastLatitude = latitude;
        lastLongitude = longitude;
        lastAttitude = attitude;
        lastVisibleSatellites = visibleSatellites;
    }

    void onNewLogMessage(const char* message, uint16_t size) {
        Serial.print("[RTK] ");
        Serial.write(message, size);
        Serial.println();
    }

    void handleParserError(Rtk::ParserError error) {
        switch (error) {
        case Rtk::INVALID_MESSAGE:
            Serial.println("RTK Invalid message type!");
            break;
        case Rtk::CHECKSUM_ERROR:
            Serial.println("RTK Checksum Error!");
            break;
        case Rtk::DYNAMIC_SIZE_TOO_LARGE:
            Serial.println("RTK Dynamic size was too large!");
            break;
        default:
            Serial.println("RTK Unknown error!");
        }
    }

    Stream& rtkStream;
    CircularBufferHandler<uint8_t, S> rtkMessageBuffer = {};

    Rtk::MessageHandler handler {
            .handleMessageGps = [](
                    Rtk::MessageHandler* self, Rtk::deg_t latitude, Rtk::deg_t longitude, Rtk::mm_t attitude,
                    uint8_t visibleSatellites) {
                return ((RtkGps<S>*) self->data)
                        ->onNewGpsMessage(latitude, longitude, attitude, visibleSatellites);
            },
            .handleMessageLog = [](Rtk::MessageHandler* self, uint16_t size, const char* message) {
                return ((RtkGps<S>*) self->data)->onNewLogMessage(message, size);
            },
            .errorHandler = [](Rtk::MessageHandler* self, Rtk::ParserError error) {
                return ((RtkGps<S>*) self->data)->handleParserError(error);
            },
            .data = this,
    };

    uint32_t lastGpsTimestamp = 0;
    Rtk::deg_t lastLatitude = {};
    Rtk::deg_t lastLongitude = {};
    Rtk::mm_t lastAttitude = {};
    uint8_t lastVisibleSatellites = 0;
};
