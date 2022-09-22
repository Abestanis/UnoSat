#pragma once

#include <stddef.h>

class FileSystem {
public:
    static bool ensureInitialized();

    static bool isInitialized();
    static void logFileSystemInfo();

    static bool writeToLogFile(const void* buffer, size_t length);
private:
    FileSystem() = default;
};
