/** Data types that are transmitted via telecommands and telemetry. */
// *** AUTOGENERATED FILE - Do not edit manually! - Changes will be overwritten! *** //

#pragma once

#include <stdint.h>
#include "generated/util.h"
#include "generated/units.h"

#ifdef __cplusplus
extern "C" {
#endif

/** The first byte of every message. */
#define SYNC_BYTE_1 170
/** The second byte of every message. */
#define SYNC_BYTE_2 85


/** All possible types of telemetry messages. */
typedef enum {
    /** Telemetry data. */
    TELEMETRY_DATA = 0,
    /** A log message. */
    TELEMETRY_LOG = 1,

    TELEMETRY_TYPE_FIRST MAYBE_UNUSED = TELEMETRY_DATA,
    TELEMETRY_TYPE_LAST MAYBE_UNUSED = TELEMETRY_LOG,
} TelemetryType;

/** The header of a message for the ground station. */
typedef struct PACKED {
    /** The first synchronisation byte. */
    uint8_t syncByte1;
    /** The second synchronisation byte. */
    uint8_t syncByte2;
    /**
     * The checksum of the message.
     * The two sync bytes and the checksum are not included in the checksum,
     * but the telemetry data is included.
     */
    uint16_t checksum;
    /** The type of telemetry that is send in this message. */
    TYPED_ENUM(TelemetryType, uint8_t) type;
} TelemetryMessageHeader;

/** The level of a telemetry message. */
typedef enum {
    /** A verbose logging message that should only be send during debugging. */
    LOG_DEBUG = 0,
    /** An informational message. */
    LOG_INFO = 1,
    /** A message indicating an error or failure. */
    LOG_ERROR = 2,

    LOG_LEVEL_FIRST MAYBE_UNUSED = LOG_DEBUG,
    LOG_LEVEL_LAST MAYBE_UNUSED = LOG_ERROR,
} LogLevel;


#ifdef __cplusplus
}
#endif
