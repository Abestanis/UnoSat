// *** AUTOGENERATED FILE - Do not edit manually! - Changes will be overwritten! *** //

#include <string.h>
#include <stddef.h>
#include "generated/telemetry.h"
#include "generated/checksum.h"


extern bool writeTelemetry(void* data, size_t size);

/** Data for the DATA message. */
typedef struct PACKED {
    /** The CPU time this telemetry was generated. */
    ms_t time;
    /** The temperature. */
    celsius_t temperature;
    /** The air humidity. */
    percent_t humidity;
    /** The air pressure. */
    pascal_t pressure;
} TelemetryDataData;

/** A DATA message. */
typedef struct PACKED {
    /** The message header. */
    TelemetryMessageHeader header;
    /** The message data. */
    TelemetryDataData data;
} TelemetryDataType;

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



bool sendTelemetryData(ms_t time, celsius_t temperature, percent_t humidity, pascal_t pressure) {
    size_t messageSize = sizeof(TelemetryDataType);
    TelemetryDataType data;
    data.header.syncByte1 = SYNC_BYTE_1;
    data.header.syncByte2 = SYNC_BYTE_2;
    data.header.type = TELEMETRY_DATA;
    data.data.time = time;
    data.data.temperature = temperature;
    static_assert(sizeof(float) == 4,
            "float has an invalid size on this platform and can not be used");
    data.data.humidity = humidity;
    static_assert(sizeof(float) == 4,
            "float has an invalid size on this platform and can not be used");
    data.data.pressure = pressure;
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
    data->header.type = TELEMETRY_LOG;
    data->data.time = time;
    data->data.level = level;
    data->data.size = size;
    memcpy(&data->data.message, message, size * sizeof(message[0]));
    data->header.checksum = calculateChecksum(
        &((uint8_t*) &data->header.checksum)[sizeof(data->header.checksum)],
        messageSize - sizeof(data->header.syncByte1)
        - sizeof(data->header.syncByte2) - sizeof(data->header.checksum));
    return writeTelemetry(data, messageSize);
}