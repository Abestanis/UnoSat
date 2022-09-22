#pragma once

class FileSystem {
public:
    static bool ensureInitialized();

    static bool isInitialized();
    static void logFileSystemInfo();
private:
    FileSystem() = default;
};
