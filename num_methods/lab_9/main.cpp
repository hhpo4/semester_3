#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

using namespace std;

double f(double x) {
    return cosh(x); // ch(x) = (e^x + e^{-x}) / 2
}

int main() {
    const int N = 10; // Количество интервалов
    
    const double a = -1.0, b = 1.0;
    vector<double> x(N + 1), y(N + 1);
    
    // Генерация узлов
    for (int i = 0; i <= N; ++i) {
        x[i] = a + i * (b - a) / N;
        y[i] = f(x[i]);
    }

    // Вычисление шагов
    vector<double> h(N);
    for (int i = 0; i < N; ++i) {
        h[i] = x[i + 1] - x[i];
    }

    // Решение системы для коэффициентов c
    vector<double> c(N + 1, 0); // c[0] не используется, c[1..N]

    // Метод прогонки для трехдиагональной системы
    vector<double> alpha(N + 1), beta(N + 1), gamma(N + 1), rhs(N + 1);

    // Первое уравнение: c[1] = 0 (естественный сплайн)
    c[1] = 0;

    // Уравнения для c[2..N-1]
    for (int i = 2; i < N; ++i) {
        double num = 3 * ((y[i] - y[i-1])/h[i-1] - (y[i-1] - y[i-2])/h[i-2]);
        alpha[i] = h[i-2];
        beta[i] = 2 * (h[i-2] + h[i-1]);
        gamma[i] = h[i-1];
        rhs[i] = num;
    }

    // Уравнение для c[N]
    alpha[N] = h[N-2];
    beta[N] = 2 * (h[N-2] + h[N-1]);
    rhs[N] = 3 * ((y[N] - y[N-1])/h[N-1] - (y[N-1] - y[N-2])/h[N-2]);

    // Прямой ход прогонки
    vector<double> delta(N + 1), lambda(N + 1);
    delta[2] = gamma[2] / beta[2];
    lambda[2] = rhs[2] / beta[2];

    for (int i = 3; i <= N; ++i) {
        delta[i] = gamma[i] / (beta[i] - alpha[i] * delta[i-1]);
        lambda[i] = (rhs[i] - alpha[i] * lambda[i-1]) / (beta[i] - alpha[i] * delta[i-1]);
    }

    // Обратный ход
    c[N] = lambda[N];
    for (int i = N-1; i >= 2; --i) {
        c[i] = lambda[i] - delta[i] * c[i+1];
    }

    // Вычисление коэффициентов a, b, d
    vector<double> a_coeff(N + 1), b_coeff(N + 1), d_coeff(N + 1);
    for (int i = 1; i <= N; ++i) {
        a_coeff[i] = y[i-1];
        if (i < N) {
            d_coeff[i] = (c[i+1] - c[i]) / (3 * h[i-1]);
            b_coeff[i] = (y[i] - y[i-1])/h[i-1] - h[i-1]*(2*c[i] + c[i+1])/3;
        } else {
            d_coeff[i] = -c[i] / (3 * h[i-1]);
            b_coeff[i] = (y[i] - y[i-1])/h[i-1] - (2 * c[i] * h[i-1])/3;
        }
    }

    // Сохранение результатов
    ofstream out("spline_coeffs.txt");
    out << N << endl;
    for (double xi : x) out << xi << " ";
    out << endl;
    for (int i = 1; i <= N; ++i) {
        out << a_coeff[i] << " " << b_coeff[i] << " " << c[i] << " " << d_coeff[i] << endl;
    }
    out.close();

    return 0;
}