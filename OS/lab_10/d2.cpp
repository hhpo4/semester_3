#include <windows.h>
#include <iostream>
#include <cstring>

int main() {
    std::cout << "=== IPC Client Process ===" << std::endl;

    const char* mapName = "Global\\MySharedMemory";
    const int bufferSize = 4096;

    HANDLE hMap = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, mapName);
    if (!hMap) {
        std::cerr << "OpenFileMapping failed: " << GetLastError() << std::endl;
        return 1;
    }

    char* buffer = static_cast<char*>(MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, bufferSize));
    if (!buffer) {
        std::cerr << "MapViewOfFile failed: " << GetLastError() << std::endl;
        CloseHandle(hMap);
        return 1;
    }

    std::cout << "Listening for messages (press Ctrl+C to exit):" << std::endl;
    while (true) {
        std::cout << "Received: ";
        std::cout.write(buffer, std::strlen(buffer));
        std::cout << std::endl;

        Sleep(1000); // вместо std::this_thread::sleep_for
    }

    UnmapViewOfFile(buffer);
    CloseHandle(hMap);
    return 0;
}
