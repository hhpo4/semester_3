#ifndef QUICK_SORT_H
#define QUICK_SORT_H

#include <vector>
#include <iostream>
#include <algorithm>
#include "utils.h"
using namespace std;


template<typename T>
int partition(vector<T>& arr, int low, int high, bool debug) {
    T pivot = arr[high]; //опорный - последний 
    int i = low - 1; // Индекс меньшего элемента

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]); // Ставим опорный элемент на правильное место

    if (debug) {
        cout << "Опорный элемент: " << pivot << endl;
        cout << "После разделения: ";
        printArray(arr);
    }

    return i + 1; // Возвращаем индекс опорного элемента
}

template<typename T>
void quickSort(vector<T>& arr, int low, int high, bool debug) {
    if (low < high) {
        int pi = partition(arr, low, high, debug);

        quickSort(arr, low, pi - 1, debug);
        quickSort(arr, pi + 1, high, debug);
    }
}

template<typename T>
void quickSort(vector<T>& arr, bool debug = false) {
    quickSort(arr, 0, arr.size() - 1, debug);
}

#endif