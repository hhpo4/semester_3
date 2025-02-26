// benchmark.h
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
        vector<int> arr = generateRandomArray<int>(size);
        int randomIndex = (rand() % size);
        int desiredElement = MAX_ARRAY_ELEMENT + 1;
        arr[randomIndex] = desiredElement;
        
        auto startSeq = chrono::high_resolution_clock::now();
        int seqIndex = sequentialSearch(arr, desiredElement);
        auto endSeq = chrono::high_resolution_clock::now();
        chrono::duration<double, nano> seqDuration = endSeq - startSeq;
        
        vector<int> sortedArr = arr;
        quickSort(sortedArr);

        auto startBin = chrono::high_resolution_clock::now();
        int binIndex = fibonacciSearch(sortedArr, desiredElement);
        auto endBin = chrono::high_resolution_clock::now();
        chrono::duration<double, nano> binDuration = endBin - startBin;
        
        resultsFile << size << "," 
                    << seqDuration.count() << "," 
                    << binDuration.count() << "\n";
                    
        cout << "Размер: " << size << endl;
        cout << "Последовательный поиск: индекс " << seqIndex << ", время: " << seqDuration.count() << " сек" << endl;
        cout << "Бинарный поиск: индекс " << binIndex << ", время: " << binDuration.count() << " сек" << endl;
        cout << "----------------------------------------------" << endl;
    }
    resultsFile.close();
}

#endif
