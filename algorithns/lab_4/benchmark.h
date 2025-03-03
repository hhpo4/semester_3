#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <vector>
#include <chrono>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include "quick_sort.h"
#include "search_algorithms.h"
using namespace std;

#define MAX_ARRAY_ELEMENT 10000

template<typename T>
vector<T> generateRandomArray(int size) {
    vector<T> arr(size);
    for (int i = 0; i < size; i++) {
        arr[i] = static_cast<T>(rand() % MAX_ARRAY_ELEMENT);
    }
    return arr;
}

void runBenchmarks() {
vector<int> sizes = {1000000, 1500000, 2000000, 2500000, 3000000};
    ofstream resultsFile("execution_times.txt");
    if (!resultsFile.is_open()) {
        cerr << "Ошибка: не удалось открыть файл для записи результатов!" << endl;
        return;
    }
    
    for (int size : sizes) {
        cout << "Создание массива размером " << size << "...\n";
        vector<double> arr = generateRandomArray<double>(size);
        int randomIndex = (rand() % size);
        double desiredElement = (double)(MAX_ARRAY_ELEMENT + 1);
        arr[randomIndex] = desiredElement;
        
        // Последовательный поиск
        auto startSeq = chrono::high_resolution_clock::now();
        int seqIndex = sequentialSearch(arr, desiredElement);
        auto endSeq = chrono::high_resolution_clock::now();
        chrono::duration<double> seqDuration = endSeq - startSeq; // Микросекунды
        
        // Сортировка массива для бинарного поиска
        cout << "Сортировка массива...\n";
        // vector<int> sortedArr = arr;
        vector<double> sortedArr = arr;
        quickSort(sortedArr);

        // Бинарный поиск
        auto startBin = chrono::high_resolution_clock::now();
        int binIndex = fibonacciSearch(sortedArr, desiredElement);
        auto endBin = chrono::high_resolution_clock::now();
        chrono::duration<double> fibDuration = endBin - startBin; // Микросекунды
        
        // Запись результатов в файл
        resultsFile << size << "," 
                    << seqDuration.count() << "," 
                    << fibDuration.count() << "\n";
                    
        cout << "Размер: " << size << endl;
        cout << "Последовательный поиск: индекс " << seqIndex << ", время: " << seqDuration.count() << " мкс" << endl;
        cout << "Бинарный поиск: индекс " << binIndex << ", время: " << fibDuration.count() << " мкс" << endl;
        cout << "----------------------------------------------" << endl;
    }
    resultsFile.close();
}

#endif