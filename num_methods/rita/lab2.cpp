#include <iostream>
#include <cmath>
using namespace std;

// Функция f(x)
double f(double x) {
    return (pow(x, 5.0)- 3*(pow(x, 2.0))+1);
}

// Первая производная f'(x)
double f_prime(double x) {
    return 5*((pow(x, 5.0)) - 6 * x);
}

// Вторая производная f''(x)
double f_double_prime(double x) {
    return (20*(pow(x, 2.0))-6);
}

// Метод Ньютона
double newton(double x0, const double eps) {
    // Проверяем условие сходимости 
    double f_x = f(x0);
    double f_px = f_prime(x0);
    double f_ppx = f_double_prime(x0);

    if (fabs(f_x * f_ppx) >= pow(f_px, 2)) {
        cout << "Внимание! Данное начальное приближение не соответствует условию сходимости метода Ньютона." << endl;
        cout << "Сходимость возможна лишь в пределах некоторой окрестности корня. Ввидите другие значения \n" << endl;
    }
    
    double x1 = x0;               // Начальное приближение
    int iterations = 0;           // Счетчик итераций

    for (int i = 0; i < 1000; ++i) {
        f_x = f(x1);
        f_px = f_prime(x1);

        // Вычисляем новое приближение
        double x_new = x1 - f_x / f_px;
        iterations++;

        // Условие завершения
        if (fabs(x_new - x1) < eps) {
            cout << "\nМетод Ньютона: Количество итераций = " << iterations << endl;
            return x_new;
        }

        x1 = x_new;
    }

    cout << "Метод Ньютона не завершил процесс сходимости за указанное количество итераций." << endl;
    return NAN;
}

// Метод дихотомии
double dichotomy(double x0, double x1, const double eps) {
    if (f(x0) * f(x1) >= 0) {
        cout << "На данном интервале нельзя найти корень." << endl;
        return NAN;
    }
    double x2;                  // Середина отрезка
    int iterations = 0;         // Счетчик итераций
    while (fabs(x1 - x0) > eps) {
        x2 = 0.5 * (x0 + x1);    // Находим середину
        if (f(x0) * f(x2) < 0) {
            x1 = x2;             // Корень лежит cлева
        }
        else {
            x0 = x2;             // Корень лежит справа
        }
        iterations++;
    }
    cout << "\nМетод дихотомии: Количество итераций = " << iterations << endl;
    return 0.5 * (x0 + x1);
}


int main() {
    // setlocale(LC_ALL, "Russian");
    // Для дихотомии
    const double x0_d = -1;
    const double x1_d = -0.5;
    const double eps = 0.000001;

    // Вычисление методом дихотомии
    double x_d = dichotomy(x0_d, x1_d, eps);
    if (!isnan(x_d)) {
        cout << "Корень уравнения для метода Дихотомии: " << x_d << endl;
    }

    cout << "\nВведите начальное приближение для метода Ньютона (x0): ";
    double x0_n;
    cin >> x0_n;

    // Вычисление методом Ньютона
    double x_n = newton(x0_n, eps);
    if (!isnan(x_n)) {
        cout << "Корень уравнения для метода Ньютона: " << x_n << endl;
    }

    return 0;
}