#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <functional>
#include <random>

#include "quick-sort.h"
#include "insert-sort.h"
#include "bubble-sort.h"

using namespace std;

class Student {
public:
    string name;
    int age;
    double averageScore;

    Student(string n, int a, double avg) : name(n), age(a), averageScore(avg) {}

    bool operator<(const Student& other) const {
        return age < other.age;
    }

    friend ostream& operator<<(ostream& os, const Student& s) {
        os << s.name << " (Возраст: " << s.age << ", Средний балл: " << s.averageScore << ")";
        return os;
    }
};


template<typename T>
double measureTime(vector<T>& arr, string type) {
    function<void(vector<T>&, bool)> sortFunction;

    if (type == "insert") {
        sortFunction = static_cast<void(*)(vector<T>&, bool)>(insertSort);
    } else if (type == "bubble") {
        sortFunction = static_cast<void(*)(vector<T>&, bool)>(bubbleSort);
    } else if (type == "quick") {
        sortFunction = static_cast<void(*)(vector<T>&, bool)>(quickSort);
    } else {
        throw invalid_argument("Unknown sort type");
    }

    auto start = chrono::high_resolution_clock::now();
    sortFunction(arr, false);
    auto end = chrono::high_resolution_clock::now();
    
    chrono::duration<double> duration = end - start;
    return duration.count();
}

vector<int> generateRandomArray(int size) {
    vector<int> arr(size);
    generate(arr.begin(), arr.end(), []() { return rand() % 1000; }); //лямбда функция, начало и конец вектора
    return arr;
}

vector<int> generateReversedArray(int size) {
    vector<int> arr(size);
    for (int i = 0; i < size; i++) {
        arr[i] = size - i;
    }
    return arr;
}

vector<int> generateRandomArrayWithMedianAsLast(int n) {
    vector<int> arr = generateRandomArray(n - 1);
    
    vector<int> temp = arr;
    sort(temp.begin(), temp.end());
    int median = temp[(int) (temp.size() / 2)];
    
    arr.push_back(median);
    return arr;
}


int main() {
    vector<int> sizes = {10000, 15000, 20000, 25000,30000};
    ofstream resultsFile("execution_times.txt");

    string currentSortType;
    cout << "Введите тип сортировки (quick, insert, bubble): ";
    cin >> currentSortType;
    resultsFile << currentSortType << endl;

    for (int size : sizes) {
        cout << "Тестирование для массива размером " << size << ":\n";

        vector<int> test = {1, 2, 6, 3, 4, 5};
        vector<int> randomArr = generateRandomArray(size);
        vector<int> reversedArr = generateReversedArray(size);
        vector<int> arrayWithMedianAsLast = generateRandomArrayWithMedianAsLast(size);

        double randomTime, reversedTime, nearlySortedTime;

        if (currentSortType == "quick") {
            randomTime = measureTime(randomArr, "quick");
            reversedTime  = measureTime(reversedArr, "quick");
            nearlySortedTime = measureTime(arrayWithMedianAsLast, "quick");
        } else if (currentSortType == "insert") {
            randomTime = measureTime(randomArr, "insert");
            reversedTime = measureTime(reversedArr, "insert");
            nearlySortedTime = measureTime(arrayWithMedianAsLast, "insert");
        } else if (currentSortType == "bubble") {
            randomTime = measureTime(randomArr, "bubble");
            reversedTime = measureTime(reversedArr, "bubble");
            nearlySortedTime = measureTime(arrayWithMedianAsLast, "bubble");
        }

        cout << "Время выполнения для случайного массива: " << randomTime << " секунд\n";
        cout << "Время выполнения для обратного массива: " << reversedTime << " секунд\n";
        cout << "Время выполнения для массива с опорным элементом, равным медиане: " << nearlySortedTime << " секунд\n";

        resultsFile << size << "," << randomTime << "," << reversedTime << "," << nearlySortedTime << "\n";
    }

    resultsFile.close();
    cout << endl << "Результаты сохранены в файл execution_times.txt\n";

    return 0;
}