// *** AUTOGENERATED FILE - Do not edit manually! - Changes will be overwritten! *** //

#include <string.h>
#include <stddef.h>
#include "generated/telemetry.h"
#include "generated/checksum.h"


/**
 * Send the serialized telemetry.
 *
 * @param data The serialized telemetry in a buffer.
 * @param size The size of the serialized telemetry in bytes.
 * @return Whether writing the telemetry was successful.
 */
extern bool writeTelemetry(void* data, size_t size);

/** Data for the GPS message. */
typedef struct PACKED {
    /** The current latitude of the system. */
    deg_t latitude;
    /** The current longitude of the system. */
    deg_t longitude;
    /** The current altitude of the system. */
    mm_t altitude;
    /** The number of visible satellites. */
    uint8_t visibleSatellites;
} TelemetryGpsData;

/** A GPS message. */
typedef struct PACKED {
    /** The message header. */
    TelemetryMessageHeader header;
    /** The message data. */
    TelemetryGpsData data;
} TelemetryGpsType;

/** Data for the LOG message. */
typedef struct PACKED {
    /** The size of the message in bytes. */
    uint16_t size;
    /** The message text. */
    char message[0];
} TelemetryLogData;

/** A LOG message. */
typedef struct PACKED {
    /** The message header. */
    TelemetryMessageHeader header;
    /** The message data. */
    TelemetryLogData data;
} TelemetryLogType;



bool sendTelemetryGps(deg_t latitude, deg_t longitude, mm_t altitude, uint8_t visibleSatellites) {
    size_t messageSize = sizeof(TelemetryGpsType);
    TelemetryGpsType data;
    data.header.syncByte1 = SYNC_BYTE_1;
    data.header.syncByte2 = SYNC_BYTE_2;
    data.header.type = (uint8_t) TELEMETRY_GPS;
    data.data.latitude = latitude;
    static_assert(sizeof(float) == 4,
        "float has an invalid size on this platform and can not be used");
    data.data.longitude = longitude;
    static_assert(sizeof(float) == 4,
        "float has an invalid size on this platform and can not be used");
    data.data.altitude = altitude;
    data.data.visibleSatellites = visibleSatellites;
    data.header.checksum = calculateChecksum(
        &((uint8_t*) &data.header.checksum)[sizeof(data.header.checksum)],
        messageSize - sizeof(data.header.syncByte1)
        - sizeof(data.header.syncByte2) - sizeof(data.header.checksum));
    return writeTelemetry(&data, messageSize);
}

bool sendTelemetryLog(uint16_t size, const char* message) {
    size_t messageSize = sizeof(TelemetryLogType) + size * sizeof(message[0]);
    uint8_t dataBuffer[messageSize];
    TelemetryLogType* data = (TelemetryLogType*) dataBuffer;
    data->header.syncByte1 = SYNC_BYTE_1;
    data->header.syncByte2 = SYNC_BYTE_2;
    data->header.type = (uint8_t) TELEMETRY_LOG;
    data->data.size = size;
    memcpy(&data->data.message, message, size * sizeof(message[0]));
    data->header.checksum = calculateChecksum(
        &((uint8_t*) &data->header.checksum)[sizeof(data->header.checksum)],
        messageSize - sizeof(data->header.syncByte1)
        - sizeof(data->header.syncByte2) - sizeof(data->header.checksum));
    return writeTelemetry(data, messageSize);
}
