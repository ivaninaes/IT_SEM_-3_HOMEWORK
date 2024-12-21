#ifndef MATRIX_IMPL_H
#define MATRIX_IMPL_H

#include "matrix.h"
#include <random>
#include <cmath>
#include <iostream>

template<typename T>
Matrix<T>::Matrix(unsigned rows, unsigned cols, T value) : cols(cols), rows(rows) {
    data.resize(rows * cols);
    for (unsigned i = 0; i < rows * cols; ++i) {
        data[i] = value;
    }
}

template<typename T>
Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T>> init) {
    rows = init.size();
    cols = init.begin()->size();
    data.resize(rows * cols);
    unsigned i = 0;
    for (const auto& row : init) {
        if (row.size() != cols) {
            throw std::invalid_argument("All rows must have the same number of columns");
        }
        unsigned j = 0;
        for (const auto& elem : row) {
            data[i * cols + j] = elem;
            ++j;
        }
        ++i;
    }
    // Отладочное сообщение для проверки инициализации
    /*std::cout << "Matrix initialized from list:\n";
    for (unsigned r = 0; r < rows; ++r) {
        for (unsigned c = 0; c < cols; ++c) {
            std::cout << data[r * cols + c] << " ";
        }
        std::cout << "\n";
    }*/
}

template<typename T>
Matrix<T>::Matrix() : cols(0), rows(0) {}

template<typename T>
Matrix<T>::Matrix(const Matrix& other) : cols(other.cols), rows(other.rows), data(other.data) {
    // Явное копирование данных
    data.resize(rows * cols);
    for (unsigned i = 0; i < rows * cols; ++i) {
        data[i] = other.data[i];
    }
    // Отладочное сообщение для проверки копирования
    /*std::cout << "Matrix copied:\n";
    for (unsigned r = 0; r < rows; ++r) {
        for (unsigned c = 0; c < cols; ++c) {
            std::cout << data[r * cols + c] << " ";
        }
        std::cout << "\n";
    }*/
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix& other) {
    if (this != &other) {
        cols = other.cols;
        rows = other.rows;
        data.resize(rows * cols);
        for (unsigned i = 0; i < rows * cols; ++i) {
            data[i] = other.data[i];
        }
    }
    // Отладочное сообщение для проверки присваивания
    /*std::cout << "Matrix assigned:\n";
    for (unsigned r = 0; r < rows; ++r) {
        for (unsigned c = 0; c < cols; ++c) {
            std::cout << data[r * cols + c] << " ";
        }
        std::cout << "\n";
    }
    return *this;*/
}

template<typename T>
Matrix<T> Matrix<T>::Identity(unsigned size) {
    Matrix m(size, size);
    for (unsigned i = 0; i < size; ++i) {
        m(i, i) = 1;
    }
    return m;
}

template<typename T>
Matrix<T> Matrix<T>::getSpecificDeterminant(unsigned n, T determinant) {
    // Создаем единичную матрицу
    Matrix<T> mat = Matrix<T>::Identity(n);

    // Генерируем случайные значения для заполнения матрицы
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-10, 10);

    // Заполняем матрицу случайными значениями
    for (unsigned i = 0; i < n; ++i) {
        for (unsigned j = 0; j < n; ++j) {
            if (i != j) {
                mat(i, j) = dis(gen);
            }
        }
    }

    // Корректируем диагональные элементы, чтобы получить нужный детерминант
    T product = 1;
    for (unsigned i = 0; i < n; ++i) {
        product *= mat(i, i);
    }

    T factor = std::pow(determinant / product, 1.0 / n);
    for (unsigned i = 0; i < n; ++i) {
        mat(i, i) *= factor;
    }

    return mat;
}

// Реализация селекторов
template<typename T>
unsigned Matrix<T>::getRows() const {
    return rows;
}

template<typename T>
unsigned Matrix<T>::getCols() const {
    return cols;
}

template<typename T>
Matrix<T>& Matrix<T>::transpose() {
    Matrix transposed(cols, rows);
    for (unsigned i = 0; i < rows; ++i) {
        for (unsigned j = 0; j < cols; ++j) {
            transposed(j, i) = (*this)(i, j);
        }
    }
    *this = transposed;
    return *this;
}

template<typename T>
Matrix<T> Matrix<T>::transpose() const {
    Matrix transposed(cols, rows);
    for (unsigned i = 0; i < rows; ++i) {
        for (unsigned j = 0; j < cols; ++j) {
            transposed(j, i) = (*this)(i, j);
        }
    }
    return transposed;
}

template<typename T>
T& Matrix<T>::operator()(unsigned row, unsigned col) {
    if (row >= rows || col >= cols) {
        throw std::out_of_range("Index out of range");
    }
    return data[row * cols + col];
}

template<typename T>
T Matrix<T>::operator()(unsigned row, unsigned col) const {
    if (row >= rows || col >= cols) {
        throw std::out_of_range("Index out of range");
    }
    return data[row * cols + col];
}

#endif // MATRIX_IMPL_H
