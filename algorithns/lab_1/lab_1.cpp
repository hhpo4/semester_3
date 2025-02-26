#include <iostream>
#include <iomanip>

template<typename T>
T determinant(T matrix[3][3]) {
    return matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]) -
           matrix[0][1] * (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0]) +
           matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
}

template<typename T>
bool inverse(T matrix[3][3], T result[3][3]) {
    T det = determinant(matrix);
    
    if (det == 0) {
        std::cout << "Матрица вырожденная, обратной матрицы не существует\n";
        return false;
    }

    result[0][0] = (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]) / det;
    result[0][1] = -(matrix[0][1] * matrix[2][2] - matrix[0][2] * matrix[2][1]) / det;
    result[0][2] = (matrix[0][1] * matrix[1][2] - matrix[0][2] * matrix[1][1]) / det;
    
    result[1][0] = -(matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0]) / det;
    result[1][1] = (matrix[0][0] * matrix[2][2] - matrix[0][2] * matrix[2][0]) / det;
    result[1][2] = -(matrix[0][0] * matrix[1][2] - matrix[0][2] * matrix[1][0]) / det;
    
    result[2][0] = (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]) / det;
    result[2][1] = -(matrix[0][0] * matrix[2][1] - matrix[0][1] * matrix[2][0]) / det;
    result[2][2] = (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]) / det;

    return true;
}

template<typename T>
void printMatrix(T matrix[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            std::cout << std::setw(10) << std::fixed << std::setprecision(3) << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    int intMatrix[3][3] = {
        {1, 2, 0},
        {3, 4, 1},
        {0, 1, 5}
    };
    
    int intResult[3][3];
    
    std::cout << "Целочисленная матрица:\n";
    printMatrix(intMatrix);
    
    if (inverse(intMatrix, intResult)) {
        std::cout << "Обратная матрица:\n";
        printMatrix(intResult);
    }

    double doubleMatrix[3][3] = {
        {1.5, 2.3, 1.0},
        {3.1, 4.2, 2.1},
        {1.0, 1.5, 3.5}
    };
    
    double doubleResult[3][3];
    
    std::cout << "Матрица с плавающей точкой:\n";
    printMatrix(doubleMatrix);
    
    if (inverse(doubleMatrix, doubleResult)) {
        std::cout << "Обратная матрица:\n";
        printMatrix(doubleResult);
    }

    return 0;
}
