#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BUFFER_SIZE 10  // размер буфера

// Глобальные переменные для буфера и индексов
int buffer[BUFFER_SIZE];
int in = 0;      // индекс для записи
int out = 0;     // индекс для чтения
int count = 0;   // текущее количество элементов в буфере

// Объекты синхронизации
CRITICAL_SECTION cs;
HANDLE semEmpty; // семафор, отслеживающий количество свободных ячеек
HANDLE semFull;  // семафор, отслеживающий количество заполненных ячеек

// Функция-поток производителя
DWORD WINAPI Producer(LPVOID lpParam) {
    while (1) {
        // Генерируем случайное число (например, четное)
        int item = (rand() % 100) * 2;

        // Ждем появления свободной ячейки
        WaitForSingleObject(semEmpty, INFINITE);

        // Заходим в критическую секцию для изменения общих переменных
        EnterCriticalSection(&cs);
        
        // Помещаем элемент в буфер по схеме кольцевого буфера
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        count++;

        // Вывод информации о работе производителя
        printf("Producer produced: %d\n", item);
        printf("Buffer: ");
        for (int i = 0; i < BUFFER_SIZE; i++) {
            printf("%d ", buffer[i]);
        }
        printf("\nBuffer usage: %d%% full\n\n", (count * 100) / BUFFER_SIZE);
        
        LeaveCriticalSection(&cs);

        // Сигнализируем, что появился новый заполненный слот
        ReleaseSemaphore(semFull, 1, NULL);

        Sleep(500); // задержка для наглядности работы
    }
    return 0;
}

// Функция-поток потребителя
DWORD WINAPI Consumer(LPVOID lpParam) {
    while (1) {
        // Ждем наличия заполненного слота
        WaitForSingleObject(semFull, INFINITE);

        // Заходим в критическую секцию для доступа к буферу
        EnterCriticalSection(&cs);
        
        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;

        // Вывод информации о работе потребителя
        printf("Consumer consumed: %d\n", item);
        printf("Buffer: ");
        for (int i = 0; i < BUFFER_SIZE; i++) {
            printf("%d ", buffer[i]);
        }
        printf("\nBuffer usage: %d%% full\n\n", (count * 100) / BUFFER_SIZE);

        LeaveCriticalSection(&cs);

        // Сигнализируем, что появился новый свободный слот
        ReleaseSemaphore(semEmpty, 1, NULL);

        Sleep(800); // задержка для имитации разной скорости работы
    }
    return 0;
}

int main() {
    // Инициализация генератора случайных чисел
    srand((unsigned int)time(NULL));

    // Инициализация критической секции
    InitializeCriticalSection(&cs);

    // Создаем семафоры:
    // semEmpty начинается со значением BUFFER_SIZE, т.к. изначально все ячейки свободны
    semEmpty = CreateSemaphore(NULL, BUFFER_SIZE, BUFFER_SIZE, NULL);
    // semFull начинается с 0, так как буфер изначально пуст
    semFull = CreateSemaphore(NULL, 0, BUFFER_SIZE, NULL);

    // Создаем потоки производителя и потребителя
    HANDLE hProducer = CreateThread(NULL, 0, Producer, NULL, 0, NULL);
    HANDLE hConsumer = CreateThread(NULL, 0, Consumer, NULL, 0, NULL);

    // Ожидаем завершения потоков (в данном примере потоки работают бесконечно)
    WaitForSingleObject(hProducer, INFINITE);
    WaitForSingleObject(hConsumer, INFINITE);

    // Очистка ресурсов
    DeleteCriticalSection(&cs);
    CloseHandle(semEmpty);
    CloseHandle(semFull);

    return 0;
}
