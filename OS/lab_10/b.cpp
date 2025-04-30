#include <windows.h>
#include <iostream>
#include <random>
#include <chrono>

int main() {
    std::cout << "=== Memory Benchmark (Swap Detection) ===" << std::endl;

    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);
    GlobalMemoryStatusEx(&memStatus);

    std::cout << "Total physical RAM: " << memStatus.ullTotalPhys / (1024 * 1024) << " MB" << std::endl;
    std::cout << "Available physical RAM: " << memStatus.ullAvailPhys / (1024 * 1024) << " MB" << std::endl;

    // Используем ULL для больших значений
    SIZE_T size = 128 * 1024; // Начинаем с 128 KB
    const SIZE_T max_size = static_cast<SIZE_T>(2) * 1024 * 1024 * 1024; // До 2 GB

    while (size <= max_size) {
        // Выделяем память
        LPVOID mem = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        if (!mem) {
            std::cerr << "Allocation failed at size: " << size / (1024 * 1024) << " MB" << std::endl;
            break;
        }

        // Замеряем время заполнения
        auto start = std::chrono::high_resolution_clock::now();
        
        char* data = static_cast<char*>(mem);
        for (SIZE_T i = 0; i < size; i += 4096) {
            data[i] = rand() % 256;
            
            // Выводим прогресс каждые 8 KB
            if (i % (8 * 1024) == 0) {
                std::cout << "Filled: " << i / 1024 << " KB / " << size / 1024 << " KB\r";
                std::cout.flush();
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        std::cout << "\nSize: " << size / (1024 * 1024) << " MB, Time: " << duration << " ms" << std::endl;

        VirtualFree(mem, 0, MEM_RELEASE);
        size *= 2; // Увеличиваем размер в 2 раза
    }

    return 0;
}