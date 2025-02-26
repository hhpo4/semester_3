#ifndef BUBBLE_SORT_H
#define BUBBLE_SORT_H

#include <vector>
#include <iostream>
#include <algorithm>
#include "utils.h"
using namespace std;

template<typename T>
void bubbleSort(vector<T>& arr, bool debug = false) {
    size_t n = arr.size();
    bool swapped;
    for (size_t i = 0; i < n - 1; i++) {
        swapped = false;
        for (size_t j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (debug) {
            cout << "После итерации " << i + 1 << ": ";
            printArray(arr);
        }
        if (!swapped) break; // Если обменов не было, массив уже отсортирован
    }
}

#endif
