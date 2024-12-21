#include "lu_decomposition.h"
#include "../matrix/matrix.h"
#include <stdexcept>
#include <cmath>
#include <atomic>

// Функция для LU-разложения
void luDecomposition(const Matrix<double>& matrix, Matrix<double>& L, Matrix<double>& U) {
    int n = matrix.getRows();
    if (n == 0 || n != matrix.getCols()) {
        throw std::invalid_argument("Matrix must be square");
    }

    L = Matrix<double>(n, n, 0);
    U = Matrix<double>(n, n, 0);

    for (int i = 0; i < n; ++i) {
        // Заполнение верхнетреугольной матрицы U
        for (int k = i; k < n; ++k) {
            double sum = 0;
            for (int j = 0; j < i; ++j) {
                sum += (L(i, j) * U(j, k));
            }
            U(i, k) = matrix(i, k) - sum;
        }

        // Заполнение нижнетреугольной матрицы L
        for (int k = i; k < n; ++k) {
            if (i == k) {
                L(i, i) = 1; // Диагональные элементы L равны 1
            } else {
                double sum = 0;
                for (int j = 0; j < i; ++j) {
                    sum += (L(k, j) * U(j, i));
                }
                L(k, i) = (matrix(k, i) - sum) / U(i, i);
            }
        }
    }
}

// Функция для вычисления детерминанта с использованием LU-разложения
double calculateDeterminantLU(const Matrix<double>& matrix, std::atomic<bool>& cancelFlag) {
    int n = matrix.getRows();
    if (n == 0) return 1; // Детерминант пустой матрицы
    if (n != matrix.getCols()) throw std::invalid_argument("Matrix must be square");

    Matrix<double> L, U;
    luDecomposition(matrix, L, U);

    if (cancelFlag.load()) {
        throw std::runtime_error("Operation cancelled");
    }

    double det = 1.0;
    for (int i = 0; i < U.getRows(); ++i) {
        if (cancelFlag.load()) {
            throw std::runtime_error("Operation cancelled");
        }
        det *= U(i, i);
    }
    return det;
}