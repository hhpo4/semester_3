#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_PROCESSES 7

// Структура для описания процесса
typedef struct {
    char id[10];
    int burst_time;      // полное время выполнения
    int remaining_time;  // остаточное время (для RR)
    int waiting_time;
    int turnaround_time;
    int priority;        // приоритет процесса (чем меньше число, тем выше приоритет)
    int arrival_time;    // время прибытия в очередь готовности
    int completed;       // флаг завершения (для алгоритмов с учетом прибытия)
} Process;

int comparePriority(const void *a, const void *b) {
    return ((Process *)a)->priority - ((Process *)b)->priority;
}


/*
 * Алгоритм FCFS (First Come First Served)
 * Если время прибытия не задано, считается, что все процессы прибывают в 0.
 */
void fcfs(Process proc[], int n) {
    int time = 0;
    float total_waiting = 0, total_turnaround = 0;
    printf("\n=== Планирование FCFS ===\n");
    printf("Диаграмма Ганта:\n\n");
    printf("--------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("| %s ", proc[i].id);
        proc[i].waiting_time = time;
        time += proc[i].burst_time;
        proc[i].turnaround_time = time;
        total_waiting += proc[i].waiting_time;
        total_turnaround += proc[i].turnaround_time;
    }
    printf("|\n");
    printf("--------------------------------------------------\n");
    
    time = 0;
    for (int i = 0; i < n; i++) {
        time += proc[i].burst_time;
        printf("| %d ", time);
    }
    printf("|\n");
    
    printf("\nПроцесс\tBT\tWT\tTAT\n");
    for (int i = 0; i < n; i++) {
        printf("%s\t%d\t%d\t%d\n", proc[i].id, proc[i].burst_time, proc[i].waiting_time, proc[i].turnaround_time);
    }
    printf("\nСреднее время ожидания: %.2f\nСреднее время оборота: %.2f\n", total_waiting / n, total_turnaround / n);
}


/*
 * Алгоритм SJF (Shortest Job First) – не прерываемый вариант.
 * Процессы сортируются по времени выполнения (burst_time).
 */
void sjf(Process proc[], int n) {
    Process temp[NUM_PROCESSES];
    for (int i = 0; i < n; i++)
        temp[i] = proc[i];

    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (temp[i].burst_time > temp[j].burst_time) {
                Process t = temp[i];
                temp[i] = temp[j];
                temp[j] = t;
            }
        }
    }

    int time = 0;
    float total_waiting = 0, total_turnaround = 0;
    printf("\n=== Планирование SJF (без учета времени прибытия) ===\n");
    printf("Диаграмма Ганта:\n");
    printf("--------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("| %s ", temp[i].id);
        temp[i].waiting_time = time; // Время ожидания равно текущему времени
        time += temp[i].burst_time;
        temp[i].turnaround_time = time; // Время оборота равно текущему времени
        total_waiting += temp[i].waiting_time;
        total_turnaround += temp[i].turnaround_time;
    }
    printf("|\n");
    printf("--------------------------------------------------\n");
    time = 0;
    for (int i = 0; i < n; i++) {
        time += temp[i].burst_time;
        printf("   %d", time);
    }
    printf("\n");

    printf("\nПроцесс\tBT\tWT\tTAT\n");
    for (int i = 0; i < n; i++) {
        printf("%s\t%d\t%d\t%d\n", temp[i].id, temp[i].burst_time, temp[i].waiting_time, temp[i].turnaround_time);
    }
    printf("\nСреднее время ожидания: %.2f\nСреднее время оборота: %.2f\n", total_waiting / n, total_turnaround / n);
}

/*
 * Алгоритм Round Robin (RR).
 * Здесь моделируется работа алгоритма для разных значений кванта времени.
 */
void roundRobin(Process proc[], int n, int quantum) {
    Process temp[NUM_PROCESSES];
    for (int i = 0; i < n; i++) {
        temp[i] = proc[i];
        temp[i].remaining_time = temp[i].burst_time;
    }

    int time = 0, done = 0, index = 0;
    float total_waiting = 0, total_turnaround = 0;
    int gant_times[100]; // Хранение временных меток
    char gant_processes[100][10]; // Хранение ID процессов
    printf("\n=== Round Robin (Квант = %d) ===\n", quantum);
    printf("Диаграмма Ганта:\n");
    printf("--------------------------------------------------\n");

    // Пока не завершены все процессы
    while (done < n) {
        int executed = 0;
        for (int i = 0; i < n; i++) {
            if (temp[i].remaining_time > 0) {
                gant_times[index] = time; // Сохранение времени перед процессом
                strcpy(gant_processes[index], temp[i].id);
                index++;

                printf("| %s ", temp[i].id);
                if (temp[i].remaining_time > quantum) {
                    time += quantum;
                    temp[i].remaining_time -= quantum;
                } else {
                    time += temp[i].remaining_time;
                    temp[i].waiting_time = time - temp[i].burst_time;
                    temp[i].turnaround_time = time;
                    temp[i].remaining_time = 0;
                    done++;
                }
                executed = 1;
            }
        }
        if (!executed) {
            time++;
        }
    }
    printf("|\n--------------------------------------------------\n");

    // Вывод временных меток
    printf("%d", gant_times[0]); // Начальное время
    for (int i = 1; i < index; i++) {
        printf("   %d", gant_times[i]);
    }
    printf("   %d\n", time); // Конечное время

    for (int i = 0; i < n; i++) {
        total_waiting += temp[i].waiting_time;
        total_turnaround += temp[i].turnaround_time;
    }

    printf("\nПроцесс\tBT\tWT\tTAT\n");
    for (int i = 0; i < n; i++) {
        printf("%s\t%d\t%d\t%d\n", temp[i].id, temp[i].burst_time, temp[i].waiting_time, temp[i].turnaround_time);
    }
    printf("\nСреднее время ожидания: %.2f\nСреднее время оборота: %.2f\n", total_waiting / n, total_turnaround / n);
}


/*
 * Алгоритм приоритетного планирования (без учёта времени прибытия).
 * Процессы сортируются по приоритету (чем меньше значение, тем выше приоритет).
 * Для каждого процесса выводится его приоритет.
 * Если приоритет одинаковый, первый выполняется тот процесс, который прибыл первым.
 */
void priorityScheduling(Process proc[], int n) {
    Process temp[n];
    for (int i = 0; i < n; i++) {
        temp[i] = proc[i];
    }

    // Сортировка по приоритету (меньшее значение приоритета = более высокий приоритет)
    qsort(temp, n, sizeof(Process), comparePriority);

    int time = 0;
    float total_waiting = 0, total_turnaround = 0;
    int gant_times[n + 1];  // Для хранения временных меток
    char gant_processes[n][10];  // Для хранения имен процессов

    printf("\n=== Приоритетное планирование ===\n");
    printf("Диаграмма Ганта:\n");
    printf("--------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        strcpy(gant_processes[i], temp[i].id);
        gant_times[i] = time;

        printf("| %s ", temp[i].id);
        temp[i].waiting_time = time;
        time += temp[i].burst_time;
        temp[i].turnaround_time = time;
        total_waiting += temp[i].waiting_time;
        total_turnaround += temp[i].turnaround_time;
    }
    gant_times[n] = time;  // Финальное время

    printf("|\n--------------------------------------------------\n");

    // Вывод временных меток под диаграммой
    for (int i = 0; i <= n; i++) {
        printf("%d   ", gant_times[i]);
    }
    printf("\n");

    printf("\nПроцесс\tBT\tAT\tПриоритет\tWT\tTAT\n");
    for (int i = 0; i < n; i++) {
        printf("%s\t%d\t%d\t%d\t\t%d\t%d\n", temp[i].id, temp[i].burst_time, temp[i].arrival_time, temp[i].priority, temp[i].waiting_time, temp[i].turnaround_time);
    }

    printf("\nСреднее время ожидания: %.2f\nСреднее время оборота: %.2f\n", total_waiting / n, total_turnaround / n);
}

/*
 * Алгоритм приоритетного планирования с учётом времени прибытия.
 * Здесь на каждом шаге выбирается из процессов, которые уже прибыли и не завершены, тот,
 * у которого наивысший приоритет (минимальное значение priority).
 *
 * Механизм «старения» можно реализовать так:
 * на каждом такте времени для каждого процесса, находящегося в очереди (ожидает выполнения),
 * его значение приоритета корректируется (например, уменьшается на фиксированную величину или
 * вычисляется с учетом времени ожидания), что позволит процессам, которые долго ждут, повышать свои шансы на выполнение.
 */
void prioritySchedulingWithArrival(Process proc[], int n) {
    Process temp[n];
    for (int i = 0; i < n; i++) {
        temp[i] = proc[i];
        temp[i].completed = 0;
    }

    int time = 0, completed = 0;
    float total_waiting = 0, total_turnaround = 0;
    int gant_times[n];  // Временные метки (без начальной 0)
    char gant_processes[n][10];  // Названия процессов
    int gant_index = 0;

    printf("\n=== Приоритетное планирование (с учетом AT) ===\n");
    printf("Диаграмма Ганта:\n--------------------------------------------------\n");

    while (completed < n) {
        int idx = -1;
        int best_priority = 100000;
        int min_arrival = 100000;

        // Поиск доступного процесса с наивысшим приоритетом
        for (int i = 0; i < n; i++) {
            if (temp[i].arrival_time <= time && temp[i].completed == 0) {
                if (temp[i].priority < best_priority ||
                    (temp[i].priority == best_priority && temp[i].arrival_time < min_arrival)) {
                    best_priority = temp[i].priority;
                    min_arrival = temp[i].arrival_time;
                    idx = i;
                }
            }
        }

        // Если нет доступных процессов, продвигаем время
        if (idx == -1) {
            time++;
            continue;
        }

        // Записываем диаграмму Ганта только после первого выполненного процесса
        if (gant_index > 0) {
            gant_times[gant_index - 1] = time;
        }
        strcpy(gant_processes[gant_index], temp[idx].id);
        gant_index++;

        printf("| %s ", temp[idx].id);

        // Обновляем время ожидания и оборота
        temp[idx].waiting_time = time - temp[idx].arrival_time;
        time += temp[idx].burst_time;
        temp[idx].turnaround_time = time - temp[idx].arrival_time;

        total_waiting += temp[idx].waiting_time;
        total_turnaround += temp[idx].turnaround_time;
        temp[idx].completed = 1;
        completed++;
    }
    gant_times[gant_index - 1] = time;  // Финальная временная отметка

    printf("|\n--------------------------------------------------\n");

    for (int i = 0; i < gant_index; i++) {
        printf("%d\t", gant_times[i]);
    }
    printf("\n");

    printf("\nПроцесс\tBT\tAT\tPriority\tWT\tTAT\n");
    for (int i = 0; i < n; i++) {
        printf("%s\t%d\t%d\t%d\t\t%d\t%d\n", temp[i].id, temp[i].burst_time, temp[i].arrival_time, temp[i].priority, temp[i].waiting_time, temp[i].turnaround_time);
    }

    printf("\nСреднее время ожидания: %.2f\nСреднее время оборота: %.2f\n", total_waiting / n, total_turnaround / n);
}


int main() {
    //(чем меньше значение, тем выше приоритет)
    Process processes[NUM_PROCESSES] = {
        {"p1", 3, 3, 0, 0, 1, 0, 0},
        {"p2", 20,20, 0, 0, 4, 0, 0},
        {"p3", 15, 15, 0, 0, 3, 0, 0},
        {"p4", 10, 10, 0, 0, 2, 0, 0},
        {"p5", 12, 12, 0, 0, 1, 0, 0},
        {"p6", 8, 8, 0, 0, 1, 0, 0},
        {"p7", 1, 1, 0, 0, 0, 0, 0}
    };

    // Для алгоритмов с учетом времени прибытия зададим условные значения AT
    Process processesWithArrival[NUM_PROCESSES];
    for (int i = 0; i < NUM_PROCESSES; i++) {
        processesWithArrival[i] = processes[i];
    }
    // Пример значений времени прибытия (можно изменить или запросить у пользователя)
    processesWithArrival[0].arrival_time = 0;   // p1
    processesWithArrival[1].arrival_time = 2;   // p2
    processesWithArrival[2].arrival_time = 4;   // p3
    processesWithArrival[3].arrival_time = 6;   // p4
    processesWithArrival[4].arrival_time = 8;   // p5
    processesWithArrival[5].arrival_time = 10;  // p6
    processesWithArrival[6].arrival_time = 12;  // p7

    int choice;
    do {
        printf("\nВыберите алгоритм планирования:\n");
        printf("1. FCFS\n");
        printf("2. SJF\n");
        printf("3. Round Robin\n");
        printf("4. Приоритетное планирование (без учета времени прибытия)\n");
        printf("5. Приоритетное планирование (с учетом времени прибытия)\n");
        printf("6. Выход\n");
        printf("Ваш выбор: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                fcfs(processes, NUM_PROCESSES);
                break;
            case 2:
                sjf(processes, NUM_PROCESSES);
                break;
            case 3: {
                // Для алгоритма RR тестируем 5 различных квантов времени
                int quanta[8] = {2, 4, 6, 8, 10, 12, 100, 200};
                for (int i = 0; i < 8; i++) {
                    roundRobin(processes, NUM_PROCESSES, quanta[i]);
                    printf("--------------------------------------------------\n");
                }
                break;
            }
            case 4:
                priorityScheduling(processes, NUM_PROCESSES);
                break;
            case 5:
                prioritySchedulingWithArrival(processesWithArrival, NUM_PROCESSES);
                break;
            case 6:
                printf("Выход из программы.\n");
                break;
            default:
                printf("Неверный выбор. Попробуйте снова.\n");
        }
    } while(choice != 6);

    return 0;
}
