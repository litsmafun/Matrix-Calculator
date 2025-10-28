#include "BasicMatrixFactory.h"
#include "DenseMatrix.h"
#include "SparseMatrix.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <filesystem>
#include <windows.h>

namespace fs = std::filesystem;

// 获取 exe 所在目录的 matrices 文件夹路径
std::string getMatricesFolder() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    fs::path exePath(buffer);
    fs::path matricesPath = exePath.parent_path() / "matrices";
    
    // 如果文件夹不存在，创建它
    if (!fs::exists(matricesPath)) {
        fs::create_directories(matricesPath);
    }
    
    return matricesPath.string();
}

std::shared_ptr<Matrix> BasicMatrixFactory::createMatrixAuto(const std::vector<std::vector<double>>& data) {
    double sparsity = calculateSparsity(data);
    // 根据稀疏度决定创建稠密矩阵还是稀疏矩阵
    if (sparsity > 0.7) { // 稀疏度 > 70%，创建稀疏矩阵
        // 转换为 CSR 格式
        int rows = data.size();
        int cols = data[0].size();
        std::vector<double> values;
        std::vector<int> colIndices;
        std::vector<int> rowPointers(rows + 1, 0);
        
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (std::abs(data[i][j]) > 1e-10) {
                    values.push_back(data[i][j]);
                    colIndices.push_back(j);
                    rowPointers[i + 1]++;
                }
            }
        }
        
        // 累加 rowPointers
        for (int i = 1; i <= rows; ++i) {
            rowPointers[i] += rowPointers[i - 1];
        }
        
        return std::make_shared<SparseMatrix>(values, colIndices, rowPointers, rows, cols);
    } else {
        return std::make_shared<DenseMatrix>(data);
    }
}

std::shared_ptr<Matrix> BasicMatrixFactory::createMatrixAuto(int rows, int cols, const std::string& data) {
    // 解析字符串数据为二维向量
    std::vector<std::vector<double>> matrix(rows, std::vector<double>(cols, 0.0));
    std::istringstream iss(data);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            iss >> matrix[i][j];
        }
    }
    return createMatrixAuto(matrix);
}

std::shared_ptr<Matrix> BasicMatrixFactory::createMatrixAuto(const Matrix& m) {
    // 自动判断已有矩阵是否需要转换类型
    // 尝试转换为具体类型，利用已有的转换方法
    const DenseMatrix* dense = dynamic_cast<const DenseMatrix*>(&m);
    const SparseMatrix* sparse = dynamic_cast<const SparseMatrix*>(&m);
    
    if (dense) {
        // 从稠密矩阵创建：检查稀疏度决定类型
        double sparsity = 1.0 - static_cast<double>(m.nnz()) / (m.rows() * m.cols());
        if (sparsity > 0.7) {
            // 转换为稀疏矩阵
            auto sparsePtr = dense->toSparse();
            return std::make_shared<SparseMatrix>(sparsePtr->getValues(), 
                                   sparsePtr->getColIndices(), 
                                   sparsePtr->getRowPointers(), 
                                   m.rows(), m.cols());
        } else {
            // 保持稠密矩阵
            return std::make_shared<DenseMatrix>(dense->getData());
        }
    } else if (sparse) {
        // 从稀疏矩阵创建：检查稀疏度决定类型
        double sparsity = 1.0 - static_cast<double>(m.nnz()) / (m.rows() * m.cols());
        if (sparsity <= 0.7) {
            // 转换为稠密矩阵
            auto densePtr = sparse->toDense();
            return std::make_shared<DenseMatrix>(densePtr->getData());
        } else {
            // 保持稀疏矩阵
            return std::make_shared<SparseMatrix>(sparse->getValues(), 
                                   sparse->getColIndices(), 
                                   sparse->getRowPointers(), 
                                   m.rows(), m.cols());
        }
    }
    
    // 兜底：未知类型，暴力提取（理论上不应该到这里）
    std::vector<std::vector<double>> data(m.rows(), std::vector<double>(m.cols()));
    for (int i = 0; i < m.rows(); ++i) {
        for (int j = 0; j < m.cols(); ++j) {
            data[i][j] = m.get(i, j);
        }
    }
    return createMatrixAuto(data);
}

std::shared_ptr<Matrix> BasicMatrixFactory::loadFromFile(const std::string& filename) {
    // 构建完整路径：matrices 文件夹 + 文件名
    fs::path fullPath = fs::path(getMatricesFolder()) / filename;
    
    std::ifstream file(fullPath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + fullPath.string());
    }
    
    std::string format;
    std::getline(file, format);
    
    if (format == "SPARSE") {
        // 读取稀疏矩阵
        int rows, cols;
        file >> rows >> cols;
        file.ignore(); // 忽略换行符
        
        int nnz;
        file >> nnz;
        file.ignore();
        
        std::vector<double> values(nnz);
        std::vector<int> colIndices(nnz);
        std::vector<int> rowPointers(rows + 1);
        
        // 读取 values
        for (int i = 0; i < nnz; ++i) {
            file >> values[i];
        }
        file.ignore();
        
        // 读取 colIndices
        for (int i = 0; i < nnz; ++i) {
            file >> colIndices[i];
        }
        file.ignore();
        
        // 读取 rowPointers
        for (int i = 0; i <= rows; ++i) {
            file >> rowPointers[i];
        }
        
        return std::make_shared<SparseMatrix>(values, colIndices, rowPointers, rows, cols);
        
    } else if (format == "DENSE") {
        // 读取稠密矩阵
        int rows, cols;
        file >> rows >> cols;
        file.ignore();
        
        std::vector<std::vector<double>> data(rows, std::vector<double>(cols));
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                file >> data[i][j];
            }
        }
        
        return std::make_shared<DenseMatrix>(data);
        
    } else {
        // 旧格式（无标记），假设为稠密矩阵
        std::vector<std::vector<double>> data;
        
        // 第一行已经读取，需要解析
        std::vector<double> firstRow;
        std::stringstream ss(format);
        double val;
        while (ss >> val) {
            firstRow.push_back(val);
        }
        if (!firstRow.empty()) {
            data.push_back(firstRow);
        }
        
        // 读取剩余行
        std::string line;
        while (std::getline(file, line)) {
            std::vector<double> row;
            std::stringstream ss2(line);
            while (ss2 >> val) {
                row.push_back(val);
            }
            if (!row.empty()) {
                data.push_back(row);
            }
        }
        
        return createMatrixAuto(data);
    }
}

void BasicMatrixFactory::saveToFile(const std::string& filename, const Matrix& matrix) {
    // 构建完整路径：matrices 文件夹 + 文件名
    fs::path fullPath = fs::path(getMatricesFolder()) / filename;
    
    std::ofstream file(fullPath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + fullPath.string());
    }
    
    // 尝试转换为稀疏矩阵或稠密矩阵以确定类型
    const SparseMatrix* sparse = dynamic_cast<const SparseMatrix*>(&matrix);
    
    if (sparse) {
        // 保存为稀疏矩阵格式
        file << "SPARSE" << std::endl;
        file << matrix.rows() << " " << matrix.cols() << std::endl;
        
        auto values = sparse->getValues();
        auto colIndices = sparse->getColIndices();
        auto rowPointers = sparse->getRowPointers();
        
        // 保存非零元素个数
        file << values.size() << std::endl;
        
        // 保存 values
        for (size_t i = 0; i < values.size(); ++i) {
            file << values[i];
            if (i < values.size() - 1) file << " ";
        }
        file << std::endl;
        
        // 保存 colIndices
        for (size_t i = 0; i < colIndices.size(); ++i) {
            file << colIndices[i];
            if (i < colIndices.size() - 1) file << " ";
        }
        file << std::endl;
        
        // 保存 rowPointers
        for (size_t i = 0; i < rowPointers.size(); ++i) {
            file << rowPointers[i];
            if (i < rowPointers.size() - 1) file << " ";
        }
        file << std::endl;
    } else {
        // 保存为稠密矩阵格式
        file << "DENSE" << std::endl;
        file << matrix.rows() << " " << matrix.cols() << std::endl;
        
        for (int i = 0; i < matrix.rows(); ++i) {
            for (int j = 0; j < matrix.cols(); ++j) {
                file << matrix.get(i, j);
                if (j < matrix.cols() - 1) file << " ";
            }
            file << std::endl;
        }
    }
    
    file.close();
}

double BasicMatrixFactory::calculateSparsity(const std::vector<std::vector<double>>& data) {
    if (data.empty()) return 0.0;
    int total = data.size() * data[0].size();
    int zeroCount = 0;
    for (const auto& row : data) {
        for (double val : row) {
            if (val == 0.0) ++zeroCount;
        }
    }
    return static_cast<double>(zeroCount) / total;
}
