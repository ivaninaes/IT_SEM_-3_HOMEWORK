#include <iostream>
#include <vector>
#include <stdexcept>
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

            if (std::abs(matrix[max_row][i]) < 1e-9) {
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
// делаем нижнюю треугольную матрицу
Matrix<double> generateLowerTriangularMatrix(unsigned int size) {
    Matrix<double> mat(size, size);
    for (unsigned int i = 0; i < size; ++i) {
        for (unsigned int j = 0; j < size; ++j) {
            if (j <= i) {
                mat[i][j] = std::rand() % 10 + 1; 
            } else {
                mat[i][j] = 0; 
            }
        }
    }
    return mat;
}

// делаем матрицу с ненулевым столбцом
Matrix<double> generateMatrixWithNonZeroColumn(unsigned int size) {
    Matrix<double> mat(size, size);
    for (unsigned int i = 0; i < size; ++i) {
        for (unsigned int j = 0; j < size; ++j) {
            mat[i][j] = 0;
        }
    }
    int nonZeroCol = std::rand() % size;
    for (unsigned int i = 0; i < size; ++i) {
        mat[i][nonZeroCol] = std::rand() % 10 + 1;
    }
    return mat;
}

// складываем матрицы
Matrix<double> addMatrices(const Matrix<double>& a, const Matrix<double>& b) {
    Matrix<double> result(a.rows(), a.cols());
    for (unsigned int i = 0; i < a.rows(); ++i) {
        for (unsigned int j = 0; j < a.cols(); ++j) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
    return result;
}

void testMatrixDeterminant() {
    std::srand(static_cast<unsigned int>(std::time(0)));

    for (const auto& size : {5, 50, 100}) {
        Matrix<double> lowertri = generateLowerTriangularMatrix(size);
        Matrix<double> nonzerocol = generateMatrixWithNonZeroColumn(size);
        Matrix<double> combined = addMatrices(lowertri, nonzerocol);

        std::cout << size << "x" << size << " matrix determinant: " << combined.determinant() << std::endl;
    }
}

int main() {
    testMatrixDeterminant();
    return 0;
}
