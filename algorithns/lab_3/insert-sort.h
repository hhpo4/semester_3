#ifndef INSERT_SORT_H
#define INSERT_SORT_H

#include <vector>
#include <iostream>
#include <algorithm>
#include "utils.h"
using namespace std;


template<typename T>
void insertSort(vector<T>& arr, bool debug = false) {
    for (size_t i = 1; i < arr.size(); i++) {
        T key = arr[i]; // Текущий элемент, который нужно вставить в отсортированную часть
        int j = i - 1;

        // Сдвигаем элементы большие, чем key, вправо
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key; // Вставляем key на правильное место

        if (debug) {
            cout << "После итерации " << i << ": ";
            printArray(arr);
        }
    }
}

#endif