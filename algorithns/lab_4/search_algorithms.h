#ifndef SEARCH_ALGORITHMS_H
#define SEARCH_ALGORITHMS_H

#include <vector>
using namespace std;

template<typename T>
int sequentialSearch(const vector<T>& arr, const T& key) {
    for (size_t i = 0; i < arr.size(); i++) {
        if (arr[i] == key)
            return i;
    }
    return -1;
}

template<typename T>
int fibonacciSearch(const vector<T>& arr, const T& key) {
    int n = arr.size();
    int fib2 = 0;
    int fib1 = 1;
    int fib = fib2 + fib1;
    
    while (fib < n) {
        fib2 = fib1;
        fib1 = fib;
        fib = fib2 + fib1;
    }
    
    int offset = -1;
    while (fib > 1) {
        int i = min(offset + fib2, n - 1);
        if (arr[i] < key) {
            fib = fib1;
            fib1 = fib2;
            fib2 = fib - fib1;
            offset = i;
        } else if (arr[i] > key) {
            fib = fib2;
            fib1 -= fib2;
            fib2 = fib - fib1;
        } else {
            return i;
        }
    }
    
    if (fib1 && offset + 1 < n && arr[offset + 1] == key)
        return offset + 1;
    
    return -1;
}

#endif
