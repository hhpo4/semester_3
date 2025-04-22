#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <functional>

using namespace std;

double func(double x) {
    return pow(sin(x), 2) - 2 * tan(x);
}

pair<double, double> goldenSectionMin(const function<double(double)>& f, double a, double b, double eps) {
    const double phi = (sqrt(5.0) - 1) / 2;  // ≈0.618034
    double x1 = b - phi * (b - a);
    double x2 = a + phi * (b - a);
    double f1 = f(x1);
    double f2 = f(x2);
    while (b - a > eps) {
        if (f1 > f2) {
            a = x1;
            x1 = x2;
            f1 = f2;
            x2 = a + phi * (b - a);
            f2 = f(x2);
        } else {
            b = x2;
            x2 = x1;
            f2 = f1;
            x1 = b - phi * (b - a);
            f1 = f(x1);
        }
    }
    double xMin = (a + b) / 2;
    double fMin = f(xMin);
    return make_pair(xMin, fMin);
}

pair<double, double> goldenSectionMax(const function<double(double)>& f, double a, double b, double eps) {
    auto negF = [&](double x) { return -f(x); };
    auto result = goldenSectionMin(negF, a, b, eps);
    double xMax = result.first;
    double fMax = -result.second;
    return make_pair(xMax, fMax);
}

int main() {
    double eps = 1e-5;
    // Разбиение [0,2] на сегменты из-за разрыва tan(x) при x≈1.5708
    vector<pair<double, double>> segments = {make_pair(0.0, 1.56), make_pair(1.58, 2.0)};
    ofstream outFile("extrema_results.txt");
    if (!outFile) {
        cerr << "Ошибка открытия файла для записи результатов." << endl;
        return 1;
    }
    cout << "Поиск экстремумов функции f(x)=sin^2(x)-2*tan(x) на [0,2]" << endl;
    int countFound = 0;
    for (const auto& seg : segments) {
        double a = seg.first;
        double b = seg.second;
        
        pair<double, double> minResult = goldenSectionMin(func, a, b, eps);
        double xMin = minResult.first;
        double fMin = minResult.second;
        
        pair<double, double> maxResult = goldenSectionMax(func, a, b, eps);
        double xMax = maxResult.first;
        double fMax = maxResult.second;
        
        cout << "Интервал [" << a << ", " << b << "]:\n";
        cout << "  Минимум: x = " << xMin << ", f = " << fMin << endl;
        cout << "  Максимум: x = " << xMax << ", f = " << fMax << endl;
        outFile << a << " " << b
                << " min " << xMin << " " << fMin
                << " max " << xMax << " " << fMax << endl;
        countFound += 2;
        if (countFound >= 3) break;
    }
    outFile.close();
    return 0;
}