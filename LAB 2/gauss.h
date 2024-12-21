#ifndef DETERMINANT_H
#define DETERMINANT_H

#include "../matrix/matrix.h"
#include <atomic>

double calculateDeterminantGauss(const Matrix<double>& matrix, std::atomic<bool>& cancelFlag);

#endif // DETERMINANT_H
