#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <iostream>
using namespace std;

template<typename T>
void printArray(const vector<T>& arr) {
    for (const auto& elem : arr) {
        cout << elem << " ";
    }
    cout << endl;
}

#endif
