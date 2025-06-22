#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdlib>

using namespace std;

const double PI = 3.14159265358979323846;

// Периодическая функция: прямоугольная волна
double square_wave(double t, double period) {
    t = fmod(t, period);
    return (t < period / 2.0) ? 1.0 : -1.0;
}

// Непериодическая функция: экспонента с гауссовым импульсом
double non_periodic_func(double t) {
    return exp(-t) * sin(4 * PI * t);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <function_type (0=periodic, 1=nonperiodic)>" << endl;
        return 1;
    }
    int function_type = atoi(argv[1]);

    // Параметры сигнала
    double a = 0.0;
    double T = (function_type == 0) ? 2.0 : 5.0;
    double b = a + T;
    int n = 201;
    int m_max = (n - 1) / 2;

    vector<double> t(n);
    vector<double> f(n);
    double dt = T / (n - 1);
    double omega = 2 * PI / T;

    // Дискретизация сигнала
    for (int i = 0; i < n; ++i) {
        t[i] = a + i * dt;
        if (function_type == 0) {
            f[i] = square_wave(t[i], T);
        } else {
            f[i] = non_periodic_func(t[i]);
        }
    }

    // Расчёт коэффициентов Фурье
    vector<double> ak(m_max + 1, 0.0);
    vector<double> bk(m_max + 1, 0.0);

    for (int k = 0; k <= m_max; ++k) {
        for (int i = 0; i < n; ++i) {
            double angle = k * omega * t[i];
            ak[k] += f[i] * cos(angle);
            bk[k] += f[i] * sin(angle);
        }
        ak[k] *= 2.0 / n;
        bk[k] *= 2.0 / n;
    }
    ak[0] /= 2.0;

    // Поиск оптимального числа гармоник
    vector<double> Q(m_max + 1, 0.0);
    vector<double> y_opt(n, 0.0);
    int m_opt = 10;

    for (int m = 1; m <= m_max; ++m) {
        double Q_sum = 0.0;
        vector<double> y(n, ak[0]);

        for (int k = 1; k <= m; ++k) {
            for (int i = 0; i < n; ++i) {
                double angle = k * omega * t[i];
                y[i] += ak[k] * cos(angle) + bk[k] * sin(angle);
            }
        }

        for (int i = 0; i < n; ++i) {
            double diff = y[i] - f[i];
            Q_sum += diff * diff;
        }
        Q[m] = sqrt(Q_sum / n);

        if (m > 1 && abs(Q[m - 1] - Q[m]) / Q[m - 1] < 0.01) {
            m_opt = m;
            y_opt = y;
            break;
        }
        if (m == m_max) {
            m_opt = m_max;
            y_opt = y;
        }
    }

    // Расчёт амплитуд и фаз (до m_max)
    vector<double> Ak(m_max + 1, 0.0);
    vector<double> phi_k(m_max + 1, 0.0);
    vector<double> freq(m_max + 1, 0.0);

    for (int k = 0; k <= m_max; ++k) {
        freq[k] = k * omega;
        if (k == 0) {
            Ak[k] = abs(ak[0]);
            phi_k[k] = 0.0;
        } else {
            Ak[k] = sqrt(ak[k] * ak[k] + bk[k] * bk[k]);
            phi_k[k] = atan2(bk[k], ak[k]);
        }
    }

    // Энергия сигнала
    double Ef = 0.0;
    for (int i = 0; i < n; ++i) {
        Ef += f[i] * f[i];
    }
    Ef *= dt;

    // Сохранение результатов
    string prefix = (function_type == 0) ? "periodic" : "nonperiodic";

    // Исходный и аппроксимированный сигнал
    ofstream fsig(prefix + "_signal.txt");
    for (int i = 0; i < n; ++i) {
        fsig << t[i] << " " << f[i] << " " << y_opt[i] << "\n";
    }
    fsig.close();

    // Ошибка аппроксимации
    ofstream ferr(prefix + "_error.txt");
    for (int i = 0; i < n; ++i) {
        ferr << t[i] << " " << y_opt[i] - f[i] << "\n";
    }
    ferr.close();

    // Амплитудный спектр (до m_max)
    ofstream fspec(prefix + "_spectrum.txt");
    for (int k = 0; k <= m_max; ++k) {
        fspec << freq[k] << " " << Ak[k] << " " << phi_k[k] << "\n";
    }
    fspec.close();

    // Ошибка аппроксимации Q(m)
    ofstream fq(prefix + "_q.txt");
    for (int m = 1; m <= m_max; ++m) {
        fq << m << " " << Q[m] << "\n";
    }
    fq.close();

    // Энергия
    ofstream fe(prefix + "_energy.txt");
    fe << Ef << "\n";
    fe.close();

    return 0;
}