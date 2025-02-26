// main.cpp
#include <iostream>
#include <vector>
#include "Student.h"
#include "quick_sort.h"
#include "search_algorithms.h"
#include "benchmark.h"

using namespace std;

int main() {
    cout << "Запуск тестирования для целочисленных массивов:" << endl;
    runBenchmarks();
    
    cout << "\nТестирование поиска для объектов типа Student:" << endl;
    
    vector<Student> students = {
        Student("Alice", 20, 8.5),
        Student("Bob", 22, 7.9),
        Student("Charlie", 19, 9.1),
        Student("Diana", 21, 8.0),
        Student("Ethan", 23, 7.5)
    };
    
    Student searchTarget("Charlie", 19, 9.1);
    int indexSeq = sequentialSearch(students, searchTarget);
    if (indexSeq != -1) {
        cout << "Последовательный поиск: найден " << students[indexSeq] 
             << " на позиции " << indexSeq << endl;
    } else {
        cout << "Последовательный поиск: элемент не найден." << endl;
    }
    
    quickSort(students);
    int indexBin = fibonacciSearch(students, searchTarget);
    if (indexBin != -1) {
        cout << "Поиск фибоначчи : найден " << students[indexBin] 
             << " на позиции " << indexBin << endl;
    } else {
        cout << "Фибоначчи поиск: элемент не найден." << endl;
    }
    
    cout << "\nТеоретическая сложность фибоначчи поиска: O(log n)" << endl;
    
    return 0;
}
