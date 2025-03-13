#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <limits> // Для std::numeric_limits

using namespace std;

double f(double x) {
    if (fabs(x) < 1e-10) {
        return numeric_limits<double>::infinity(); 
    }
    return (1 + pow(x, 2)) / (2 * sqrt(1 + 2 * pow(x, 2)));
}

double f_prime(double x) {
    if (fabs(x) < 1e-10) {
        return numeric_limits<double>::infinity();
    }
    return ((x / sqrt(1 + 2 * pow(x, 2))) - (x * (pow(x, 2) + 1)) / pow(2 * pow(x, 2) + 1, 1.5));
}

double f_double_prime(double x) {
    return ((3 * pow(x, 2)) / (sqrt(2 * pow(x, 2) + 1) * pow(2 * pow(x, 2) + 1, 2)));
}

double f_triple_prime(double x) {
    return - ((18 * pow(x, 3) - 6*x) / (sqrt(2 * pow(x, 2) + 1) * pow(2 * pow(x, 2) + 1, 3)));
}

double dr1(double x, double h) {
    return (f(x + h) - f(x)) / h;
}

double dl1(double x, double h) {
    return (f(x) - f(x - h)) / h;
}

double dc1(double x, double h) {
    return (f(x + h) - f(x - h)) / (2 * h);
}

double dr2(double (*f)(double), double x, double h) {
    return (f(x + 2 * h) - 2 * f(x + h) + f(x)) / (h * h);
}

double dl2(double (*f)(double), double x, double h) {
    return (f(x) - 2 * f(x - h) + f(x - 2 * h)) / (h * h);
}

double dc2(double (*f)(double), double x, double h) {
    return (f(x + h) + f(x - h) - 2 * f(x)) / (h * h);
}

double dr3(double (*f)(double), double x, double h) {
    return (f(x + 3 * h) - 3 * f(x + 2 * h) + 3 * f(x + h) - f(x)) / (h * h * h);
}

double dl3(double (*f)(double), double x, double h) {
    return (-f(x - 3 * h) + 3 * f(x - 2 * h) - 3 * f(x - h) + f(x)) / (h * h * h);
}

double dc3(double (*f)(double), double x, double h) {
    return (f(x + 2 * h) - 2 * f(x + h) + 2 * f(x - h) - f(x - 2 * h)) / (2 * h * h * h);
}

int main() {
    int N;
    double a, b, h;
    cout << "Enter the number of nodes N: ";
    cin >> N;
    a = -4.0;
    b = -2.5;
    h = (b - a) / N;
    if (h <= 0) {
        cerr << "Ошибка: шаг h должен быть положительным!" << endl;
        return 1;
    }
    int c;
    cout << "What is the derivative of what order? (1 - 1st, 2 - 2nd, 3 - 3rd): ";
    cin >> c;
    ofstream output_file("data.txt");
    if (!output_file) {
        cerr << "Ошибка при открытии файла!" << endl;
        return 1;
    }
    output_file << fixed << setprecision(5);

    if (c == 1) {
        output_file << "x\tf'(x) левая\tf'(x) центральная\tf'(x) правая\tf'(x) аналитическая" << endl;
        for (double x = a; x <= b; x += h) {
            if (fabs(x) < 1e-10) continue;
            output_file << setw(10) << x << setw(25) << dl1(x, h) << setw(25) << dc1(x, h) << setw(25) << dr1(x, h) << setw(25) << f_prime(x) << endl;
        }
    } else if (c == 2) {
        output_file << "x\tf''(x) левая\tf''(x) центральная\tf''(x) правая\tf''(x) аналитическая" << endl;
        for (double x = a; x <= b; x += h) {
            if (fabs(x) < 1e-10) continue;
            output_file << setw(10) << x << setw(25) << dl2(f, x, h) << setw(25) << dc2(f, x, h) << setw(25) << dr2(f, x, h) << setw(25) << f_double_prime(x) << endl;
        }
    } else if (c == 3) {
        output_file << "x\tf'''(x) левая\tf'''(x) центральная\tf'''(x) правая\tf'''(x) аналитическая" << endl;
        for (double x = a; x <= b; x += h) {
            if (fabs(x) < 1e-10) continue;
            output_file << setw(10) << x << setw(25) << dl3(f, x, h) << setw(25) << dc3(f, x, h) << setw(25) << dr3(f, x, h) << setw(25) << f_triple_prime(x) << endl;
        }
    }

    output_file.close();
    cout << "Результаты записаны в data.txt" << endl;
    return 0;
}
