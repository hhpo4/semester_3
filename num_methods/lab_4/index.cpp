#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

using namespace std;

void printMatrix(const vector<vector<double>>& matrix) {
    for (const auto& row : matrix) {
        for (int j = 0; j < row.size() - 1; ++j) {
            cout << setw(8) << fixed << setprecision(2) << row[j];
        }
        cout << " |" << setw(8) << fixed << setprecision(2) << row.back();
        cout << endl;
    }
}

void checkSolution(const vector<vector<double>>& matrix, const vector<double>& roots) {
    cout << "\nПроверка решений:" << endl;
    int n = matrix.size();
    for (int i = 0; i < n; ++i) {
        double left = 0;
        for (int j = 0; j < n; ++j) {
            left += matrix[i][j] * roots[j];
        }
        cout << "Уравнение " << i + 1 << ": " << left << " ≈ " << matrix[i].back() << endl;
    }
}

vector<double> gauss(vector<vector<double>> matrix) {
    int n = matrix.size();

    // Прямой ход
    for (int k = 0; k < n - 1; ++k) {
        if (abs(matrix[k][k]) == 0) {
            cout << "Ошибка: деление на ноль!" << endl;
            return {};
        }

        for (int i = k + 1; i < n; ++i) {
            double factor = matrix[i][k] / matrix[k][k];
            for (int j = k; j <= n; ++j) {
                matrix[i][j] -= factor * matrix[k][j];
            }
        }
    }

    cout << "Матрица после прямого хода:" << endl;
    printMatrix(matrix);

    //Обратный ход
    vector<double> roots(n);
    for (int i = n - 1; i >= 0; --i) {
        double sum = 0;
        for (int j = i + 1; j < n; ++j) {
            sum += matrix[i][j] * roots[j];
        }
        roots[i] = (matrix[i][n] - sum) / matrix[i][i];
    }

    cout << "\nКорни системы:" << endl;
    for (double root : roots) {
        cout << setw(8) << fixed << setprecision(2) << root << " ";
    }
    cout << endl;

    checkSolution(matrix, roots);
    return roots;
}

vector<double> progon(const vector<vector<double>>& matrix) {
    int n = matrix.size();
    vector<double> alpha(n), beta(n), x(n);

    alpha[0] = -matrix[0][1] / matrix[0][0];
    beta[0] = matrix[0][n] / matrix[0][0];

    for (int i = 1; i < n; ++i) {
        double denominator = matrix[i][i] + matrix[i][i - 1] * alpha[i - 1];
        if (abs(denominator) < 1e-10) {
            cout << "Ошибка: делить на 0 нельзя" << endl;
            return {};
        }
        alpha[i] = -(i < n - 1 ? matrix[i][i + 1] : 0) / denominator;
        beta[i] = (matrix[i][n] - matrix[i][i - 1] * beta[i - 1]) / denominator;
    }

    x[n - 1] = beta[n - 1];
    for (int i = n - 2; i >= 0; --i) {
        x[i] = alpha[i] * x[i + 1] + beta[i];
    }

    cout << "\nКорни системы:" << endl;
    for (double root : x) {
        cout << setw(8) << fixed << setprecision(2) << root << " ";
    }
    cout << endl;

    checkSolution(matrix, x);
    return x;
}

void solveSystem(int method) {
    vector<vector<double>> matrix;
    if (method == 1) {
        matrix = {
            {-3, 8, -2, -8, 1, 4, 11},
            {-9, -8, -8, -7, 4, -7, -182},
            {6, 6, 7, -1, 2, -1, 54},
            {1, 1, -8, 3, -2, 3, 14},
            {-8, 4, -2, 0, -4, 2, 36},
            {-7, -2, -7, 8, -2, -1, -12}
        };
    }
    else if (method == 2) {
        matrix = {
            {-3, 2, 0, 0, 0, 0, -14},
            {7, -3, 5, 0, 0, 0, -4},
            {0, -4, 6, 6, 0, 0, -26},
            {0, 0, 8, 2, -6, 0, -22},
            {0, 0, 0, -1, -3, -4, 1},
            {0, 0, 0, 0, -1, 1, 8}
        };
    }

    cout << "Исходная матрица:" << endl;
    printMatrix(matrix);

    if (method == 1) {
        gauss(matrix);
    }
    else if (method == 2) {
        progon(matrix);
    }
}

int main() {
    int choice;
    while (true) {
        cout << "\nВыберите метод для решения системы линейных уравнений:\n" << endl;
        cout << "Введите 1 для решения системы методом Гаусса" << endl;
        cout << "Введите 2 для решения системы методом Прогонки" << endl;
        cout << "Для выхода введите 3" << endl;
        cin >> choice;

        if (choice == 3) {
            break;
        }
        else if (choice == 1 || choice == 2) {
            solveSystem(choice);
        }
        else {
            cout << "Неверный выбор. Введите 1 из предложенных вариантов.\n" << endl;
        }
    }
    return 0;
}