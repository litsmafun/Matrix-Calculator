#include "BasicMatrixOperations.h"
#include "DenseMatrix.h"
#include "SparseMatrix.h"
#include <stdexcept>
#include <vector>
#include <cmath>
#include <memory>

// 矩阵加法
Matrix* BasicMatrixOperations::add(const Matrix& a, const Matrix& b) {
    if (a.rows() != b.rows() || a.cols() != b.cols()) {
        throw std::invalid_argument("Matrix dimensions must match for addition");
    }
    
    int rows = a.rows();
    int cols = a.cols();
    std::vector<std::vector<double>> result(rows, std::vector<double>(cols));
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result[i][j] = a.get(i, j) + b.get(i, j);
        }
    }
    
    return new DenseMatrix(result);
}

// 矩阵减法
Matrix* BasicMatrixOperations::sub(const Matrix& a, const Matrix& b) {
    if (a.rows() != b.rows() || a.cols() != b.cols()) {
        throw std::invalid_argument("Matrix dimensions must match for subtraction");
    }
    
    int rows = a.rows();
    int cols = a.cols();
    std::vector<std::vector<double>> result(rows, std::vector<double>(cols));
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result[i][j] = a.get(i, j) - b.get(i, j);
        }
    }
    
    return new DenseMatrix(result);
}

// 矩阵点乘（矩阵乘法）
Matrix* BasicMatrixOperations::dot(const Matrix& a, const Matrix& b) {
    if (a.cols() != b.rows()) {
        throw std::invalid_argument("Matrix dimensions incompatible for multiplication");
    }
    
    int rows = a.rows();
    int cols = b.cols();
    int inner = a.cols();
    std::vector<std::vector<double>> result(rows, std::vector<double>(cols, 0.0));
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            double sum = 0.0;
            for (int k = 0; k < inner; ++k) {
                sum += a.get(i, k) * b.get(k, j);
            }
            result[i][j] = sum;
        }
    }
    
    return new DenseMatrix(result);
}

// 矩阵叉乘（仅适用于3x1向量）
Matrix* BasicMatrixOperations::cross(const Matrix& a, const Matrix& b) {
    if (a.rows() != 3 || a.cols() != 1 || b.rows() != 3 || b.cols() != 1) {
        throw std::invalid_argument("Cross product only defined for 3x1 vectors");
    }
    
    std::vector<std::vector<double>> result(3, std::vector<double>(1));
    
    result[0][0] = a.get(1, 0) * b.get(2, 0) - a.get(2, 0) * b.get(1, 0);
    result[1][0] = a.get(2, 0) * b.get(0, 0) - a.get(0, 0) * b.get(2, 0);
    result[2][0] = a.get(0, 0) * b.get(1, 0) - a.get(1, 0) * b.get(0, 0);
    
    return new DenseMatrix(result);
}

// 矩阵转置
Matrix* BasicMatrixOperations::transpose(const Matrix& m) {
    int rows = m.rows();
    int cols = m.cols();
    std::vector<std::vector<double>> result(cols, std::vector<double>(rows));
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result[j][i] = m.get(i, j);
        }
    }
    
    return new DenseMatrix(result);
}

// 计算行列式（使用LU分解）
double BasicMatrixOperations::determinant(const Matrix& m) {
    if (m.rows() != m.cols()) {
        throw std::invalid_argument("Determinant only defined for square matrices");
    }
    
    int n = m.rows();
    std::vector<std::vector<double>> matrix(n, std::vector<double>(n));
    
    // 复制矩阵
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            matrix[i][j] = m.get(i, j);
        }
    }
    
    double det = 1.0;
    
    // 高斯消元法计算行列式
    for (int i = 0; i < n; ++i) {
        // 寻找主元
        int maxRow = i;
        for (int k = i + 1; k < n; ++k) {
            if (std::abs(matrix[k][i]) > std::abs(matrix[maxRow][i])) {
                maxRow = k;
            }
        }
        
        // 交换行
        if (maxRow != i) {
            std::swap(matrix[i], matrix[maxRow]);
            det *= -1; // 行交换改变行列式符号
        }
        
        // 如果主元为0，行列式为0
        if (std::abs(matrix[i][i]) < 1e-10) {
            return 0.0;
        }
        
        det *= matrix[i][i];
        
        // 消元
        for (int k = i + 1; k < n; ++k) {
            double factor = matrix[k][i] / matrix[i][i];
            for (int j = i; j < n; ++j) {
                matrix[k][j] -= factor * matrix[i][j];
            }
        }
    }
    
    return det;
}

// 矩阵求逆（使用高斯-约当消元法）
Matrix* BasicMatrixOperations::inverse(const Matrix& m) {
    if (m.rows() != m.cols()) {
        throw std::invalid_argument("Inverse only defined for square matrices");
    }
    
    int n = m.rows();
    
    // 检查行列式是否为0
    if (std::abs(determinant(m)) < 1e-10) {
        throw std::invalid_argument("Matrix is singular and cannot be inverted");
    }
    
    // 创建增广矩阵 [A|I]
    std::vector<std::vector<double>> augmented(n, std::vector<double>(2 * n));
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            augmented[i][j] = m.get(i, j);
        }
        for (int j = n; j < 2 * n; ++j) {
            augmented[i][j] = (j - n == i) ? 1.0 : 0.0;
        }
    }
    
    // 高斯-约当消元
    for (int i = 0; i < n; ++i) {
        // 寻找主元
        int maxRow = i;
        for (int k = i + 1; k < n; ++k) {
            if (std::abs(augmented[k][i]) > std::abs(augmented[maxRow][i])) {
                maxRow = k;
            }
        }
        
        // 交换行
        if (maxRow != i) {
            std::swap(augmented[i], augmented[maxRow]);
        }
        
        // 归一化当前行
        double pivot = augmented[i][i];
        for (int j = 0; j < 2 * n; ++j) {
            augmented[i][j] /= pivot;
        }
        
        // 消元
        for (int k = 0; k < n; ++k) {
            if (k != i) {
                double factor = augmented[k][i];
                for (int j = 0; j < 2 * n; ++j) {
                    augmented[k][j] -= factor * augmented[i][j];
                }
            }
        }
    }
    
    // 提取逆矩阵
    std::vector<std::vector<double>> result(n, std::vector<double>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            result[i][j] = augmented[i][j + n];
        }
    }
    
    return new DenseMatrix(result);
}
