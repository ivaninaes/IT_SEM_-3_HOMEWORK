#include "tests.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <random>
#include <functional>
#include <thread>
#include <future>
#include <atomic>

// Обертка для функции, которая поддерживает отмену
double cancellableFunction(DeterminantFunction func, const Matrix<double>& matrix, std::atomic<bool>& cancelFlag) {
    return func(matrix, cancelFlag);
}

// Функция для генерации треугольной матрицы
Matrix<double> generateTriangularMatrix(int size, bool upper = true) {
    Matrix<double> matrix(size, size, 0.0);
    std::mt19937 gen(42); // фиксированный seed для воспроизводимости
    std::uniform_real_distribution<> dis(1.0, 10.0);

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if ((upper && j >= i) || (!upper && j <= i)) {
                matrix(i, j) = dis(gen);
            }
        }
    }
    return matrix;
}

// Функция для вычисления детерминанта треугольной матрицы
double calculateTriangularDeterminant(const Matrix<double>& matrix) {
    double det = 1.0;
    for (unsigned i = 0; i < matrix.getRows(); ++i) {
        det *= matrix(i, i);
    }
    return det;
}

// Функция для добавления тестов с большими матрицами
void addLargeMatrixTests(std::vector<TestCase>& testCases) {
    // Генерация диагональной матрицы 10x10
    Matrix<double> matrix10x10(10, 10, 0.0);
    for (int i = 0; i < 10; ++i) {
        matrix10x10(i, i) = i + 1;
    }
    testCases.push_back({matrix10x10, 3628800}); // 10!

    // Генерация диагональной матрицы 100x100
    Matrix<double> matrix100x100(100, 100, 0.0);
    for (int i = 0; i < 100; ++i) {
        matrix100x100(i, i) = i + 1;
    }
    testCases.push_back({matrix100x100, 9.33262154439441e+157}); // 100!

    // Генерация диагональной матрицы 1000x1000
    Matrix<double> matrix1000x1000(1000, 1000, 0.0);
    for (int i = 0; i < 1000; ++i) {
        matrix1000x1000(i, i) = i + 1;
    }
    testCases.push_back({matrix1000x1000, 0}); // Детерминант слишком большой для точного вычисления
}

// Функция для генерации тестов
std::vector<TestCase> genTests() {
    std::vector<TestCase> testCases = {
        {Matrix<double>(0, 0), 1},
        {Matrix<double>(1, 1, 5), 5},
        {Matrix<double>({{1, 2}, {3, 4}}), -2},
        {Matrix<double>({{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}), 1},
        {Matrix<double>({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}), 0},
        {Matrix<double>({{2, 3, 1}, {4, 1, 6}, {7, 8, 9}}), -35},
        {Matrix<double>({{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}), 0},
        {Matrix<double>({{1, 2, 3}, {2, 4, 6}, {3, 6, 9}}), 0}
    };

    // Добавление тестов для треугольных матриц
    auto triangular10x10 = generateTriangularMatrix(10);
    testCases.push_back({triangular10x10, calculateTriangularDeterminant(triangular10x10)});

    auto triangular1000x1000 = generateTriangularMatrix(1000);
    testCases.push_back({triangular1000x1000, calculateTriangularDeterminant(triangular1000x1000)});

    auto triangular2000x2000 = generateTriangularMatrix(2000);
    testCases.push_back({triangular2000x2000, calculateTriangularDeterminant(triangular2000x2000)});

    // Добавление тестов для больших диагональных матриц
    addLargeMatrixTests(testCases);
    /*for (const auto& [matrix, expected] : testCases) {
        try {
            std::cout << "first Element " << matrix(0, 0) << std::endl;
        }
        catch (...) {}

    }*/
    return testCases;
}

// Функция для выполнения тестов
void runTests(const std::vector<TestCase>& testCases, DeterminantFunction func, const std::string& funcName) {
    std::cout << "Testing function: " << funcName << std::endl;

    std::string reportFileName = funcName + "_tests.txt";
    std::ofstream report(reportFileName);
    if (!report.is_open()) {
        std::cerr << "Failed to open report file: " << reportFileName << std::endl;
        return;
    }

    int testNumber = 1;
    int passedTests = 0;

    for (const auto& [matrix, expected] : testCases) {
        std::cout << "Running test " << testNumber << "..." << std::endl;

        std::atomic<bool> cancelFlag(false);

        try {
            auto start = std::chrono::high_resolution_clock::now();

            // Запуск функции в отдельном потоке с использованием std::async
            auto future = std::async(std::launch::async, cancellableFunction, func, matrix, std::ref(cancelFlag));

            // Ожидание результата с таймаутом в 1 минуту
            if (future.wait_for(std::chrono::minutes(1)) == std::future_status::timeout) {
                cancelFlag.store(true); // Устанавливаем флаг отмены
                report << "Test " << testNumber << " failed. Time limit exceeded.\n";
                std::cerr << "Test " << testNumber << " failed. Time limit exceeded.\n";
            } else {
                double result = future.get();
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;

                if (abs(result - expected) < 1e-9) {
                    report << "Test " << testNumber << " passed in " << duration.count() << " seconds.\n";
                    ++passedTests;
                } else {
                    report << "Test " << testNumber << " failed. Expected " << expected << ", got " << result << ".\n";
                }
            }
        } catch (const std::invalid_argument& e) {
            report << "Test " << testNumber << " failed with error: " << e.what() << "\n";
            std::cerr << "Test " << testNumber << " failed with error: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            report << "Test " << testNumber << " failed with unexpected error: " << e.what() << "\n";
            std::cerr << "Test " << testNumber << " failed with unexpected error: " << e.what() << std::endl;
        } catch (...) {
            report << "Test " << testNumber << " failed with unknown error.\n";
            std::cerr << "Test " << testNumber << " failed with unknown error.\n";
        }

        ++testNumber;
    }

    report << "Passed " << passedTests << " out of " << testCases.size() << " tests.\n";
    report.close();
}
