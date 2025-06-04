#include <iostream>
#include <cmath>
#include <vector>
#include <functional>
#include <limits>

using namespace std;

const double EPS = 1e-6;
const double STEP_SIZE = 1e-4;
const double GRAD_STEP = 1e-6;

// Функция для лемнискаты Бернулли
void lemniscate(double theta, double &x, double &y) {
    double r_squared = cos(2*theta);
    if (r_squared < 0) r_squared = 0; // Обработка областей, где cos(2ϕ) < 0
    double r = sqrt(r_squared);
    x = r * cos(theta);
    y = r * sin(theta);
}

// Функция для второй кривой: xy² = 4(2 - y)
double curve2_x(double y) {
    return 4.0 * (2.0 - y) / (y * y);
}

// Целевая функция - квадрат расстояния между точками
double target_function(double theta, double y2) {
    double x1, y1;
    lemniscate(theta, x1, y1);
    
    double x2 = curve2_x(y2);
    
    return (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2);
}

// Численное вычисление градиента
vector<double> numerical_gradient(double theta, double y2) {
    vector<double> grad(2);
    
    // Производная по theta
    double f_theta_plus = target_function(theta + GRAD_STEP, y2);
    double f_theta_minus = target_function(theta - GRAD_STEP, y2);
    grad[0] = (f_theta_plus - f_theta_minus) / (2 * GRAD_STEP);
    
    // Производная по y2
    double f_y2_plus = target_function(theta, y2 + GRAD_STEP);
    double f_y2_minus = target_function(theta, y2 - GRAD_STEP);
    grad[1] = (f_y2_plus - f_y2_minus) / (2 * GRAD_STEP);
    
    return grad;
}

// Метод наискорейшего спуска
vector<double> steepest_descent(double theta_init, double y2_init, int max_iter = 1000) {
    vector<double> current = {theta_init, y2_init};
    
    for (int i = 0; i < max_iter; ++i) {
        auto grad = numerical_gradient(current[0], current[1]);
        
        // Проверка на сходимость
        double grad_norm = sqrt(grad[0]*grad[0] + grad[1]*grad[1]);
        if (grad_norm < EPS) {
            cout << "Сходимость достигнута на итерации " << i << endl;
            break;
        }
        
        // Направление антиградиента
        vector<double> direction = {-grad[0], -grad[1]};
        
        // Одномерная минимизация вдоль направления
        double t = STEP_SIZE;
        double f_current = target_function(current[0], current[1]);
        double f_new = target_function(current[0] + t*direction[0], 
                                      current[1] + t*direction[1]);
        
        // Простой линейный поиск
        while (f_new < f_current) {
            f_current = f_new;
            t += STEP_SIZE;
            f_new = target_function(current[0] + t*direction[0], 
                                    current[1] + t*direction[1]);
        }
        
        // Обновление текущей точки
        current[0] += (t - STEP_SIZE) * direction[0];
        current[1] += (t - STEP_SIZE) * direction[1];
    }
    
    return current;
}

int main() {
    // Начальные приближения (можно варьировать для поиска разных минимумов)
    vector<double> initial_points[] = {
        {0.5, 1.0},   
        {1.5, 1.5},  
        {-0.5, 0.5}  
    };
    
    for (auto &init : initial_points) {
        cout << "Начальная точка: theta = " << init[0] << ", y2 = " << init[1] << endl;
        
        auto result = steepest_descent(init[0], init[1]);
        
        double theta = result[0];
        double y2 = result[1];
        double x1, y1;
        lemniscate(theta, x1, y1);
        double x2 = curve2_x(y2);
        
        double distance = sqrt(target_function(theta, y2));
        
        cout << "Результат:" << endl;
        cout << "Точка на лемнискате: (" << x1 << ", " << y1 << ")" << endl;
        cout << "Точка на второй кривой: (" << x2 << ", " << y2 << ")" << endl;
        cout << "Минимальное расстояние: " << distance << endl;
        cout << "----------------------------------------" << endl;
    }
    
    return 0;
}