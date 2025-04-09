#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <iomanip> // Для установки точности вывода

using namespace std;

double f(double x) {
    return (exp(pow(x, 2)));
    // return 1;
}

double integrate_right_rectangles(double (*func)(double), double a, double b, int n) {
    double h = (b - a) / n; // Шаг разбиения
    double integral = 0.0;

    for (int i = 1; i <= n; ++i) {
        double xi = a + i * h; // Правая граница i-го отрезка
        integral += func(xi);
    }

    integral *= h; // Умножаем сумму на шаг
    return integral;
}

double integrate_left_rectangles(double (*func)(double), double a, double b, int n) {
    double h = (b - a) / n;
    double integral = 0.0;

    for (int i = 0; i < n; ++i) {
        double xi = a + i * h;
        integral += func(xi);
    }

    return integral * h;
}

double integrate_central_rectangles(double (*func)(double), double a, double b, int n) {
    double h = (b - a) / n;
    double integral = 0.0;

    for (int i = 0; i < n; ++i) {
        double xi = a + (i + 0.5) * h;
        integral += func(xi);
    }

    return integral * h;
}

double integrate_trapezoidal(double (*func)(double), double a, double b, int n) {
    double h = (b - a) / n; 
    double integral = 0.5 * (func(a) + func(b)); // Считаем первое и последнее слагаемые

    for (int i = 1; i < n; ++i) {
        double xi = a + i * h; // Промежуточные точки
        integral += func(xi);
    }

    integral *= h; // Умножаем сумму на шаг
    return integral;
}

double integrate_simpson(double (*func)(double), double a, double b, int n) {
    if (n % 2 != 0) {
        return NAN;
    }

    double h = (b - a) / n; 
    double integral = func(a) + func(b); 

    for (int i = 1; i < n; ++i) {
        double xi = a + i * h; // Промежуточные точки
        if (i % 2 == 1) {
            integral += 4 * func(xi); // Нечетные индексы умножаются на 4
        }
        else {
            integral += 2 * func(xi); // Четные индексы умножаются на 2
        }
    }

    integral *= h / 3; 
    return integral;
}

void estimate_error(double result_N, double result_2N, string method_name) {
    if (isnan(result_N) || isnan(result_2N)) {
        cout << "Невозможно вычислить погрешность для " << method_name << endl;
        return;
    }
    double error = abs(result_2N - result_N);
    cout << "Погрешность (" << method_name << "): " << error << endl;
}

int main() {
    double a = 0.0; 
    double b = 1.0; 
    int N = 11;
    int two_N = 2 * N; 
    double result_analytical = 1.457;

    // Вычисляем интегралы для N и 2N методом правых прямоугольников
    double result_right_rectangles_N = integrate_right_rectangles(f, a, b, N);
    double result_right_rectangles_2N = integrate_right_rectangles(f, a, b, two_N);

    // Вычисляем интегралы для N и 2N методом центральных прямоугольников
    double result_central_rectangles_N = integrate_central_rectangles(f, a, b, N);
    double result_central_rectangles_2N = integrate_central_rectangles(f, a, b, two_N);

    // Вычисляем интегралы для N и 2N методом левых прямоугольников
    double result_left_rectangles_N = integrate_left_rectangles(f, a, b, N);
    double result_left_rectangles_2N = integrate_left_rectangles(f, a, b, two_N);

    // Вычисляем  методом трапеций
    double result_trapezoidal_N = integrate_trapezoidal(f, a, b, N);
    double result_trapezoidal_2N = integrate_trapezoidal(f, a, b, two_N);

    
    double result_simpson_N = integrate_simpson(f, a, b, N);
    double result_simpson_2N = integrate_simpson(f, a, b, two_N);

    cout << fixed << setprecision(15);

    // Результаты для N
    cout << "Вычисление интеграла при N=" << N << ":" << endl;
    cout << "Метод правых прямоугольников: " << result_right_rectangles_N << endl;
    cout << "Метод центральных прямоугольников: " << result_central_rectangles_N << endl;
    cout << "Метод левых прямоугольников: " << result_left_rectangles_N << endl;
    cout << "Метод трапеций: " << result_trapezoidal_N << endl;
    cout << "Метод Симпсона: ";
    if (std::isnan(result_simpson_N)) {
        cout << "N должно быть четным";
    } else {
        cout << result_simpson_N;
    }
    cout << endl << endl;

    // Результаты для 2N
    cout << "Вычисление интеграла при 2N=" << two_N << ":" << endl;
    cout << "Метод правых прямоугольников: " << result_right_rectangles_2N << endl;
    cout << "Метод центральных прямоугольников: " << result_central_rectangles_2N << endl;
    cout << "Метод левых прямоугольников: " << result_left_rectangles_2N << endl;
    cout << "Метод трапеций: " << result_trapezoidal_2N << endl;
    cout << "Метод Симпсона: ";
    if (std::isnan(result_simpson_2N)) {
        cout << "N должно быть четным";
    } else {
        cout << result_simpson_2N;
    }
    cout << endl << endl;

    // Оценка погрешности
    cout << "Разница между N и 2N:" << endl;
    estimate_error(result_right_rectangles_N, result_right_rectangles_2N, "Правые прямоугольники");
    estimate_error(result_central_rectangles_N, result_central_rectangles_2N, "Центральные прямоугольники");
    estimate_error(result_left_rectangles_N, result_left_rectangles_2N, "Левые прямоугольники");
    estimate_error(result_trapezoidal_N, result_trapezoidal_2N, "Трапеции");
    estimate_error(result_simpson_N, result_simpson_2N, "Симпсон");

    cout << endl << "Разница относительно аналитического решения:" << endl;
    estimate_error(result_right_rectangles_N, result_analytical, "Правые прямоугольники");
    estimate_error(result_central_rectangles_N, result_analytical, "Центральные прямоугольники");
    estimate_error(result_left_rectangles_N, result_analytical, "Левые прямоугольники");
    estimate_error(result_trapezoidal_N, result_analytical, "Трапеции");
    estimate_error(result_simpson_N, result_analytical, "Симпсон");
    return 0;
}
