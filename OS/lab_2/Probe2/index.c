#include <stdio.h>
#include <conio.h>
#include <io.h>
#include <math.h>

int main() {
    double S;

    int is_interactive = _isatty(_fileno(stdin));

    // Ввод данных
    if (is_interactive) {
        printf("enter value S:");
        fflush(stdout);
    }
    scanf("%lf", &S);

    // Вычисление результата
    S = pow(S, 3);

    // Вывод результата
    if (is_interactive) {
        printf("result (S): %.2lf\n", S);
    } else {
        printf("%.2lf\n", S);
    }
    fflush(stdout);
    return 0;
}