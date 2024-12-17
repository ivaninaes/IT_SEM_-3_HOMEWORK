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

    unsigned int rows() const { return data.size(); }
    
    unsigned int cols() const { return (data.empty()) ? 0 : data[0].size(); }

    T determinant() const {
        if (rows() != cols()) {
            throw std::invalid_argument("Determinant can only be calculated for square matrices.");
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


template<typename T>
std::vector<T> operator*(const Matrix<T>& matrix, const std::vector<T>& vec) {
    if (matrix.cols() != vec.size()) {
        throw std::invalid_argument("Size does not match.");
    }
    std::vector<T> result(matrix.rows(), 0);
    for (unsigned int i = 0; i < matrix.rows(); ++i) {
        for (unsigned int j = 0; j < matrix.cols(); ++j) {
            result[i] += matrix[i][j] * vec[j];
        }
    }
    return result;
}


template<typename T>
double vectorNorm(const std::vector<T>& vec) {
    double norm = 0;
    for (const T& val : vec) {
        norm += val * val;
    }
    return std::sqrt(norm);
}

template<typename T>
std::vector<T> solveSystem(const Matrix<T>& A, const std::vector<T>& b) {
    unsigned int n = A.rows();
    if (b.size() != n) {
        throw std::invalid_argument("The size of vector does not match");
    }

    Matrix<T> augmented(n, n + 1);
    for (unsigned int i = 0; i < n; i++) {
        for (unsigned int j = 0; j < n; j++) {
            augmented[i][j] = A[i][j];
        }
        augmented[i][n] = b[i];
    }

    for (unsigned int i = 0; i < n; i++) {

        for (unsigned int k = i + 1; k < n; k++) {
            if (std::abs(augmented[k][i]) > std::abs(augmented[i][i])) {
                std::swap(augmented[i], augmented[k]);
            }
        }

        for (unsigned int k = i + 1; k < n; k++) {
            T factor = augmented[k][i] / augmented[i][i];
            for (unsigned int j = i; j < n + 1; j++) {
                augmented[k][j] -= factor * augmented[i][j];
            }
        }
    }

    std::vector<T> x(n);
    for (int i = n - 1; i >= 0; i--) {
        x[i] = augmented[i][n] / augmented[i][i];
        for (int k = i - 1; k >= 0; k--) {
            augmented[k][n] -= augmented[k][i] * x[i];
        }
    }

    return x;
}

template<typename T>
double checkSolution(const Matrix<T>& A, const std::vector<T>& x, const std::vector<T>& b) {
    std::vector<T> r = A * x;
    for (unsigned int i = 0; i < r.size(); i++) {
        r[i] -= b[i];
    }
    return vectorNorm(r);
}

int main() {
    Matrix<double> A(3, 3);
    A[0] = {2, -1, 0};
    A[1] = {-1, 2, -1};
    A[2] = {0, -1, 2};

    std::vector<double> b = {1, 0, 1};

    try {
        std::vector<double> solution = solveSystem(A, b);
        std::cout << "Solution: ";
        for (double x : solution) {
            std::cout << x << " ";
        }
        std::cout << std::endl;

        double residual = checkSolution(A, solution, b);
        std::cout << "Residual (norm): " << residual << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
