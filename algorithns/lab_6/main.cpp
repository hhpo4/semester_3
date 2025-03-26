#include <iostream>
#include <deque>
#include <algorithm>
#include <numeric>

// Пользовательский тип
struct CustomType {
    int value;
    CustomType(int v) : value(v) {}
    bool operator<(const CustomType& other) const { return value < other.value; }
    bool operator>(const CustomType& other) const { return value > other.value; }
    bool operator==(const CustomType& other) const { return value == other.value; }
};

std::ostream& operator<<(std::ostream& os, const CustomType& obj) {
    return os << obj.value;
}

int main() {
    std::deque<CustomType> dq = { 3, 7, 1, 9, 2, 8, 4 };

    // Вывод исходного контейнера
    std::cout << "Исходный контейнер: ";
    for (const auto& el : dq) std::cout << el << " ";
    std::cout << "\n";

    // Заменить максимальный элемент минимальным
    auto min_it = std::min_element(dq.begin(), dq.end());
    auto max_it = std::max_element(dq.begin(), dq.end());
    if (min_it != dq.end() && max_it != dq.end()) {
        std::replace(dq.begin(), dq.end(), *max_it, *min_it);
    }

    std::cout << "После замены макс. элемента мин.: ";
    for (const auto& el : dq) std::cout << el << " ";
    std::cout << "\n";

    // Удалить все элементы, меньшие среднего арифметического
    double avg = std::accumulate(dq.begin(), dq.end(), 0.0, [](double sum, const CustomType& el) { return sum + el.value; }) / dq.size();
    dq.erase(std::remove_if(dq.begin(), dq.end(), [avg](const CustomType& el) { return el.value < avg; }), dq.end());

    std::cout << "После удаления элементов < " << avg << ": ";
    for (const auto& el : dq) std::cout << el << " ";
    std::cout << "\n";

    // Сортировать по убыванию
    std::sort(dq.begin(), dq.end(), std::greater<>());
    std::cout << "После сортировки по убыванию: ";
    for (const auto& el : dq) std::cout << el << " ";
    std::cout << "\n";

    // Найти медиану
    if (!dq.empty()) {
        CustomType median = dq[dq.size() / 2];
        std::cout << "Медиана: " << median << "\n";
    }

    // Изменить знак каждого элемента
    std::transform(dq.begin(), dq.end(), dq.begin(), [](CustomType& el) { return CustomType(-el.value); });
    std::cout << "После изменения знака: ";
    for (const auto& el : dq) std::cout << el << " ";
    std::cout << "\n";
    
    return 0;
}
