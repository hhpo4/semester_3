#include <iostream>
#include <list>
#include <algorithm>

struct CustomType {
    int value;
    CustomType(int v) : value(v) {}
    bool operator<(const CustomType& other) const { return value < other.value; }
    bool operator==(const CustomType& other) const { return value == other.value; }
};

std::ostream& operator<<(std::ostream& os, const CustomType& obj) {
    return os << obj.value;
}

int main() {
    std::list<CustomType> lst = {3, 0, -1, 0, 5, -3, 2, 0, 6};

    std::cout << "Исходный список: ";
    for (const auto& el : lst) std::cout << el << " ";
    std::cout << "\n";

    // 2. Заменить все элементы с нулевымс значением на 1
    for (auto& el : lst) {
        if (el.value == 0) {
            el.value = 1;
        }
    }

    std::cout << "После замены 0 на 1: ";
    for (const auto& el : lst) std::cout << el << " ";
    std::cout << "\n";

    // 3. Удалить по два элемента из начала и конца
    for (int i = 0; i < 2 && !lst.empty(); ++i) lst.pop_front();
    for (int i = 0; i < 2 && !lst.empty(); ++i) lst.pop_back();

    std::cout << "После удаления по 2 элемента с начала и конца: ";
    for (const auto& el : lst) std::cout << el << " ";
    std::cout << "\n";

    // 4. Сортировка по возрастанию
    lst.sort();

    std::cout << "После сортировки по возрастанию: ";
    for (const auto& el : lst) std::cout << el << " ";
    std::cout << "\n";

    // 5. Подсчёт количества отрицательных элементов
    int negativeCount = std::count_if(lst.begin(), lst.end(), [](const CustomType& el) {
        return el.value < 0;
    });
    std::cout << "Количество отрицательных элементов: " << negativeCount << "\n";

    // 6. К каждому элементу добавить 1
    for (auto& el : lst) {
        el.value += 1;
    }

    std::cout << "После увеличения каждого элемента на 1: ";
    for (const auto& el : lst) std::cout << el << " ";
    std::cout << "\n";

    return 0;
}
