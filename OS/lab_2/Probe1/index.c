#include <stdio.h>
#include <math.h>
#include <conio.h>
#include <io.h>

int main() {
    int N;
    double A, S = 0.0;

    int is_interactive = _isatty(_fileno(stdin));

    if (is_interactive) {
        printf("enter number N: ");
        fflush(stdout);
    }
    scanf("%d", &N);

    if (is_interactive) {
        printf("enter value A: ");
        fflush(stdout);
    }
    scanf("%lf", &A);

    for (int i = 1; i <= N; i++) {
        S += sin(A) * i;
    }

    if (is_interactive) {
        printf("result: %.2lf\n", S);
        fflush(stdout);
    } else {
        printf("%.2lf\n", S);
        fflush(stdout);
    }

    return 0;
}