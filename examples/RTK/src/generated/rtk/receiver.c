// *** AUTOGENERATED FILE - Do not edit manually! - Changes will be overwritten! *** //

#include "generated/rtk/receiver.h"
#include "generated/checksum.h"


bool parseMessage(BufferAccessor* buffer, MessageHandler* handler) {
    for (size_t availableSize = buffer->availableBytes(buffer);
         availableSize >= sizeof(TelemetryMessageHeader);
         buffer->advance(buffer, 1), availableSize--) {
        uint8_t byteRead;
        if (!buffer->peek(buffer, &byteRead, sizeof(byteRead), 0) || byteRead != SYNC_BYTE_1 ||
            !buffer->peek(buffer, &byteRead, sizeof(byteRead), 1) || byteRead != SYNC_BYTE_2) {
            continue;
        }
        TelemetryMessageHeader header;
        if (!buffer->peek(buffer, &header, sizeof(header), 0)) {
            return false;
        }
        switch (header.type) {
        case TELEMETRY_GPS: {
            size_t dataSize = 13;
            if (buffer->availableBytes(buffer) < dataSize) {
                return false;
            }
            rtk_deg_t latitude;
            buffer->peek(buffer, &latitude, sizeof(latitude), sizeof(header));
            static_assert(sizeof(float) == 4,
                "float has an invalid size on this platform and can not be used");
            rtk_deg_t longitude;
            buffer->peek(buffer, &longitude, sizeof(longitude), sizeof(header) + 4);
            static_assert(sizeof(float) == 4,
                "float has an invalid size on this platform and can not be used");
            rtk_mm_t altitude;
            buffer->peek(buffer, &altitude, sizeof(altitude), sizeof(header) + 8);
            uint8_t visibleSatellites;
            buffer->peek(buffer, &visibleSatellites, sizeof(visibleSatellites), sizeof(header) + 12);
            size_t checksumStartOffset =
                sizeof(header.syncByte1) + sizeof(header.syncByte2) + sizeof(header.checksum);
            if (header.checksum != calculateChecksumFromBuffer(buffer,
                    sizeof(TelemetryMessageHeader) - checksumStartOffset + dataSize,
                    checksumStartOffset)) {
                handler->errorHandler(handler, CHECKSUM_ERROR);
                continue;
            }
            buffer->advance(buffer, sizeof(header) + dataSize);
            handler->handleMessageGps(handler, latitude, longitude, altitude, visibleSatellites);
            return true;
        }
        case TELEMETRY_LOG: {
            size_t dataSize = 0;
            uint16_t size;
            if (!buffer->peek(buffer, &size, sizeof(size), sizeof(header) + dataSize)) {
                return false;
            }
            dataSize += sizeof(uint16_t);
            if (size > 512) {
                handler->errorHandler(handler, DYNAMIC_SIZE_TOO_LARGE);
                continue;
            }
            char message[size];
            if (!buffer->peek(buffer, &message, sizeof(message), sizeof(header) + dataSize)) {
                return false;
            }
            dataSize += size;
            size_t checksumStartOffset =
                sizeof(header.syncByte1) + sizeof(header.syncByte2) + sizeof(header.checksum);
            if (header.checksum != calculateChecksumFromBuffer(buffer,
                    sizeof(TelemetryMessageHeader) - checksumStartOffset + dataSize,
                    checksumStartOffset)) {
                handler->errorHandler(handler, CHECKSUM_ERROR);
                continue;
            }
            buffer->advance(buffer, sizeof(header) + dataSize);
            handler->handleMessageLog(handler, size, message);
            return true;
        }
        default:
            handler->errorHandler(handler, INVALID_MESSAGE);
        }
    }
    return false;
}
