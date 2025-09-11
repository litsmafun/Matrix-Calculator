#ifndef MATRIXFACTORY_H
#define MATRIXFACTORY_H

#include <string>

class Matrix; // 前向声明

class MatrixFactory {
public:
    virtual ~MatrixFactory() = default;
    virtual Matrix* createMatrixAuto(int i, int j, const std::string& data) = 0;
    virtual Matrix* createMatrixAuto(const Matrix& m) = 0;
    virtual Matrix* loadFromFile(const std::string& path) = 0;
    virtual void saveToFile(const std::string& path, const Matrix& m) = 0;
};

#endif // MATRIXFACTORY_H
