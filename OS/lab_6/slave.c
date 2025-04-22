#include <windows.h>
#include <stdio.h>
#include <conio.h>

int main() {
    HANDLE hEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, "Global\\CharEnteredEvent");

    if (hEvent == NULL) {
        printf("Не удалось открыть событие. Ошибка: %d\n", GetLastError());
        return 1;
    }

    printf("Введите символы. Для выхода нажмите ESC.\n");

    while (1) {
        char ch = _getch(); // Считывает символ без вывода
        if (ch == 27) break; // ESC

        printf("Вы ввели: %c\n", ch);

        SetEvent(hEvent); // Сигнализируем master
        Sleep(100);
    }

    CloseHandle(hEvent);
    return 0;
}
