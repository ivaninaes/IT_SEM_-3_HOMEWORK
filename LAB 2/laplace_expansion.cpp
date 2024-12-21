#include "laplace_expansion.h"
#include "../matrix/matrix.h"
#include <stdexcept>
#include <cmath>
#include <atomic>

// Функция для вычисления миноров
Matrix<double> getMinor(const Matrix<double>& matrix, int row, int col) {
    int n = matrix.getRows();
    Matrix<double> minorMatrix(n - 1, n - 1);

    for (int i = 0, mi = 0; i < n; ++i) {
        if (i == row) continue;
        for (int j = 0, mj = 0; j < n; ++j) {
            if (j == col) continue;
            minorMatrix(mi, mj) = matrix(i, j);
            ++mj;
        }
        ++mi;
    }

    return minorMatrix;
}

// Функция для вычисления детерминанта с использованием разложения по строке или столбцу
double calculateDeterminantLaplace(const Matrix<double>& matrix, std::atomic<bool>& cancelFlag) {
    int n = matrix.getRows();
    if (n == 0) return 1; // Детерминант пустой матрицы
    if (n != matrix.getCols()) throw std::invalid_argument("Matrix must be square");

    if (n == 1) return matrix(0, 0);
    if (n == 2) return matrix(0, 0) * matrix(1, 1) - matrix(0, 1) * matrix(1, 0);

    double det = 0.0;
    for (int j = 0; j < n; ++j) {
        if (cancelFlag.load()) {
            throw std::runtime_error("Operation cancelled");
        }
        Matrix<double> minorMatrix = getMinor(matrix, 0, j);
        det += (j % 2 == 0 ? 1 : -1) * matrix(0, j) * calculateDeterminantLaplace(minorMatrix, cancelFlag);
    }

    return det;
}