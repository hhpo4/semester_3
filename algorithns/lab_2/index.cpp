#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <iomanip>

const int sizes[] = {6, 7, 8, 9, 10};

template <typename T>
T** allocateMatrix(int n) {
    T** matrix = new T*[n];
    for (int i = 0; i < n; i++) {
        matrix[i] = new T[n];
    }
    return matrix;
}

template <typename T>
void freeMatrix(T** matrix, int n) {
    for (int i = 0; i < n; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

template <typename T>
void generateMatrix(T** matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = static_cast<T>(rand() % 100 + 1);
        }
    }
}

template <typename T>
T determinant(T** matrix, int n) {
    if (n == 1) return matrix[0][0];
    if (n == 2) return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];

    T det = 0;
    T** subMatrix = allocateMatrix<T>(n - 1);

    for (int x = 0; x < n; x++) {
        int subRow = 0;
        for (int i = 1; i < n; i++) {
            int subCol = 0;
            for (int j = 0; j < n; j++) {
                if (j == x) continue;
                subMatrix[subRow][subCol] = matrix[i][j];
                subCol++;
            }
            subRow++;
        }
        det += (x % 2 == 0 ? 1 : -1) * matrix[0][x] * determinant(subMatrix, n - 1);
    }

    freeMatrix(subMatrix, n - 1);
    return det;
}

template <typename T>
bool inverse(T** matrix, T** result, int n) {
    T det = determinant(matrix, n);
    if (det == 0) {
        return false; //вырожденная
    }

    T** cofactor = allocateMatrix<T>(n); //Из алгебраических дополнений
    T** temp = allocateMatrix<T>(n - 1);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int subi = 0;
            for (int x = 0; x < n; x++) {
                if (x == i) continue;
                int subj = 0;
                for (int y = 0; y < n; y++) {
                    if (y == j) continue;
                    temp[subi][subj] = matrix[x][y];
                    subj++;
                }
                subi++;
            }
            cofactor[i][j] = ((i + j) % 2 == 0 ? 1 : -1) * determinant(temp, n - 1);
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[j][i] = cofactor[i][j] / det;
        }
    }

    freeMatrix(cofactor, n);
    freeMatrix(temp, n - 1);
    return true;
}

template<typename T>
double testMatrix(size_t size, const std::string& typeName, std::ofstream& file) {
    double totalTime = 0;

    for (int trial = 0; trial < 3; trial++) {
        T** matrix = allocateMatrix<T>(size);
        T** result = allocateMatrix<T>(size);
        generateMatrix(matrix, size);

        clock_t start = clock();
        inverse(matrix, result, size);
        clock_t end = clock();

        totalTime += static_cast<double>(end - start) / CLOCKS_PER_SEC;

        freeMatrix(matrix, size);
        freeMatrix(result, size);
    }

    double avgTime = totalTime / 3;
    std::cout << "Тип: " << typeName << ", Размер: " << size << ", Время: " << avgTime << " сек\n";

    return avgTime;
}

int main() {
    std::string types[] = {"int", "float", "double", "char"};
    std::ofstream file("execution_times.txt");

    for (const auto& type : types) {
        for (size_t i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i++) {
            size_t size = sizes[i];

            if (type == "int") {
                file << testMatrix<int>(size, type, file);
                if (i < sizeof(sizes) / sizeof(sizes[0]) - 1) {
                    file << ",";
                }
            } else if (type == "float") {
                file << testMatrix<float>(size, type, file);
                if (i < sizeof(sizes) / sizeof(sizes[0]) - 1) {
                    file << ",";
                }
            } else if (type == "double") {
                file << testMatrix<double>(size, type, file);
                if (i < sizeof(sizes) / sizeof(sizes[0]) - 1) {
                    file << ",";
                }
            } else if (type == "char") {
                file << testMatrix<char>(size, type, file);
                if (i < sizeof(sizes) / sizeof(sizes[0]) - 1) {
                    file << ",";
                }
            }
        }
        file << "\n";
    }

    file.close();
    return 0;
}
