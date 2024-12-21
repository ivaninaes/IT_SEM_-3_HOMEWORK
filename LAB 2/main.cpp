#include "tests/tests.h"
#include "matrix/matrix.h"
#include <iostream>
#include <atomic>
#include "determinantFunctions/gauss.h"
#include "determinantFunctions/lu_decomposition.h"
#include "determinantFunctions/laplace_expansion.h"

int main() {
    // Генерация тестов
    std::vector<TestCase> testCases = genTests();

    // Запуск тестов с функцией вычисления детерминанта методом Гаусса
    runTests(testCases, calculateDeterminantGauss, "GaussDeterminant");

    return 0;
}