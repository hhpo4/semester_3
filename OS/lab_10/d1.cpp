#include <windows.h>
#include <iostream>
#include <string>
#include <cstring>

int main() {
    std::cout << "=== IPC Server Process ===" << std::endl;

    const char* mapName = "Global\\MySharedMemory";
    const int bufferSize = 4096;

    // 1. Создаем File Mapping
    HANDLE hMap = CreateFileMappingA(
        INVALID_HANDLE_VALUE,
        NULL,
        PAGE_READWRITE,
        0,
        bufferSize,
        mapName
    );
    if (!hMap) {
        std::cerr << "CreateFileMapping failed: " << GetLastError() << std::endl;
        return 1;
    }

    // 2. Проецируем память
    char* buffer = static_cast<char*>(MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, bufferSize));
    if (!buffer) {
        std::cerr << "MapViewOfFile failed: " << GetLastError() << std::endl;
        CloseHandle(hMap);
        return 1;
    }

    // 3. Инициализация буфера
    strncpy(buffer, "Server initialized", bufferSize - 1);
    buffer[bufferSize - 1] = '\0';

    // 4. Основной цикл
    std::cout << "Enter messages (type 'exit' to quit):" << std::endl;
    while (true) {
        std::string input;
        std::getline(std::cin, input);

        if (input == "exit") break;

        // Явно копируем данные в общую память
        strncpy(buffer, input.c_str(), bufferSize - 1);
        buffer[bufferSize - 1] = '\0'; // Обязательно!

        std::cout << "Sent: " << buffer << std::endl;
    }

    // 5. Очистка
    UnmapViewOfFile(buffer);
    CloseHandle(hMap);
    return 0;
}