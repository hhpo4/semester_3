#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>

using namespace std;

// Структура для представления блока памяти
struct MemoryBlock {
    int start;
    int size;
    int process_id; // -1 означает свободный блок
    MemoryBlock* next;
    
    MemoryBlock(int s, int sz, int pid = -1) : start(s), size(sz), process_id(pid), next(nullptr) {}
    
    bool isFree() const {
        return process_id == -1;
    }
};

// Класс для управления памятью
class MemoryManager {
private:
    MemoryBlock* head;
    int total_memory;
    int search_counter;
    vector<float> fragmentation_history;
    
    // Слияние соседних свободных блоков
    void mergeFreeBlocks() {
        MemoryBlock* current = head;
        while (current != nullptr && current->next != nullptr) {
            if (current->isFree() && current->next->isFree()) {
                MemoryBlock* next_block = current->next;
                current->size += next_block->size;
                current->next = next_block->next;
                delete next_block;
            } else {
                current = current->next;
            }
        }
    }
    
public:
    MemoryManager(int total) : total_memory(total), search_counter(0) {
        head = new MemoryBlock(0, total);
    }
    
    ~MemoryManager() {
        MemoryBlock* current = head;
        while (current != nullptr) {
            MemoryBlock* next = current->next;
            delete current;
            current = next;
        }
    }
    
    void resetCounter() {
        search_counter = 0;
    }
    
    // Первое подходящее размещение
    bool firstFit(int process_id, int process_size) {
        MemoryBlock* current = head;
        MemoryBlock* prev = nullptr;
        
        while (current != nullptr) {
            search_counter++;
            if (current->isFree() && current->size >= process_size) {
                // Нашли подходящий блок
                if (current->size > process_size) {
                    // Разделяем блок
                    MemoryBlock* new_block = new MemoryBlock(
                        current->start + process_size,
                        current->size - process_size
                    );
                    new_block->next = current->next;
                    current->next = new_block;
                    current->size = process_size;
                }
                current->process_id = process_id;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false; // Не нашли подходящий блок
    }
    
    // Наиболее подходящее размещение
    bool bestFit(int process_id, int process_size) {
        MemoryBlock* best = nullptr;
        MemoryBlock* current = head;
        MemoryBlock* prev = nullptr;
        int min_diff = total_memory + 1;
        
        while (current != nullptr) {
            search_counter++;
            if (current->isFree() && current->size >= process_size) {
                int diff = current->size - process_size;
                if (diff < min_diff) {
                    min_diff = diff;
                    best = current;
                }
            }
            prev = current;
            current = current->next;
        }
        
        if (best != nullptr) {
            if (best->size > process_size) {
                MemoryBlock* new_block = new MemoryBlock(
                    best->start + process_size,
                    best->size - process_size
                );
                new_block->next = best->next;
                best->next = new_block;
                best->size = process_size;
            }
            best->process_id = process_id;
            return true;
        }
        return false;
    }
    
    // Наименее подходящее размещение
    bool worstFit(int process_id, int process_size) {
        MemoryBlock* worst = nullptr;
        MemoryBlock* current = head;
        MemoryBlock* prev = nullptr;
        int max_size = -1;
        
        while (current != nullptr) {
            search_counter++;
            if (current->isFree() && current->size >= process_size) {
                if (current->size > max_size) {
                    max_size = current->size;
                    worst = current;
                }
            }
            prev = current;
            current = current->next;
        }
        
        if (worst != nullptr) {
            if (worst->size > process_size) {
                MemoryBlock* new_block = new MemoryBlock(
                    worst->start + process_size,
                    worst->size - process_size
                );
                new_block->next = worst->next;
                worst->next = new_block;
                worst->size = process_size;
            }
            worst->process_id = process_id;
            return true;
        }
        return false;
    }
    
    // Освобождение памяти по ID процесса
    void deallocate(int process_id) {
        MemoryBlock* current = head;
        while (current != nullptr) {
            if (current->process_id == process_id) {
                current->process_id = -1;
            }
            current = current->next;
        }
        mergeFreeBlocks();
    }
    
    // Расчет фрагментации
    float calculateFragmentation() {
        int total_free = 0;
        int external_frag = 0;
        int free_blocks = 0;
        
        MemoryBlock* current = head;
        while (current != nullptr) {
            if (current->isFree()) {
                total_free += current->size;
                free_blocks++;
            }
            current = current->next;
        }
        
        if (free_blocks == 0) return 0.0f;
        
        // Внешняя фрагментация (если есть свободные блоки, но нельзя разместить процесс)
        bool can_allocate = false;
        int largest_free_block = 0;
        current = head;
        while (current != nullptr) {
            if (current->isFree()) {
                if (current->size > largest_free_block) {
                    largest_free_block = current->size;
                }
            }
            current = current->next;
        }
        
        // Процент фрагментации
        float fragmentation = (1.0f - (static_cast<float>(largest_free_block) / total_free)) * 100;
        fragmentation_history.push_back(fragmentation);
        return fragmentation;
    }
    
    int getSearchCount() const {
        return search_counter;
    }
    
    void printMemory() const {
        MemoryBlock* current = head;
        cout << "Memory layout:" << endl;
        while (current != nullptr) {
            cout << "[" << current->start << "-" << current->start + current->size - 1 << "], "
                << "Size: " << current->size << ", "
                << (current->isFree() ? "Free" : "Allocated (PID: " + to_string(current->process_id) + ")")
                << endl;
            current = current->next;
        }
    }
};

// Генератор случайных процессов
vector<pair<int, int>> generateProcesses(int num_processes, int max_size) {
    vector<pair<int, int>> processes;
    for (int i = 0; i < num_processes; ++i) {
        int size = rand() % max_size + 1;
        processes.emplace_back(i+1, size);
    }
    return processes;
}

// Тестирование стратегий
void testStrategy(MemoryManager& manager, const vector<pair<int, int>>& processes, 
                const string& strategy_name, bool (*fit_func)(MemoryManager&, int, int)) {
    cout << "\nTesting " << strategy_name << " strategy:" << endl;
    
    manager.resetCounter();
    float total_fragmentation = 0;
    int allocations = 0;
    
    // Аллокация процессов
    for (const auto& proc : processes) {
        if (fit_func(manager, proc.first, proc.second)) {
            allocations++;
            float frag = manager.calculateFragmentation();
            total_fragmentation += frag;
            cout << "Allocated process " << proc.first << " (size: " << proc.second 
                << "), Fragmentation: " << frag << "%" << endl;
        } else {
            cout << "Failed to allocate process " << proc.first << " (size: " << proc.second << ")" << endl;
        }
    }
    
    // Освобождение случайных процессов
    for (int i = 0; i < allocations / 2; ++i) {
        int pid_to_free = rand() % processes.size() + 1;
        manager.deallocate(pid_to_free);
        cout << "Freed process " << pid_to_free << ", Fragmentation: " 
            << manager.calculateFragmentation() << "%" << endl;
    }
    
    cout << "\n" << strategy_name << " results:" << endl;
    cout << "Total search operations: " << manager.getSearchCount() << endl;
    cout << "Average fragmentation: " << (total_fragmentation / processes.size()) << "%" << endl;
    manager.printMemory();
}

// Функции для передачи в testStrategy
bool firstFitWrapper(MemoryManager& manager, int pid, int size) {
    return manager.firstFit(pid, size);
}

bool bestFitWrapper(MemoryManager& manager, int pid, int size) {
    return manager.bestFit(pid, size);
}

bool worstFitWrapper(MemoryManager& manager, int pid, int size) {
    return manager.worstFit(pid, size);
}

int main() {
    srand(time(nullptr));
    
    const int TOTAL_MEMORY = 1024; // 1KB памяти
    const int NUM_PROCESSES = 10;
    const int MAX_PROCESS_SIZE = 256; // Максимальный размер процесса
    
    // Генерация случайных процессов
    auto processes = generateProcesses(NUM_PROCESSES, MAX_PROCESS_SIZE);
    
    // Тестирование стратегий
    MemoryManager firstFitManager(TOTAL_MEMORY);
    testStrategy(firstFitManager, processes, "First Fit", firstFitWrapper);
    
    MemoryManager bestFitManager(TOTAL_MEMORY);
    testStrategy(bestFitManager, processes, "Best Fit", bestFitWrapper);
    
    MemoryManager worstFitManager(TOTAL_MEMORY);
    testStrategy(worstFitManager, processes, "Worst Fit", worstFitWrapper);
    
    return 0;
}