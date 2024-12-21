#ifndef LAPLACE_EXPANSION_H
#define LAPLACE_EXPANSION_H

#include "../matrix/matrix.h"
#include <atomic>

double calculateDeterminantLaplace(const Matrix<double>& matrix, std::atomic<bool>& cancelFlag);

#endif // LAPLACE_EXPANSION_H
