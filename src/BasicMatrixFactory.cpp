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

Matrix* BasicMatrixFactory::createMatrixAuto(const std::vector<std::vector<double>>& data) {
    double sparsity = calculateSparsity(data);
    if (sparsity > 0.7) { // 阈值70%
        // 需要将 data 转换为 CSR 格式
        return new DenseMatrix(data); // 暂时返回 DenseMatrix
    } else {
        return new DenseMatrix(data);
    }
}

Matrix* BasicMatrixFactory::createMatrixAuto(int rows, int cols, const std::string& data) {
    std::vector<std::vector<double>> matrix(rows, std::vector<double>(cols, 0.0));
    std::istringstream iss(data);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            iss >> matrix[i][j];
        }
    }
    return createMatrixAuto(matrix);
}

Matrix* BasicMatrixFactory::createMatrixAuto(const Matrix& m) {
    std::vector<std::vector<double>> data(m.rows(), std::vector<double>(m.cols()));
    for (int i = 0; i < m.rows(); ++i) {
        for (int j = 0; j < m.cols(); ++j) {
            data[i][j] = m.get(i, j);
        }
    }
    return createMatrixAuto(data);
}

Matrix* BasicMatrixFactory::loadFromFile(const std::string& filename) {
    // 构建完整路径：matrices 文件夹 + 文件名
    fs::path fullPath = fs::path(getMatricesFolder()) / filename;
    
    std::ifstream file(fullPath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + fullPath.string());
    }
    std::vector<std::vector<double>> data;
    std::string line;
    while (std::getline(file, line)) {
        std::vector<double> row;
        std::stringstream ss(line);
        double val;
        while (ss >> val) {
            row.push_back(val);
        }
        if (!row.empty()) {
            data.push_back(row);
        }
    }
    return createMatrixAuto(data);
}

void BasicMatrixFactory::saveToFile(const std::string& filename, const Matrix& matrix) {
    // 构建完整路径：matrices 文件夹 + 文件名
    fs::path fullPath = fs::path(getMatricesFolder()) / filename;
    
    std::ofstream file(fullPath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + fullPath.string());
    }
    for (int i = 0; i < matrix.rows(); ++i) {
        for (int j = 0; j < matrix.cols(); ++j) {
            file << matrix.get(i, j);
            if (j < matrix.cols() - 1) file << " ";
        }
        file << "\n";
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
