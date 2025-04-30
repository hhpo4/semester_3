#include <windows.h>
#include <iostream>
#include <string>
#include <cstring> // Для стандартных строковых функций

// Альтернатива для strcpy_s и strcat_s в MinGW
void safe_strcpy(char* dest, size_t dest_size, const char* src) {
    strncpy(dest, src, dest_size - 1);
    dest[dest_size - 1] = '\0';
}

void safe_strcat(char* dest, size_t dest_size, const char* src) {
    size_t len = strlen(dest);
    if (len < dest_size) {
        strncat(dest, src, dest_size - len - 1);
    }
}

int main() {
    std::cout << "=== File Mapping Example ===" << std::endl;

    const char* filename = "memory_mapped_file.dat";
    const int file_size = 4096; // 4 KB

    // 1. Создаем файл
    HANDLE hFile = CreateFileA(
        filename,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "CreateFile failed: " << GetLastError() << std::endl;
        return 1;
    }

    // 2. Создаем File Mapping
    HANDLE hMap = CreateFileMappingA(
        hFile,
        NULL,
        PAGE_READWRITE,
        0,
        file_size,
        NULL
    );

    if (!hMap) {
        std::cerr << "CreateFileMapping failed: " << GetLastError() << std::endl;
        CloseHandle(hFile);
        return 1;
    }

    // 3. Проецируем файл в память
    char* mapView = static_cast<char*>(MapViewOfFile(
        hMap,
        FILE_MAP_ALL_ACCESS,
        0,
        0,
        file_size
    ));

    if (!mapView) {
        std::cerr << "MapViewOfFile failed: " << GetLastError() << std::endl;
        CloseHandle(hMap);
        CloseHandle(hFile);
        return 1;
    }

    // 4. Работаем с данными
    std::string message = "Hello from memory mapped file!";
    safe_strcpy(mapView, file_size, message.c_str());

    std::cout << "Written to file: " << mapView << std::endl;

    // 5. Модифицируем данные
    safe_strcat(mapView, file_size, " [MODIFIED]");
    std::cout << "Modified content: " << mapView << std::endl;

    // 6. Освобождаем ресурсы
    UnmapViewOfFile(mapView);
    CloseHandle(hMap);
    CloseHandle(hFile);

    std::cout << "File operations completed. Check " << filename << std::endl;
    return 0;
}