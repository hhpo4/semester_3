#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int count = 0; //текущее кол-во
int head = 0;  // Указывает на первый занятый элемент
int tail = 0;  // Указывает на первую свободную позицию

CRITICAL_SECTION cs; // Критическая секция для синхронизации доступа к буферу
HANDLE buffer_not_full; // Событие: буфер не полон
HANDLE buffer_not_empty; // Событие: буфер не пуст

// Функция для вывода состояния буфера
void print_buffer() {
    printf("Буфер: [");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        printf("%d", buffer[i]);
        if (i < BUFFER_SIZE - 1) printf(", ");
    }
    printf("], Заполненность: %.2f%%\n", (float)count / BUFFER_SIZE * 100);
}

DWORD WINAPI producer(LPVOID lpParam) {
    while (1) {
        EnterCriticalSection(&cs);

        while (count == BUFFER_SIZE) {
            LeaveCriticalSection(&cs);
            WaitForSingleObject(buffer_not_full, INFINITE);
            EnterCriticalSection(&cs);
        }

        int number = (rand() % 3) + 1;
        buffer[tail] = number;
        tail = (tail + 1) % BUFFER_SIZE;
        count++;

        printf("Производитель: положил %d. ", number);
        print_buffer();

        SetEvent(buffer_not_empty);
        LeaveCriticalSection(&cs);

        Sleep(rand() % 2000 + 1000);
    }
    return 0;
}


DWORD WINAPI consumer(LPVOID lpParam) {
    while (1) {
        EnterCriticalSection(&cs);

        while (count == 0) {
            LeaveCriticalSection(&cs);
            WaitForSingleObject(buffer_not_empty, INFINITE);
            EnterCriticalSection(&cs);
        }

        int searched_number = (rand() % 3);
        int found_index = -1;
        int current_pos = head;

        // Поиск элемента в буфере
        for (int i = 0; i < count; i++) {
            if (buffer[current_pos] == searched_number) {
                found_index = current_pos;
                break;
            }
            current_pos = (current_pos + 1) % BUFFER_SIZE;
        }

        if (found_index != -1) {
            // Удаляем найденный элемент
            int pos = found_index;
            while (pos != (tail - 1 + BUFFER_SIZE) % BUFFER_SIZE) {
                int next = (pos + 1) % BUFFER_SIZE;
                buffer[pos] = buffer[next];
                pos = next;
            }
            tail = (tail - 1 + BUFFER_SIZE) % BUFFER_SIZE;
            buffer[tail] = 0;
            count--;

            printf("Потребитель: взял %d. ", searched_number);
            print_buffer();

            SetEvent(buffer_not_full);
        } else {
            printf("Потребитель: число %d не найдено. ", searched_number);
            print_buffer();
            
            // Даем шанс производителю добавить элементы
            LeaveCriticalSection(&cs);
            Sleep(100);
            continue;
        }

        LeaveCriticalSection(&cs);
    }
    return 0;
}

int main() {
    srand(time(NULL)); // Инициализация генератора случайных чисел

    // Инициализация критической секции
    InitializeCriticalSection(&cs);

    // Создание событий
    buffer_not_full = CreateEvent(NULL, FALSE, TRUE, NULL); // Буфер изначально не полон
    buffer_not_empty = CreateEvent(NULL, FALSE, FALSE, NULL); // Буфер изначально пуст

    // Инициализация буфера нулями
    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer[i] = 0;
    }

    // Создание потоков
    HANDLE producer_thread = CreateThread(NULL, 0, producer, NULL, 0, NULL);
    HANDLE consumer_thread = CreateThread(NULL, 0, consumer, NULL, 0, NULL);

    // Ожидание завершения потоков (в данном случае они работают бесконечно)
    WaitForSingleObject(producer_thread, INFINITE);
    WaitForSingleObject(consumer_thread, INFINITE);

    // Закрытие дескрипторов
    CloseHandle(producer_thread);
    CloseHandle(consumer_thread);
    CloseHandle(buffer_not_full);
    CloseHandle(buffer_not_empty);

    // Удаление критической секции
    DeleteCriticalSection(&cs);

    return 0;
}
