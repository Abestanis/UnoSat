#include <Arduino.h>
#include <SdFat.h>
#include <sdios.h>

#include "fileSystem.h"
#include "log.h"

static bool isFileSystemInitialized = false;
static SdFs sd;
static FsFile logFile;

/**
 * Whether to print the entire contents of the file system when logging the file system info.
 * Unfortunately, the Arduino Uno does not have enough RAM to support this
 * and also logging to the SD-Card at the same time (because the SD-Card library uses a 512 byte cache,
 * a quarter of the 2048 available RAM on the Arduino Uno, see FsCache::m_buffer).
 */
#define ENABLE_FILE_SYSTEM_PRINTING 0

#define ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))

static void openLogFile() {
    FsFile logDirectory;
    if (logDirectory.open("/logs")) {
        char filePath[sizeof("logs") + 10 + sizeof(".bin")] = "logs/";
        char* fileName = &filePath[sizeof("logs")];
        unsigned int index = 1;
        do {
            snprintf(fileName, ARRAY_SIZE(filePath) - sizeof("logs"), "%u.bin", index++);
        } while (logDirectory.exists(fileName));
        if (!logFile.open(filePath, O_CREAT | O_WRONLY)) {
            Logger::errorFormatted("Failed to create log file at /logs/%s", fileName);
        }
    } else if (sd.mkdir("/logs")) {
        if (!logFile.open("logs/0.bin", O_CREAT | O_WRONLY)) {
            Logger::error("Failed to create log file at /logs/0.bin");
        }
    } else {
        Logger::error("Failed to open/create the logs directory");
    }
}

bool FileSystem::ensureInitialized() {
    if (!isFileSystemInitialized) {
        isFileSystemInitialized = sd.begin(SdSpiConfig(PIND4, DEDICATED_SPI, SPI_HALF_SPEED));
        if (isFileSystemInitialized) {
            openLogFile();
        }
    }
    return isFileSystemInitialized;
}

bool FileSystem::isInitialized() {
    return isFileSystemInitialized;
}

#if ENABLE_FS_PRINTING

static void logChildInfo(FsFile& child, uint8_t indent) {
    // We can't allocate much more, the RAM on the Arduino UNO is very limited.
    char buffer[32];
    // Print any indent spaces
    memset(buffer, ' ', indent);
    size_t writeOffset = indent;
    writeOffset += child.getName(&buffer[writeOffset], ARRAY_SIZE(buffer) - writeOffset);
    if (child.isDirectory() && writeOffset < ARRAY_SIZE(buffer) - 1) {
        buffer[writeOffset++] = '/';
    }

    // Print modify date/time
    constexpr size_t dateSize = sizeof("YYYY-MM-DD") - 1;
    constexpr size_t timeSize = sizeof("hh:mm") - 1;
    if (writeOffset < ARRAY_SIZE(buffer) - (3 + dateSize + timeSize)) {
        uint16_t modifiedDate, modifiedTime;
        if (child.getModifyDateTime(&modifiedDate, &modifiedTime)) {
            buffer[writeOffset++] = ' ';
            writeOffset += dateSize; // Increment first, because `fsFmtDate` writes backwards for some reason.
            fsFmtDate(&buffer[writeOffset], modifiedDate);
            buffer[writeOffset++] = ' ';
            writeOffset += timeSize; // Increment first, because `fsFmtTime` writes backwards for some reason.
            fsFmtTime(&buffer[writeOffset], modifiedTime);
        }
    }

    // Print size
    if (!child.isDirectory() && writeOffset < ARRAY_SIZE(buffer) - 10) {
        buffer[writeOffset++] = ' ';
        writeOffset += sprintf(&buffer[writeOffset], "%lu", (unsigned long) child.size());
    }

    buffer[writeOffset] = '\0';
    Logger::info(buffer);
}

static void listDirectory(FsFile& directory, uint8_t indent = 0) {
    FsFile child;
    while (child.openNext(&directory)) {
        logChildInfo(child, indent);
        if (child.isDirectory()) { // List subdirectory content
            delay(1000);
            listDirectory(child, indent + 1);
        }
    }
    child.close();
}

#endif /* ENABLE_FILE_SYSTEM_PRINTING */

void FileSystem::logFileSystemInfo() {
    { // This scope exists so the variables can be released before calling listDirectory
        // print the type of card
        const char* cardType;
        switch (sd.card()->type()) {
            case SD_CARD_TYPE_SD1:
                cardType = "SD1";
                break;
            case SD_CARD_TYPE_SD2:
                cardType = "SD2";
                break;
            case SD_CARD_TYPE_SDHC:
                csd_t csd;
                if (sd.card()->readCSD(&csd) && csd.capacity() < 70000000) {
                    cardType = "SDHC";
                } else {
                    cardType = "SDXC";
                }
                break;
            default:
                cardType = "Unknown";
        }
        Logger::debugFormatted("Card type: %s", cardType);

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
    }

    // list all files in the card with date and size
    FsFile root;
    if (!root.open("/")) {
        Logger::error("Failed to open SD-Card root");
        return;
    }
#if ENABLE_FILE_SYSTEM_PRINTING
    Logger::info("Files found on the card (name, date and size in bytes):");
    listDirectory(root);
#endif /* ENABLE_FILE_SYSTEM_PRINTING */
}

bool FileSystem::writeToLogFile(const void* buffer, size_t length) {
    // Don't use Logger or any other telemetry generating function in here to avoid infinite recursion.
    if (!isFileSystemInitialized || !logFile) {
        return false;
    }
    if (logFile.write(buffer, length)) {
        logFile.flush();
        return true;
    }
    isFileSystemInitialized = false;
    return false;
}
