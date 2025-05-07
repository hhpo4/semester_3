#include <windows.h>
#include <iostream>

int main() {
    std::cout << "=== Basic Virtual Memory Operations ===" << std::endl;

    // 1. Резервируем регион памяти
    LPVOID mem = VirtualAlloc(NULL, 4096, MEM_RESERVE, PAGE_READWRITE);
    if (!mem) {
        std::cerr << "VirtualAlloc (reserve) failed: " << GetLastError() << std::endl;
        return 1;
    }
    std::cout << "Memory reserved at: " << mem << std::endl;

    // 2. Выделяем физическую память в зарезервированном регионе
    mem = VirtualAlloc(mem, 4096, MEM_COMMIT, PAGE_READWRITE);
    if (!mem) {
        std::cerr << "VirtualAlloc (commit) failed: " << GetLastError() << std::endl;
        return 1;
    }
    std::cout << "Physical memory committed" << std::endl;

    // 3. Используем память (записываем данные)
    int* numbers = static_cast<int*>(mem);
    numbers[0] = 42;
    std::cout << "Value written to memory: " << numbers[0] << std::endl;

    // 4. Освобождаем физическую память, сохраняется регион
    if (!VirtualFree(mem, 4096, MEM_DECOMMIT)) {
        std::cerr << "VirtualFree (decommit) failed: " << GetLastError() << std::endl;
        return 1;
    }
    std::cout << "Physical memory decommitted" << std::endl;

    // 5. Освобождаем регион полностью
    if (!VirtualFree(mem, 0, MEM_RELEASE)) {
        std::cerr << "VirtualFree (release) failed: " << GetLastError() << std::endl;
        return 1;
    }
    std::cout << "Memory region released" << std::endl;

    return 0;
}