#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define N_PROCESSES 6
#define N_RESOURCES 5

int allocation[N_PROCESSES][N_RESOURCES] = {
    {0, 1, 0, 0, 1},  // P0
    {1, 0, 0, 0, 0},  // P1
    {0, 0, 1, 0, 0},  // P2
    {0, 0, 0, 0, 0},  // P3
    {0, 0, 0, 1, 0},  // P4
    {0, 0, 0, 0, 0}   // P5
};

// int available[N_RESOURCES] = {3, 4, 6, 5, 6}; 
int available[N_RESOURCES] = {3, 4, 4, 4, 4};  


// Матрица Max (максимальные потребности каждого процесса)
int max_need[N_PROCESSES][N_RESOURCES] = {
    {1, 2, 1, 1, 2},  // P0
    {2, 1, 2, 2, 1},  // P1
    {1, 1, 3, 2, 2},  // P2
    {1, 2, 1, 1, 1},  // P3
    {1, 1, 1, 2, 1},  // P4
    {1, 1, 2, 1, 1}   // P5
};

// Матрица Need = Max - Allocation
int need[N_PROCESSES][N_RESOURCES];

// Функция для вычисления матрицы Need
void calculate_need() {
    for (int i = 0; i < N_PROCESSES; i++) {
        for (int j = 0; j < N_RESOURCES; j++) {
            need[i][j] = max_need[i][j] - allocation[i][j];
        }
    }
}

// Функция проверки безопасности системы.
// Если состояние безопасно, функция возвращает true и заполняет массив safe_seq порядком завершения процессов.
bool is_safe_state(int avail[], int alloc[][N_RESOURCES], int nd[][N_RESOURCES], int safe_seq[]) {
    int work[N_RESOURCES];
    bool finish[N_PROCESSES] = {false};
    // Копируем в work текущее состояние available
    for (int j = 0; j < N_RESOURCES; j++) {
        work[j] = avail[j];
    }
    int count = 0;
    
    bool found;
    do {
        found = false;
        for (int i = 0; i < N_PROCESSES; i++) {
            if (!finish[i]) {
                // Проверяем, что для всех j need[i][j] <= work[j]
                bool canFinish = true;
                for (int j = 0; j < N_RESOURCES; j++) {
                    if (nd[i][j] > work[j]) {
                        canFinish = false;
                        break;
                    }
                }
                if (canFinish) {
                    // Если процесс может завершиться, освободим его ресурсы
                    for (int j = 0; j < N_RESOURCES; j++) {
                        work[j] += alloc[i][j];
                    }
                    finish[i] = true;
                    safe_seq[count++] = i;
                    found = true;
                }
            }
        }
    } while(found);
    
    // Если все процессы завершены, то состояние безопасно
    for (int i = 0; i < N_PROCESSES; i++) {
        if (!finish[i])
            return false;
    }
    return true;
}

// Функция обработки запроса ресурсов процессом
// process_id - идентификатор процесса, request - массив запрашиваемых ресурсов
bool request_resources(int process_id, int request[]) {
    printf("\nЗапрос процесса P%d: [", process_id);
    for (int j = 0; j < N_RESOURCES; j++) {
        printf("%d", request[j]);
        if(j < N_RESOURCES - 1) printf(" ");
    }
    printf("]\n");

    // Проверка 1: запрошенные ресурсы не должны превышать доступные
    for (int j = 0; j < N_RESOURCES; j++) {
        if (request[j] > available[j]) {
            printf("Ошибка: запрошено ресурсов типа %d больше, чем доступно (%d > %d)\n", 
                j, request[j], available[j]);
            return false;
        }
    }

    // Проверка 2: запрос не должен превышать потребности
    for (int j = 0; j < N_RESOURCES; j++) {
        if (request[j] > need[process_id][j]) {
            printf("Ошибка: P%d запрашивает больше, чем его Need.\n", process_id);
            return false;
        }
    }

    int avail_temp[N_RESOURCES];
    int alloc_temp[N_PROCESSES][N_RESOURCES];
    int need_temp[N_PROCESSES][N_RESOURCES];
    int safe_seq[N_PROCESSES];
    
    // Копируем текущие значения
    for (int j = 0; j < N_RESOURCES; j++) {
        avail_temp[j] = available[j];
    }
    for (int i = 0; i < N_PROCESSES; i++) {
        for (int j = 0; j < N_RESOURCES; j++) {
            alloc_temp[i][j] = allocation[i][j];
            need_temp[i][j] = need[i][j];
        }
    }
    
    // Изменяем состояния для процесс process_id
    for (int j = 0; j < N_RESOURCES; j++) {
        avail_temp[j] -= request[j];
        alloc_temp[process_id][j] += request[j];
        need_temp[process_id][j] -= request[j];
    }
    
    // Проверка безопасности нового состояния
    if (is_safe_state(avail_temp, alloc_temp, need_temp, safe_seq)) {
        // Если состояние безопасно — обновляем глобальные данные
        for (int j = 0; j < N_RESOURCES; j++) {
            available[j] = avail_temp[j];
            allocation[process_id][j] = alloc_temp[process_id][j];
            need[process_id][j] = need_temp[process_id][j];
        }

        // Проверяем, удовлетворены ли все потребности процесса
        bool all_needs_satisfied = true;
        for (int j = 0; j < N_RESOURCES; j++) {
            if (need[process_id][j] > 0) {
                all_needs_satisfied = false;
                break;
            }
        }

        if (all_needs_satisfied) {
            printf("Все потребности процесса P%d удовлетворены. Освобождаем ресурсы.\n", process_id);
            for (int j = 0; j < N_RESOURCES; j++) {
                available[j] += allocation[process_id][j];
                allocation[process_id][j] = 0;
            }
        }

        printf("Запрос удовлетворён. Состояние безопасно.\n");
        printf("Последовательность завершения: ");
        for (int i = 0; i < N_PROCESSES; i++) {
            printf("P%d ", safe_seq[i]);
        }
        printf("\n");
        return true;
    } else {
        printf("Запрос не может быть удовлетворён: состояние системы становится небезопасным.\n");
        return false;
    }
}

// Функция для вывода текущего состояния системы
void print_state() {
    printf("\nТекущее состояние системы:\n");
    printf("Available: [");
    for (int j = 0; j < N_RESOURCES; j++) {
        printf("%d", available[j]);
        if(j < N_RESOURCES-1) printf(" ");
    }
    printf("]\n\n");
    
    printf("Allocation:\n");
    for (int i = 0; i < N_PROCESSES; i++) {
        printf("P%d: [", i);
        for (int j = 0; j < N_RESOURCES; j++) {
            printf("%d", allocation[i][j]);
            if(j < N_RESOURCES-1) printf(" ");
        }
        printf("]\n");
    }
    
    printf("\nNeed:\n");
    for (int i = 0; i < N_PROCESSES; i++) {
        printf("P%d: [", i);
        for (int j = 0; j < N_RESOURCES; j++) {
            printf("%d", need[i][j]);
            if(j < N_RESOURCES-1) printf(" ");
        }
        printf("]\n");
    }
    printf("\n");
}

int main() {
    // Вычисление матрицы Need на основе Max и Allocation
    calculate_need();
    
    printf("Начальное состояние системы:\n");
    print_state();
    
    while (1) {
        int process_id;
        int request[N_RESOURCES];
        
        printf("Введите номер процесса (0-%d) или -1 для выхода: ", N_PROCESSES-1);
        if (scanf("%d", &process_id) != 1) {
            printf("Ошибка ввода.\n");
            exit(EXIT_FAILURE);
        }
        
        if (process_id == -1) {
            break;
        }
        
        if (process_id < 0 || process_id >= N_PROCESSES) {
            printf("Неверный номер процесса.\n");
            continue;
        }
        
        printf("Введите запрос ресурсов (в виде %d чисел): ", N_RESOURCES);
        for (int j = 0; j < N_RESOURCES; j++) {
            if (scanf("%d", &request[j]) != 1) {
                printf("Ошибка ввода.\n");
                exit(EXIT_FAILURE);
            }
        }
        
        // Обработка запроса
        request_resources(process_id, request);
        
        // Вывод текущего состояния системы и проверка безопасности
        print_state();
        int safe_seq[N_PROCESSES];
        if (is_safe_state(available, allocation, need, safe_seq)) {
            printf("Состояние системы безопасно.\n");
        } else {
            printf("Состояние системы НЕБЕЗОПАСНО!\n");
        }
    }
    
    printf("Завершение работы программы.\n");
    return 0;
}
