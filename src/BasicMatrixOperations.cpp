#include "BasicMatrixOperations.h"
#include "DenseMatrix.h"
#include "SparseMatrix.h"
#include <stdexcept>
#include <vector>
#include <cmath>
#include <memory>
#include <map>

// 矩阵加法
std::shared_ptr<Matrix> BasicMatrixOperations::add(const Matrix& a, const Matrix& b) {
    if (a.rows() != b.rows() || a.cols() != b.cols()) {
        throw std::invalid_argument("Matrix dimensions must match for addition");
    }
    
    int rows = a.rows();
    int cols = a.cols();
    
    // 尝试转换为稀疏矩阵
    const SparseMatrix* sparseA = dynamic_cast<const SparseMatrix*>(&a);
    const SparseMatrix* sparseB = dynamic_cast<const SparseMatrix*>(&b);
    
    // 如果两个都是稀疏矩阵，使用稀疏矩阵加法
    if (sparseA && sparseB) {
        std::vector<double> values;
        std::vector<int> colIndices;
        std::vector<int> rowPointers(rows + 1, 0);
        
        for (int i = 0; i < rows; ++i) {
            std::map<int, double> rowMap;
            
            // 添加矩阵A的元素
            for (int idx = sparseA->getRowPointers()[i]; idx < sparseA->getRowPointers()[i + 1]; ++idx) {
                int j = sparseA->getColIndices()[idx];
                rowMap[j] = sparseA->getValues()[idx];
            }
            
            // 添加矩阵B的元素
            for (int idx = sparseB->getRowPointers()[i]; idx < sparseB->getRowPointers()[i + 1]; ++idx) {
                int j = sparseB->getColIndices()[idx];
                rowMap[j] += sparseB->getValues()[idx];
            }
            
            // 构建结果行
            for (const auto& pair : rowMap) {
                if (std::abs(pair.second) > 1e-10) {
                    values.push_back(pair.second);
                    colIndices.push_back(pair.first);
                    rowPointers[i + 1]++;
                }
            }
        }
        
        // 累积行指针
        for (int i = 1; i <= rows; ++i) {
            rowPointers[i] += rowPointers[i - 1];
        }
        
        return std::make_shared<SparseMatrix>(values, colIndices, rowPointers, rows, cols);
    }
    
    // 否则使用稠密矩阵加法
    std::vector<std::vector<double>> result(rows, std::vector<double>(cols));
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result[i][j] = a.get(i, j) + b.get(i, j);
        }
    }
    
    return std::make_shared<DenseMatrix>(result);
}

// 矩阵减法
std::shared_ptr<Matrix> BasicMatrixOperations::sub(const Matrix& a, const Matrix& b) {
    if (a.rows() != b.rows() || a.cols() != b.cols()) {
        throw std::invalid_argument("Matrix dimensions must match for subtraction");
    }
    
    int rows = a.rows();
    int cols = a.cols();
    
    // 尝试转换为稀疏矩阵
    const SparseMatrix* sparseA = dynamic_cast<const SparseMatrix*>(&a);
    const SparseMatrix* sparseB = dynamic_cast<const SparseMatrix*>(&b);
    
    // 如果两个都是稀疏矩阵，使用稀疏矩阵减法
    if (sparseA && sparseB) {
        std::vector<double> values;
        std::vector<int> colIndices;
        std::vector<int> rowPointers(rows + 1, 0);
        
        for (int i = 0; i < rows; ++i) {
            std::map<int, double> rowMap;
            
            // 添加矩阵A的元素
            for (int idx = sparseA->getRowPointers()[i]; idx < sparseA->getRowPointers()[i + 1]; ++idx) {
                int j = sparseA->getColIndices()[idx];
                rowMap[j] = sparseA->getValues()[idx];
            }
            
            // 减去矩阵B的元素
            for (int idx = sparseB->getRowPointers()[i]; idx < sparseB->getRowPointers()[i + 1]; ++idx) {
                int j = sparseB->getColIndices()[idx];
                rowMap[j] -= sparseB->getValues()[idx];
            }
            
            // 构建结果行
            for (const auto& pair : rowMap) {
                if (std::abs(pair.second) > 1e-10) {
                    values.push_back(pair.second);
                    colIndices.push_back(pair.first);
                    rowPointers[i + 1]++;
                }
            }
        }
        
        // 累积行指针
        for (int i = 1; i <= rows; ++i) {
            rowPointers[i] += rowPointers[i - 1];
        }
        
        return std::make_shared<SparseMatrix>(values, colIndices, rowPointers, rows, cols);
    }
    
    // 否则使用稠密矩阵减法
    std::vector<std::vector<double>> result(rows, std::vector<double>(cols));
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result[i][j] = a.get(i, j) - b.get(i, j);
        }
    }
    
    return std::make_shared<DenseMatrix>(result);
}

// 矩阵/向量点乘
std::shared_ptr<Matrix> BasicMatrixOperations::dot(const Matrix& a, const Matrix& b) {
    if (a.cols() != b.rows()) {
        throw std::invalid_argument("Matrix dimensions incompatible for multiplication");
    }
    
    int rows = a.rows();
    int cols = b.cols();
    int inner = a.cols();
    
    // 尝试转换为稀疏矩阵
    const SparseMatrix* sparseA = dynamic_cast<const SparseMatrix*>(&a);
    const SparseMatrix* sparseB = dynamic_cast<const SparseMatrix*>(&b);
    
    // 如果两个都是稀疏矩阵，使用稀疏矩阵乘法
    if (sparseA && sparseB) {
        std::vector<double> values;
        std::vector<int> colIndices;
        std::vector<int> rowPointers(rows + 1, 0);
        
        // 转置矩阵B以便更高效地访问列
        std::vector<std::vector<std::pair<int, double>>> bCols(cols);
        for (int i = 0; i < b.rows(); ++i) {
            for (int idx = sparseB->getRowPointers()[i]; idx < sparseB->getRowPointers()[i + 1]; ++idx) {
                int j = sparseB->getColIndices()[idx];
                double val = sparseB->getValues()[idx];
                bCols[j].push_back({i, val});
            }
        }
        
        for (int i = 0; i < rows; ++i) {
            std::map<int, double> rowMap;
            
            // 遍历A的第i行的非零元素
            for (int idxA = sparseA->getRowPointers()[i]; idxA < sparseA->getRowPointers()[i + 1]; ++idxA) {
                int k = sparseA->getColIndices()[idxA];
                double valA = sparseA->getValues()[idxA];
                
                // 遍历B的第k行的非零元素
                for (int idxB = sparseB->getRowPointers()[k]; idxB < sparseB->getRowPointers()[k + 1]; ++idxB) {
                    int j = sparseB->getColIndices()[idxB];
                    double valB = sparseB->getValues()[idxB];
                    rowMap[j] += valA * valB;
                }
            }
            
            // 构建结果行
            for (const auto& pair : rowMap) {
                if (std::abs(pair.second) > 1e-10) {
                    values.push_back(pair.second);
                    colIndices.push_back(pair.first);
                    rowPointers[i + 1]++;
                }
            }
        }
        
        // 累积行指针
        for (int i = 1; i <= rows; ++i) {
            rowPointers[i] += rowPointers[i - 1];
        }
        
        return std::make_shared<SparseMatrix>(values, colIndices, rowPointers, rows, cols);
    }
    // 如果只有A是稀疏矩阵
    else if (sparseA) {
        std::vector<std::vector<double>> result(rows, std::vector<double>(cols, 0.0));
        
        for (int i = 0; i < rows; ++i) {
            for (int idx = sparseA->getRowPointers()[i]; idx < sparseA->getRowPointers()[i + 1]; ++idx) {
                int k = sparseA->getColIndices()[idx];
                double valA = sparseA->getValues()[idx];
                
                for (int j = 0; j < cols; ++j) {
                    result[i][j] += valA * b.get(k, j);
                }
            }
        }
        
        return std::make_shared<DenseMatrix>(result);
    }
    // 如果只有B是稀疏矩阵
    else if (sparseB) {
        std::vector<std::vector<double>> result(rows, std::vector<double>(cols, 0.0));
        
        for (int i = 0; i < rows; ++i) {
            for (int k = 0; k < inner; ++k) {
                double valA = a.get(i, k);
                if (std::abs(valA) > 1e-10) {
                    for (int idx = sparseB->getRowPointers()[k]; idx < sparseB->getRowPointers()[k + 1]; ++idx) {
                        int j = sparseB->getColIndices()[idx];
                        double valB = sparseB->getValues()[idx];
                        result[i][j] += valA * valB;
                    }
                }
            }
        }
        
        return std::make_shared<DenseMatrix>(result);
    }
    
    // 两个都是稠密矩阵
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
    
    return std::make_shared<DenseMatrix>(result);
}

// 向量叉乘
std::shared_ptr<Matrix> BasicMatrixOperations::cross(const Matrix& a, const Matrix& b) {
    if (a.rows() != 3 || a.cols() != 1 || b.rows() != 3 || b.cols() != 1) {
        throw std::invalid_argument("Cross product only defined for 3x1 vectors");
    }
    
    std::vector<std::vector<double>> result(3, std::vector<double>(1));
    
    result[0][0] = a.get(1, 0) * b.get(2, 0) - a.get(2, 0) * b.get(1, 0);
    result[1][0] = a.get(2, 0) * b.get(0, 0) - a.get(0, 0) * b.get(2, 0);
    result[2][0] = a.get(0, 0) * b.get(1, 0) - a.get(1, 0) * b.get(0, 0);
    
    return std::make_shared<DenseMatrix>(result);
}

// 矩阵转置
std::shared_ptr<Matrix> BasicMatrixOperations::transpose(const Matrix& m) {
    int rows = m.rows();
    int cols = m.cols();
    
    // 尝试转换为稀疏矩阵
    const SparseMatrix* sparse = dynamic_cast<const SparseMatrix*>(&m);
    
    // 如果是稀疏矩阵，使用稀疏矩阵转置
    if (sparse) {
        std::vector<std::vector<std::pair<int, double>>> tempCols(cols);
        
        // 收集每列的元素
        for (int i = 0; i < rows; ++i) {
            for (int idx = sparse->getRowPointers()[i]; idx < sparse->getRowPointers()[i + 1]; ++idx) {
                int j = sparse->getColIndices()[idx];
                double val = sparse->getValues()[idx];
                tempCols[j].push_back({i, val});
            }
        }
        
        // 构建转置后的CSR格式
        std::vector<double> values;
        std::vector<int> colIndices;
        std::vector<int> rowPointers(cols + 1, 0);
        
        for (int j = 0; j < cols; ++j) {
            for (const auto& pair : tempCols[j]) {
                values.push_back(pair.second);
                colIndices.push_back(pair.first);
                rowPointers[j + 1]++;
            }
        }
        
        // 累积行指针
        for (int i = 1; i <= cols; ++i) {
            rowPointers[i] += rowPointers[i - 1];
        }
        
        return std::make_shared<SparseMatrix>(values, colIndices, rowPointers, cols, rows);
    }
    
    // 否则使用稠密矩阵转置
    std::vector<std::vector<double>> result(cols, std::vector<double>(rows));
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result[j][i] = m.get(i, j);
        }
    }
    
    return std::make_shared<DenseMatrix>(result);
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
std::shared_ptr<Matrix> BasicMatrixOperations::inverse(const Matrix& m) {
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
    
    return std::make_shared<DenseMatrix>(result);
}
