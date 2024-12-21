#ifndef MATRIX_H
#define MATRIX_H

#include "subvector.h"
#include <stdexcept>
#include <initializer_list>
#include <iostream>

template<typename T>
class Matrix {
    subvector<T> data;  // Используем шаблонный subvector для хранения данных
    unsigned cols, rows;

public:
    // 1) Конструирование

    // создание матрицы, заполненной value
    Matrix(unsigned rows, unsigned cols, T value = 0);

    // создание матрицы из списка инициализации
    Matrix(std::initializer_list<std::initializer_list<T>> init);

    // конструктор по умолчанию
    Matrix();

    // конструктор копирования
    Matrix(const Matrix& other);

    // оператор присваивания
    Matrix& operator=(const Matrix& other);

    // создание единичной матрицы
    static Matrix Identity(unsigned size);

    // создание случайной матрицы с заданным детерминантом
    static Matrix getSpecificDeterminant(unsigned n, T determinant);

    // 2) Селекторы
    unsigned getRows() const;
    unsigned getCols() const;

    // 3) Методы для разминки и проверки работы вычисления детерминанта

    Matrix& transpose();    // транспонирую себя
    Matrix transpose() const;   // копирую себя, транспонируют и возвращаю

    // 4) Доступ к элементам
    T& operator()(unsigned row, unsigned col);
    T operator()(unsigned row, unsigned col) const;

    // 5) Любые методы, которые вам будут удобны для работы с матрицей
    // например, создание подматрицы, перестановка строк (столбцов), умножение строки (столбца) на число и так далее
};

#include "matrix_impl.h"  // Включаем реализацию шаблонного класса

#endif // MATRIX_H
