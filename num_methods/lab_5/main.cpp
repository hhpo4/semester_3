#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

using namespace std;

// Основная функция системы дифференциальных уравнений
vector<double> f(double x, const vector<double>& F) {
    double y = F[0];  // Текущее значение y
    double z = F[1];  // Текущее значение z

    vector<double> derivatives(2);
    derivatives[0] = exp(-(y*y + z*z)) + 3.5*x;  
    derivatives[1] = 4.5*y*y + z;  

    return derivatives;
}

vector<double> f_test(double x, const vector<double>& F) {
    double y = F[0];  // Текущее значение y
    double z = F[1];  // Текущее значение z

    vector<double> derivatives(2);
    derivatives[0] = z - cos(x);  
    derivatives[1] = y + sin(x);  

    return derivatives;
}

// Метод Рунге-Кутты 2-го порядка
void rk_2nd(double x0, double xf, double h, vector<double>& F0, 
            const string& filename, bool use_test_function) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка открытия файла '" << filename << "'" << endl;
        return;
    }

    const double alpha = 0.5;
    double x = x0;
    int steps = static_cast<int>((xf - x0) / h);

    for (int i = 0; i < steps; ++i) {
        // Вычисляем коэффициенты
        vector<double> k1 = use_test_function ? f_test(x, F0) : f(x, F0);

        vector<double> F_mid(2);
        F_mid[0] = F0[0] + (h / (2 * alpha)) * k1[0];
        F_mid[1] = F0[1] + (h / (2 * alpha)) * k1[1];
        vector<double> k2 = use_test_function ? f_test(x + (h / (2 * alpha)), F_mid) 
                                            : f(x + (h / (2 * alpha)), F_mid);

        // Обновляем решение
        F0[0] += h * ((1 - alpha) * k1[0] + alpha * k2[0]);
        F0[1] += h * ((1 - alpha) * k1[1] + alpha * k2[1]);
        x += h;

        file << x << " " << F0[0] << " " << F0[1] << endl;
        cout << "Шаг " << i + 1 << ": x = " << x
            << ", y = " << F0[0] << ", z = " << F0[1] << endl;
    }

    file.close();
}

// Метод Рунге-Кутты 4-го порядка
void rk_4th(double x0, double xf, double h, vector<double>& F0, 
            const string& filename, bool use_test_function) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка открытия файла '" << filename << "'" << endl;
        return;
    }

    double x = x0;
    int steps = static_cast<int>((xf - x0) / h);

    for (int i = 0; i < steps; ++i) {
        // Вычисляем 4 коэффициента
        vector<double> K1 = use_test_function ? f_test(x, F0) : f(x, F0);

        vector<double> F_temp(2);
        F_temp[0] = F0[0] + h / 2 * K1[0];
        F_temp[1] = F0[1] + h / 2 * K1[1];
        vector<double> K2 = use_test_function ? f_test(x + h / 2, F_temp) 
                                             : f(x + h / 2, F_temp);

        F_temp[0] = F0[0] + h / 2 * K2[0];
        F_temp[1] = F0[1] + h / 2 * K2[1];
        vector<double> K3 = use_test_function ? f_test(x + h / 2, F_temp) 
                                            : f(x + h / 2, F_temp);

        F_temp[0] = F0[0] + h * K3[0];
        F_temp[1] = F0[1] + h * K3[1];
        vector<double> K4 = use_test_function ? f_test(x + h, F_temp) 
                                             : f(x + h, F_temp);

        // Обновляем решение
        F0[0] += h / 6 * (K1[0] + 2 * K2[0] + 2 * K3[0] + K4[0]);
        F0[1] += h / 6 * (K1[1] + 2 * K2[1] + 2 * K3[1] + K4[1]);
        x += h;

        file << x << " " << F0[0] << " " << F0[1] << endl;
        cout << "Шаг " << i + 1 << ": x = " << x
            << ", y = " << F0[0] << ", z = " << F0[1] << endl;
    }

    file.close();
}

int main() {
    const double x0 = 0.0;
    const double xf = 1.0;
    const double x0_test = 0.0;  
    const double xf_test = 0.5; 
    const double h = 0.2;  

    vector<double> initial_F = {0.5, 1.0};  // Начальные условия для основной функции
    vector<double> initial_F_test = {0.0, 0.0};  // Начальные условия для тестовой функции

    while (true) {
        cout << "\nВыберите режим работы:\n"
             << "1. Основная функция\n"
             << "2. Тестовая функция\n"
             << "3. Выход\n"
             << "Ваш выбор: ";
        
        int mode_choice;
        cin >> mode_choice;

        if (mode_choice == 3) return 0;
        if (mode_choice != 1 && mode_choice != 2) {
            cout << "Неверный выбор!" << endl;
            continue;
        }

        bool use_test_function = (mode_choice == 2);
        double current_x0 = use_test_function ? x0_test : x0;
        double current_xf = use_test_function ? xf_test : xf;
        vector<double> current_F = use_test_function ? initial_F_test : initial_F;

        cout << "\nВыберите метод решения:\n"
            << "1. Рунге-Кутта 2-го порядка\n"
            << "2. Рунге-Кутта 4-го порядка\n"
            << "3. Вернуться к выбору функции\n"
            << "Ваш выбор: ";

        int method_choice;
        cin >> method_choice;

        vector<double> F = current_F;  // Копируем начальные условия

        switch (method_choice) {
        case 1:
            rk_2nd(current_x0, current_xf, h, F, 
                use_test_function ? "rk2_test_results.txt" : "rk2_results.txt", 
                use_test_function);
            cout << "Результаты сохранены в " 
                << (use_test_function ? "rk2_test_results.txt" : "rk2_results.txt") 
                << endl;
            break;
        case 2:
            rk_4th(current_x0, current_xf, h, F, 
                use_test_function ? "rk4_test_results.txt" : "rk4_results.txt", 
                use_test_function);
            cout << "Результаты сохранены в " 
                << (use_test_function ? "rk4_test_results.txt" : "rk4_results.txt") 
                << endl;
            break;
        case 3:
            continue;
        default:
            cout << "Неверный выбор!" << endl;
        }
    }
}

