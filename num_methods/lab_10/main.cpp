#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

void derivs(double x, const vector<double>& y, vector<double>& dydx) {
    dydx[0] = y[1]; // y' = z
    dydx[1] = y[0] + 2*x; // z' = y + 2x
}

void rk4_step(vector<double>& y, double& x, double h, 
              void (*derivs)(double, const vector<double>&, vector<double>&)) {
    int n = y.size();
    vector<double> k1(n), k2(n), k3(n), k4(n), y_temp(n);
    vector<double> dydx(n);
    
    derivs(x, y, dydx);
    for(int i=0; i<n; ++i) {
        k1[i] = h * dydx[i];
        y_temp[i] = y[i] + 0.5*k1[i];
    }
    
    derivs(x + 0.5*h, y_temp, dydx);
    for(int i=0; i<n; ++i) {
        k2[i] = h * dydx[i];
        y_temp[i] = y[i] + 0.5*k2[i];
    }
    
    derivs(x + 0.5*h, y_temp, dydx);
    for(int i=0; i<n; ++i) {
        k3[i] = h * dydx[i];
        y_temp[i] = y[i] + k3[i];
    }
    
    derivs(x + h, y_temp, dydx);
    for(int i=0; i<n; ++i) {
        k4[i] = h * dydx[i];
        y[i] += (k1[i] + 2*k2[i] + 2*k3[i] + k4[i])/6.0;
    }
    x += h;
}

double solve_ode(double k, double h, bool save_trajectory, const string& filename) {
    vector<double> y = {0.0, k};
    double x = 0.0;
    ofstream outfile;
    if (save_trajectory) {
        outfile.open(filename);
        outfile << x << " " << y[0] << endl;
    }
    
    while (x < 1.0) {
        double step = h;
        if (x + h > 1.0) {
            step = 1.0 - x;
        }
        rk4_step(y, x, step, derivs);
        if (save_trajectory) {
            outfile << x << " " << y[0] << endl;
        }
    }
    
    if (save_trajectory) {
        outfile.close();
    }
    
    return y[0];
}

double newton_method(double k0, double h, double epsilon, double dk, int max_iter) {
    double k = k0;
    for (int i = 0; i < max_iter; ++i) {
        string filename = "shot_" + to_string(i) + ".txt";
        double y1 = solve_ode(k, h, true, filename);
        double F = y1 + 1.0;
        
        cout << "Iteration " << i << ": k = " << k << ", F(k) = " << F << endl;
        
        if (abs(F) < epsilon) {
            cout << "after " << i+1 << " iterations." << endl;
            break;
        }
        
        double y1_changed = solve_ode(k + dk, h, true, "changed_" + to_string(i) + ".txt");
        double F_changed = y1_changed + 1.0; //Ошибка для промежуточного параметра
        double dFdk = (F_changed - F) / dk;
        
        k = k - F / dFdk;
    }
    return k;
}

int main() {
    double h = 0.01;
    double epsilon = 1e-6;
    double dk = 0.1;
    double k0 = 0;
    
    // 1. Запуск метода Ньютона для подбора k
    double k_final = newton_method(k0, h, epsilon, dk, 100);
    
    // 2. Решение ОДУ с найденным k_final
    solve_ode(k_final, h, true, "final_solution.txt");
    
    ofstream analytic_file("analytic_solution.txt");
    for (double x = 0; x <= 1.0; x += h) {
        double C1 = exp(1.0) / (exp(2.0) - 1);
        double C2 = -C1;
        double y = C1 * exp(x) + C2 * exp(-x) - 2*x;
        analytic_file << x << " " << y << endl;
    }
    analytic_file.close();
    
    return 0;
}