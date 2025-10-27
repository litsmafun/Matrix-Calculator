#include "BasicMatrixFactory.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::unique_ptr<Matrix> BasicMatrixFactory::createMatrixAuto(const std::vector<std::vector<double>>& data) {
    double sparsity = calculateSparsity(data);
    if (sparsity > 0.7) { // 阈值70%
        return std::make_unique<SparseMatrix>(data);
    } else {
        return std::make_unique<DenseMatrix>(data);
    }
}


std::unique_ptr<Matrix> BasicMatrixFactory::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
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

void BasicMatrixFactory::saveToFile(const Matrix& matrix, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    for (int i = 0; i < matrix.rows(); ++i) {
        for (int j = 0; j < matrix.cols(); ++j) {
            file << matrix.get(i, j);
            if (j < matrix.cols() - 1) file << " ";
        }
        file << "\n";
    }
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
