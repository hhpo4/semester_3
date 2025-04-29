#include <iostream>
#include <unordered_map>
#include <climits>
using namespace std;

const int PAGE_COUNT = 21;
const int MAX_FRAMES = 7;

int pages[PAGE_COUNT] = {9, 5, 5, 7, 5, 6, 5, 4, 9, 5, 6, 2, 7, 7, 3, 5, 1, 2, 5, 1};

struct Node {
    int data;
    Node* next;
    Node* prev;
};

class Queue {
private:
    Node* front;
    Node* rear;
    int size;
    int capacity;
    
public:
    Queue(int cap) : front(nullptr), rear(nullptr), size(0), capacity(cap) {}
    
    bool isFull() { return size == capacity; }
    bool isEmpty() { return size == 0; }
    
    void enqueue(int page) {
        Node* newNode = new Node{page, nullptr, nullptr};
        if (isEmpty()) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            newNode->prev = rear;
            rear = newNode;
        }
        size++;
    }
    
    int dequeue() {
        if (isEmpty()) return -1;
        int page = front->data;
        Node* temp = front;
        front = front->next;
        if (front) front->prev = nullptr;
        else rear = nullptr;
        delete temp;
        size--;
        return page;
    }
};

int fifo(int frames) {
    Queue q(frames);
    unordered_map<int, bool> inMemory;
    int pageFaults = 0;

    for (int i = 0; i < PAGE_COUNT; i++) {
        int page = pages[i];
        if (!inMemory[page]) {
            pageFaults++;
            if (q.isFull()) {
                int removed = q.dequeue();
                inMemory[removed] = false;
            }
            q.enqueue(page);
            inMemory[page] = true;
        }
    }
    return pageFaults;
}

int opt(int frames) {
    int memory[MAX_FRAMES] = {0};
    int pageFaults = 0;
    int currentSize = 0;

    for (int i = 0; i < PAGE_COUNT; i++) {
        int page = pages[i];
        bool found = false;
        
        for (int j = 0; j < currentSize; j++) {
            if (memory[j] == page) {
                found = true;
                break;
            }
        }
        
        if (!found) {
            pageFaults++;
            if (currentSize < frames) {
                memory[currentSize++] = page;
            } else {
                int farthest = -1, replaceIdx = 0;
                for (int j = 0; j < frames; j++) {
                    int nextUse = INT_MAX;
                    for (int k = i + 1; k < PAGE_COUNT; k++) {
                        if (pages[k] == memory[j]) {
                            nextUse = k;
                            break;
                        }
                    }
                    if (nextUse > farthest) {
                        farthest = nextUse;
                        replaceIdx = j;
                    }
                }
                memory[replaceIdx] = page;
            }
        }
    }
    return pageFaults;
}

int lru(int frames) {
    struct LRUNode {
        int page;
        int lastUsed;
    };
    
    LRUNode memory[MAX_FRAMES] = {0};
    int pageFaults = 0;
    int currentSize = 0;

    for (int i = 0; i < PAGE_COUNT; i++) {
        int page = pages[i];
        bool found = false;
        int foundIdx = -1;
        
        for (int j = 0; j < currentSize; j++) {
            if (memory[j].page == page) {
                found = true;
                foundIdx = j;
                break;
            }
        }
        
        if (found) {
            memory[foundIdx].lastUsed = i;
        } else {
            pageFaults++;
            if (currentSize < frames) {
                memory[currentSize++] = {page, i};} else {
                int lruIdx = 0;
                for (int j = 1; j < frames; j++) {
                    if (memory[j].lastUsed < memory[lruIdx].lastUsed) {
                        lruIdx = j;
                    }
                }
                memory[lruIdx] = {page, i};
            }
        }
    }
    return pageFaults;
}

int main() {
    cout << "кол-во блоков | FIFO | OPT  | LRU  |\n";
    cout << "--------------|------|------|------|\n";
    
    for (int frames = 1; frames <= MAX_FRAMES; frames++) {
        printf("%13d | %4d | %4d | %4d |\n", 
              frames, fifo(frames), opt(frames), lru(frames));
    }

    return 0;
}