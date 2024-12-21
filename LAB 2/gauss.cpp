#include "gauss.h"
#include "../matrix/matrix.h"
#include <stdexcept>

#include <iostream>
#include <atomic>
#include <cmath>

double calculateDeterminantGauss(const Matrix<double>& matrix, std::atomic<bool>& cancelFlag) {
    int n = matrix.getRows();
    if (n == 0) return 1; // Детерминант пустой матрицы
    if (n != matrix.getCols()) throw std::invalid_argument("Matrix must be square");
    // std::cout << "initial matrix " << matrix(0,0) << ":\n";
    Matrix<double> mat = matrix;
    double det = 1;
    const double epsilon = 1e-9; // Порог для проверки нулевых значений

    for (int i = 0; i < n; ++i) {
        /*// Отладочные сообщения
        std::cout << "Step " << i << ":\n";
        std::cout << "Matrix:\n";
        for (int r = 0; r < n; ++r) {
            for (int c = 0; c < n; ++c) {
                std::cout << mat(r, c) << " ";
            }
            std::cout << "\n";
        }
        std::cout << "Determinant so far: " << det << "\n";*/
        if (cancelFlag.load()) {
            throw std::runtime_error("Operation cancelled");
        }
        int pivot = i;
        for (int j = i + 1; j < n; ++j) {
            if (std::abs(mat(j, i)) > std::abs(mat(pivot, i))) {
                pivot = j;
            }
        }
        // std::cout << "pivot " << mat(pivot, i) << " ";
        if (std::abs(mat(pivot, i)) < epsilon) return 0;

        if (i != pivot) {
            for (int k = 0; k < n; ++k) {
                std::swap(mat(i, k), mat(pivot, k));
            }
            det = -det;
        }

        det *= mat(i, i);

        for (int j = i + 1; j < n; ++j) {
            mat(i, j) /= mat(i, i);
        }

        for (int j = i + 1; j < n; ++j) {
            for (int k = i + 1; k < n; ++k) {
                mat(j, k) -= mat(j, i) * mat(i, k);
            }
        }


    }

    return det;
}