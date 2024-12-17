#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <ctime>

template<typename T>
class Matrix {
private:
    std::vector<std::vector<T>> data;

public:
    Matrix(unsigned int rows, unsigned int cols) : data(rows, std::vector<T>(cols, 0)) {}

    std::vector<T>& operator[](unsigned int index) {
        if (index >= data.size()) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    const std::vector<T>& operator[](unsigned int index) const {
        if (index >= data.size()) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    unsigned int rows() const {
        return data.size();
    }

    unsigned int cols() const {
        return (data.empty()) ? 0 : data[0].size();
    }

    T determinant() const {
        if (rows() != cols()) {
            throw std::invalid_argument("Determinant is only calculated for square matrices.");
        }

        return calculateDeterminant(data);
    }

private:
    T calculateDeterminant(std::vector<std::vector<T>> matrix) const {
        unsigned int size = matrix.size();
        T det = 1;

        for (unsigned int i = 0; i < size; ++i) {
            unsigned int max_row = i;
            for (unsigned int k = i + 1; k < size; k++) {
                if (std::abs(matrix[k][i]) > std::abs(matrix[max_row][i])) {
                    max_row = k;
                }
            }

            if (matrix[max_row][i] == 0) {
                return 0;
            }

            if (max_row != i) {
                std::swap(matrix[max_row], matrix[i]);
                det = -det;
            }

            for (unsigned int k = i + 1; k < size; k++) {
                T factor = matrix[k][i] / matrix[i][i];
                for (unsigned int j = i; j < size; j++) {
                    matrix[k][j] -= factor * matrix[i][j];
                }
            }
        }

        for (unsigned int i = 0; i < size; i++) {
            det *= matrix[i][i];
        }

        return det;
    }
};

Matrix<double> generateMatrixWithDeterminant(unsigned int size, double det);

Matrix<double> generateRandomMatrix(unsigned int rows, unsigned int cols) {
    Matrix<double> mat(rows, cols);
    std::srand(static_cast<unsigned int>(std::time(0)));
    for (unsigned int i = 0; i < rows; ++i) {
        for (unsigned int j = 0; j < cols; ++j) {
            mat[i][j] = std::rand() % 100;
        }
    }
    return mat;
}

Matrix<double> generateMatrixWithDeterminant(unsigned int size, double det) {
    Matrix<double> mat(size, size);
    for (unsigned int i = 0; i < size; ++i) {
        for (unsigned int j = 0; j < size; ++j) {
            mat[i][j] = (i == j) ? 1 : 0;
        }
    }
    mat[0][0] = det;
    return mat;
}

void testMatrixDeterminant() {
    // Тестирование случайной матрицы размером 5x5
    {
        Matrix<double> randomMat = generateRandomMatrix(5, 5);
        std::cout << "5x5 Random Matrix Determinant: " << randomMat.determinant() << std::endl;
    }

    // Тестирование случайной матрицы размером 50x50
    {
        Matrix<double> randomMat50x50 = generateRandomMatrix(50, 50);
        std::cout << "50x50 Random Matrix Determinant: " << randomMat50x50.determinant() << std::endl;
    }

    // Тестирование случайной матрицы размером 100x100
    {
        Matrix<double> randomMat100x100 = generateRandomMatrix(100, 100);
        std::cout << "100x100 Random Matrix Determinant: " << randomMat100x100.determinant() << std::endl;
    }
}

int main() {
    testMatrixDeterminant();
    return 0;
}
