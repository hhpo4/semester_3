#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <algorithm>

using namespace std;

double determinant(double mat[3][3]) {
    return mat[0][0] * (mat[1][1]*mat[2][2] - mat[2][1]*mat[1][2]) -
           mat[0][1] * (mat[1][0]*mat[2][2] - mat[2][0]*mat[1][2]) +
           mat[0][2] * (mat[1][0]*mat[2][1] - mat[2][0]*mat[1][1]);
}

bool inverseMatrix(double mat[3][3], double inv[3][3]) {
    double det = determinant(mat);
    if (det == 0) return false;
    
    inv[0][0] = (mat[1][1]*mat[2][2] - mat[1][2]*mat[2][1]) / det;
    inv[0][1] = (mat[0][2]*mat[2][1] - mat[0][1]*mat[2][2]) / det;
    inv[0][2] = (mat[0][1]*mat[1][2] - mat[0][2]*mat[1][1]) / det;
    
    inv[1][0] = (mat[1][2]*mat[2][0] - mat[1][0]*mat[2][2]) / det;
    inv[1][1] = (mat[0][0]*mat[2][2] - mat[0][2]*mat[2][0]) / det;
    inv[1][2] = (mat[0][2]*mat[1][0] - mat[0][0]*mat[1][2]) / det;
    
    inv[2][0] = (mat[1][0]*mat[2][1] - mat[1][1]*mat[2][0]) / det;
    inv[2][1] = (mat[0][1]*mat[2][0] - mat[0][0]*mat[2][1]) / det;
    inv[2][2] = (mat[0][0]*mat[1][1] - mat[0][1]*mat[1][0]) / det;
    
    return true;
}

void printMatrix(double mat[3][3], const string& name) {
    cout << "Матрица " << name << ":\n";
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            cout << setw(12) << mat[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

void printVector(double vec[3], const string& name) {
    cout << "Вектор " << name << ":\n";
    for (int i = 0; i < 3; ++i) {
        cout << setw(12) << vec[i] << "\n";
    }
    cout << "\n";
}

void printSystem(double C[3][3], double F[3]) {
    cout << "Система уравнений:\n";
    cout << C[0][0] << "·a1 + " << C[0][1] << "·a2 + " << C[0][2] << "·a3 = " << F[0] << "\n";
    cout << C[1][0] << "·a1 + " << C[1][1] << "·a2 + " << C[1][2] << "·a3 = " << F[1] << "\n";
    cout << C[2][0] << "·a1 + " << C[2][1] << "·a2 + " << C[2][2] << "·a3 = " << F[2] << "\n\n";
}

double calculateAvgEpsilon(const vector<double>& eps) {
    double sum = 0;
    for (double e : eps) {
        sum += abs(e);
    }
    return sum / eps.size();
}

void processData(const vector<double>& x, const vector<double>& y, const string& suffix) {
    // Построение матрицы C и вектора F
    double C[3][3] = {0}, F[3] = {0};
    for (size_t i = 0; i < x.size(); ++i) {
        double xi = x[i];
        double f1 = sqrt(xi);
        double f2 = xi;
        double f3 = log(xi);
        
        C[0][0] += f1*f1; C[0][1] += f1*f2; C[0][2] += f1*f3;
        C[1][1] += f2*f2; C[1][2] += f2*f3;
        C[2][2] += f3*f3;
        
        F[0] += f1 * y[i];
        F[1] += f2 * y[i];
        F[2] += f3 * y[i];
    }
    C[1][0] = C[0][1]; C[2][0] = C[0][2]; C[2][1] = C[1][2];

    cout << "=== Результаты для " << suffix << " ===\n";
    printMatrix(C, "C " + suffix);
    printVector(F, "F " + suffix);
    printSystem(C, F);

    // Обращение матрицы C
    double invC[3][3];
    if (!inverseMatrix(C, invC)) {
        cerr << "Матрица вырождена!" << endl;
        return;
    }
    printMatrix(invC, "C^{-1} " + suffix);

    // Вычисление коэффициентов a = C^{-1} * F
    double a[3] = {0};
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            a[i] += invC[i][j] * F[j];
        }
    }

    // Вычисление Rmin и стандартных ошибок
    double Rmin = 0;
    vector<double> eps(x.size());
    for (size_t i = 0; i < x.size(); ++i) {
        double y_pred = a[0]*sqrt(x[i]) + a[1]*x[i] + a[2]*log(x[i]);
        eps[i] = y[i] - y_pred;
        Rmin += pow(eps[i], 2);
    }
    double n = x.size();
    double p = 3;
    double sigma2 = Rmin / (n - p);

    // Стандартные ошибки коэффициентов
    double D[3];
    for (int k = 0; k < 3; ++k) {
        D[k] = sqrt(sigma2 * invC[k][k]);
    }

    // Доверительные интервалы (95%, t=1.96)
    const double t = 1.96;
    double conf_low[3], conf_high[3];
    for (int k = 0; k < 3; ++k) {
        conf_low[k] = a[k] - t * D[k];
        conf_high[k] = a[k] + t * D[k];
    }

    // Вывод результатов
    cout << fixed << setprecision(6);
    cout << "Коэффициенты " << suffix << ":\n";
    cout << "a1 = " << a[0] << " ± " << D[0] << "  Дов. интервал: [" << conf_low[0] << ", " << conf_high[0] << "]\n";
    cout << "a2 = " << a[1] << " ± " << D[1] << "  Дов. интервал: [" << conf_low[1] << ", " << conf_high[1] << "]\n";
    cout << "a3 = " << a[2] << " ± " << D[2] << "  Дов. интервал: [" << conf_low[2] << ", " << conf_high[2] << "]\n\n";

    cout << "Остаточная сумма квадратов Rmin = " << Rmin << "\n";
    cout << "Оценка дисперсии ошибок σ² = " << sigma2 << "\n";
    cout << "Среднее |ε| = " << calculateAvgEpsilon(eps) << "\n\n";

    // Сохранение данных для графика
    ofstream data_file("data_" + suffix + ".txt");
    for (size_t i = 0; i < x.size(); ++i) {
        data_file << x[i] << " " << y[i] << " " << a[0]*sqrt(x[i]) + a[1]*x[i] + a[2]*log(x[i]) << "\n";
    }
    data_file.close();
}

int main() {
    vector<double> x_full = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
    vector<double> y_full = {-2.28,-2.83,-1.45,-1.40,-2.33,0.09,-1.96,-2.28,-0.36,0.71,
                            -0.80,0.26,-0.06,-0.29,1.26,0.68,0.19,1.04,0.60,1.94};

    // каждая вторая
    vector<double> x_half, y_half;
    for (size_t i = 0; i < x_full.size(); i += 2) {
        x_half.push_back(x_full[i]);
        y_half.push_back(y_full[i]);
    }

    processData(x_full, y_full, "full");
    processData(x_half, y_half, "half");

    return 0;
}