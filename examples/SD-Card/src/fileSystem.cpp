#include <Arduino.h>
#include <SdFat.h>
#include <sdios.h>

#include "fileSystem.h"
#include "log.h"

static bool isFileSystemInitialized = false;
static SdFs sd;

bool FileSystem::ensureInitialized() {
    if (!isFileSystemInitialized) {
        isFileSystemInitialized = sd.begin(SdSpiConfig(PIND4, DEDICATED_SPI, SPI_HALF_SPEED));
    }
    return isFileSystemInitialized;
}

bool FileSystem::isInitialized() {
    return isFileSystemInitialized;
}



#define ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))

static void listDirectory(FsFile& directory, uint8_t flags = 0, uint8_t indent = 0) {
    FsFile child;
    while (child.openNext(&directory, O_RDONLY)) {
        {
            // We can't allocate much more, the RAM on the Arduino UNO is very limited.
            char buffer[128];
            // print any indent spaces
            memset(buffer, ' ', indent);
            size_t writeOffset = indent;
            writeOffset += child.getName(&buffer[writeOffset], ARRAY_SIZE(buffer) - writeOffset);
            if (child.isDirectory() && writeOffset < ARRAY_SIZE(buffer) - 1) {
                buffer[writeOffset++] = '/';
            }

            // print modify date/time if requested
//            constexpr size_t dateSize = sizeof("YYYY-MM-DD") - 1;
//            constexpr size_t timeSize = sizeof("hh:mm") - 1;
//            if ((flags & LS_DATE) && writeOffset < ARRAY_SIZE(buffer) - (3 + dateSize + timeSize)) {
//                uint16_t modifiedDate, modifiedTime;
//                if (child.getModifyDateTime(&modifiedDate, &modifiedTime)) {
//                    buffer[writeOffset++] = ' ';
//                    writeOffset += dateSize; // Increment first, because `fsFmtDate` writes backwards for some reason.
//                    fsFmtDate(&buffer[writeOffset], modifiedDate);
//                    buffer[writeOffset++] = ' ';
//                    writeOffset += timeSize; // Increment first, because `fsFmtTime` writes backwards for some reason.
//                    fsFmtTime(&buffer[writeOffset], modifiedTime);
//                }
//            }
//            // print size if requested
//            if ((flags & LS_SIZE) && !child.isDirectory() && writeOffset < ARRAY_SIZE(buffer) - 10) {
//                buffer[writeOffset++] = ' ';
//                writeOffset += sprintf(&buffer[writeOffset], "%lu", (unsigned long) child.size());
//            }
            buffer[writeOffset] = '\0';
            Logger::info(buffer);
        }

        // list subdirectory content if requested
        if ((flags & LS_R) && child.isDirectory()) {
            listDirectory(child, flags, indent + 2);
        }
        child.close();
    }
}

void FileSystem::logFileSystemInfo() {

    // print the type of card
//    const char* cardType;
//    switch (sd.card()->type()) {
//        case SD_CARD_TYPE_SD1:
//            cardType = "SD1";
//            break;
//        case SD_CARD_TYPE_SD2:
//            cardType = "SD2";
//            break;
//        case SD_CARD_TYPE_SDHC:
//            csd_t csd;
//            if (sd.card()->readCSD(&csd) && csd.capacity() < 70000000) {
//                cardType = "SDHC";
//            } else {
//                cardType = "SDXC";
//            }
//            break;
//        default:
//            cardType = "Unknown";
//    }
//    Logger::debugFormatted("Card type: %s", cardType);

    // print the type and size of the first FAT-type volume
    const char* volumeType;
    switch (sd.fatType()) {
        case FAT_TYPE_EXFAT:
            volumeType = "ExFat";
            break;
        case FAT_TYPE_FAT32:
            volumeType = "Fat32";
            break;
        case FAT_TYPE_FAT16:
            volumeType = "Fat16";
            break;
        default:
            volumeType = "unknown";
    }
    Logger::debugFormatted("Volume type is %s", volumeType);

    // The SD-Card is made of clusters, which are made up of blocks of 512 bytes.
    double volumeSize = ((double) ((uint64_t) sd.clusterCount() * sd.bytesPerCluster())) / 1024.0;
    const char* unit = "Kb";
    if (volumeSize > 1024) {
        volumeSize /= 1024;
        unit = "Mb";
        if (volumeSize > 1024) {
            volumeSize /= 1024;
            unit = "Gb";
        }
    }
    // we cant use %.2f here, because it is not supported on the Arduino Uno
    char buf[33];
    Logger::debugFormatted("Volume size: %s%s", dtostrf(volumeSize, 4, 2, buf), unit);

    // list all files in the card with date and size
    FsFile root;
    if (!root.open("/")) {
        Logger::error("Failed to open SD-Card root");
        return;
    }
    Logger::info("Files found on the card (name, date and size in bytes):");
    listDirectory(root, LS_R | LS_DATE | LS_SIZE);
}
