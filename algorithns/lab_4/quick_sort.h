#ifndef SORT_UTILS_H
#define SORT_UTILS_H

#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

template<typename T>
void printArray(const vector<T>& arr) {
    for (const auto& elem : arr)
        cout << elem << " ";
    cout << endl;
}

template<typename T>
int partition(vector<T>& arr, int low, int high, bool debug = false) {
    T pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    
    if (debug) {
        cout << "Опорный элемент: " << pivot << endl;
        cout << "После разделения: ";
        printArray(arr);
    }
    return i + 1;
}

template<typename T>
void quickSort(vector<T>& arr, int low, int high, bool debug = false) {
    if (low < high) {
        int pi = partition(arr, low, high, debug);
        quickSort(arr, low, pi - 1, debug);
        quickSort(arr, pi + 1, high, debug);
    }
}

template<typename T>
void quickSort(vector<T>& arr, bool debug = false) {
    if (!arr.empty())
        quickSort(arr, 0, arr.size() - 1, debug);
}

#endif 
