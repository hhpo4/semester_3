#include <windows.h>
#include <iostream>
#include <cstring>

int main() {
    std::cout << "=== IPC Client Process ===" << std::endl;

    const char* mapName = "Global\\MySharedMemory";
    const int bufferSize = 4096;

    // 1. Открываем File Mapping
    HANDLE hMap = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, mapName);
    if (!hMap) {
        std::cerr << "OpenFileMapping failed: " << GetLastError() << std::endl;
        return 1;
    }

    // 2. Проецируем память
    char* buffer = static_cast<char*>(MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, bufferSize));
    if (!buffer) {
        std::cerr << "MapViewOfFile failed: " << GetLastError() << std::endl;
        CloseHandle(hMap);
        return 1;
    }

    std::cout << "Listening for messages (press Ctrl+C to exit):" << std::endl;

    // 3. Бесконечный цикл чтения
    while (true) {
        if (buffer[0] != '\0') { // Если в буфере есть данные
            std::cout << "Received: " << buffer << std::endl;
        } else {
            std::cout << "Waiting for data..." << std::endl;
        }
        Sleep(1000); // Пауза 1 сек
    }

    // 4. Очистка (не выполнится из-за while(true), но для порядка)
    UnmapViewOfFile(buffer);
    CloseHandle(hMap);
    return 0;
}