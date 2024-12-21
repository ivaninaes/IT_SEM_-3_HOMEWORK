#ifndef LU_DECOMPOSITION_H
#define LU_DECOMPOSITION_H

#include "../matrix/matrix.h"
#include <atomic>

void luDecomposition(const Matrix<double>& matrix, Matrix<double>& L, Matrix<double>& U);

double calculateDeterminantLU(const Matrix<double>& matrix, std::atomic<bool>& cancelFlag);

#endif // LU_DECOMPOSITION_H
