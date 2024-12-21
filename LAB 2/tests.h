#ifndef TESTS_H
#define TESTS_H

#include <vector>
#include <functional>
#include <string>
#include <atomic>
#include "../matrix/matrix.h"

// Определение типа для тестовых случаев
using TestCase = std::pair<Matrix<double>, double>;

// Определение типа для функции, вычисляющей детерминант
using DeterminantFunction = std::function<double(const Matrix<double>&, std::atomic<bool>&)>;

// Функция для генерации тестов
std::vector<TestCase> genTests();

// Функция для выполнения тестов
void runTests(const std::vector<TestCase>& testCases, DeterminantFunction func, const std::string& funcName);

#endif // TESTS_H
