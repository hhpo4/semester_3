#include <windows.h>
#include <stdio.h>

int main() {
    // Создание или открытие именованного события
    HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, "Global\\CharEnteredEvent");

    if (hEvent == NULL) {
        printf("Не удалось создать или открыть событие. Ошибка: %d\n", GetLastError());
        return 1;
    }

    printf("Ожидаем ввод символов от Slave...\n");

    while (1) {
        DWORD result = WaitForSingleObject(hEvent, INFINITE);
        if (result == WAIT_OBJECT_0) {
            printf("*\n"); // Символ при каждом вводе в slave
        } else {
            printf("Ошибка ожидания события. Код: %d\n", GetLastError());
            break;
        }
    }

    CloseHandle(hEvent);
    return 0;
}
