// *** AUTOGENERATED FILE - Do not edit manually! - Changes will be overwritten! *** //

#include <string.h>
#include <stddef.h>
#include "generated/telemetry.h"
#include "generated/checksum.h"


extern bool writeTelemetry(void* data, size_t size);

/** Data for the HEARTBEAT message. */
typedef struct PACKED {
    /** The time the heartbeat was generated in milliseconds since boot. */
    ms_t time;
} TelemetryHeartbeatData;

/** A HEARTBEAT message. */
typedef struct PACKED {
    /** The message header. */
    TelemetryMessageHeader header;
    /** The message data. */
    TelemetryHeartbeatData data;
} TelemetryHeartbeatType;

/** Data for the LOG message. */
typedef struct PACKED {
    /** The CPU time this message was generated. */
    ms_t time;
    /** The severity of this message. */
    TYPED_ENUM(LogLevel, uint8_t) level;
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



bool sendTelemetryHeartbeat(ms_t time) {
    size_t messageSize = sizeof(TelemetryHeartbeatType);
    TelemetryHeartbeatType data;
    data.header.syncByte1 = SYNC_BYTE_1;
    data.header.syncByte2 = SYNC_BYTE_2;
    data.header.type = (uint8_t) TELEMETRY_HEARTBEAT;
    data.data.time = time;
    data.header.checksum = calculateChecksum(
        &((uint8_t*) &data.header.checksum)[sizeof(data.header.checksum)],
        messageSize - sizeof(data.header.syncByte1)
        - sizeof(data.header.syncByte2) - sizeof(data.header.checksum));
    return writeTelemetry(&data, messageSize);
}

bool sendTelemetryLog(ms_t time, LogLevel level, uint16_t size, const char* message) {
    size_t messageSize = sizeof(TelemetryLogType) + size * sizeof(message[0]);
    uint8_t dataBuffer[messageSize];
    TelemetryLogType* data = (TelemetryLogType*) dataBuffer;
    data->header.syncByte1 = SYNC_BYTE_1;
    data->header.syncByte2 = SYNC_BYTE_2;
    data->header.type = (uint8_t) TELEMETRY_LOG;
    data->data.time = time;
    data->data.level = (uint8_t) level;
    data->data.size = size;
    memcpy(&data->data.message, message, size * sizeof(message[0]));
    data->header.checksum = calculateChecksum(
        &((uint8_t*) &data->header.checksum)[sizeof(data->header.checksum)],
        messageSize - sizeof(data->header.syncByte1)
        - sizeof(data->header.syncByte2) - sizeof(data->header.checksum));
    return writeTelemetry(data, messageSize);
}
