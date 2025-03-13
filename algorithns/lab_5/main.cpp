#include <vector>

#include "double-linked-list.h"
int main() {
    std::vector<size_t> sizes = {100000, 500000, 1000000, 5000000, 10000000};
    for (size_t n : sizes) {
        DoublyLinkedList<int> list;
        list.measurePerformance(n);
    }
    std::cout << "Результаты сохранены в performance.txt" << std::endl;

    DoublyLinkedList<int> list;
    for (int i = 0; i < 10; i++) {
        list.insert(i);
    }

    std::cout << "Текущая структура: " << std::endl;
    list.display();

    while (true) {
        std::cout << "Выберите действие: insert (1), remove (2), display (3), print addresses (4), exit (0)" << std::endl;
        int choice;
        std::cin >> choice;

        if (choice == 1) {
            std::cout << "Введите число для вставки: ";
            int num;
            std::cin >> num;
            list.insert(num);
        } else if (choice == 2) {
            std::cout << "Введите число для удаления: ";
            int num;
            std::cin >> num;
            list.remove(num);
        } else if (choice == 3) {
            list.display();
        } else if (choice == 4) {
            list.printAddresses();
        } else if (choice == 0) {
            break;
        }
    }
    return 0;
}
