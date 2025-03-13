#include <iostream>
#include <chrono>
#include <fstream>

template <typename T>
struct TNode {
    T data;
    TNode<T>* next;
    TNode<T>* prev;
    
    TNode(T value) : data(value), next(nullptr), prev(nullptr) {}
};

template <typename T>
class DoublyLinkedList {
private:
    TNode<T>* head;
    TNode<T>* tail;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr) {}

    void insert(T value) {
        TNode<T>* newNode = new TNode<T>(value);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    void remove(T value) {
        TNode<T>* current = head;
        while (current) {
            if (current->data == value) {
                if (current->prev) current->prev->next = current->next;
                if (current->next) current->next->prev = current->prev;
                if (current == head) head = current->next;
                if (current == tail) tail = current->prev;
                delete current;
                return;
            }
            current = current->next;
        }
    }

    void display() {
        TNode<T>* current = head;
        while (current) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }

    void printAddresses() {
        TNode<T>* current = head;
        while (current) {
            std::cout << "Value: " << current->data << " Address: " << current << std::endl;
            current = current->next;
        }
    }


    static void logPerformance(size_t n, double insertTime, double removeTime) {
        std::ofstream file("performance.txt", std::ios::app);
        if (file.is_open()) {
            file << n << " " << insertTime << " " << removeTime << "\n";
            file.close();
        }
    }

    void measurePerformance(size_t n) {
        for (size_t i = 1; i <= n; ++i) {
            insert(i);
        }
        
        auto start = std::chrono::high_resolution_clock::now();
        insert(n + 1);
        auto end = std::chrono::high_resolution_clock::now();
        double insertTime = std::chrono::duration<double>(end - start).count();
        
        start = std::chrono::high_resolution_clock::now();
        remove(n + 1);
        end = std::chrono::high_resolution_clock::now();
        double removeTime = std::chrono::duration<double>(end - start).count();
        
        logPerformance(n, insertTime, removeTime);
    }

    ~DoublyLinkedList() {
        TNode<T>* current = head;
        while (current) {
            TNode<T>* next = current->next;
            delete current;
            current = next;
        }
    }
};