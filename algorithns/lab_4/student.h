#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>
#include <string>
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

    bool operator>(const Student& other) const {
        return age > other.age;
    }

    bool operator==(const Student& other) const {
        return (name == other.name && age == other.age && averageScore == other.averageScore);
    }

    friend ostream& operator<<(ostream& os, const Student& s) {
        os << s.name << " (Возраст: " << s.age << ", Средний балл: " << s.averageScore << ")";
        return os;
    }
};

#endif
