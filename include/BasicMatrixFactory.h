#ifndef BASICMATRIXFACTORY_H
#define BASICMATRIXFACTORY_H

#include "MatrixFactory.h"

class BasicMatrixFactory : public MatrixFactory {
public:
    Matrix* createMatrixAuto(int i, int j, const std::string& data) override;
    Matrix* createMatrixAuto(const Matrix& m) override;
    Matrix* loadFromFile(const std::string& path) override;
    void saveToFile(const std::string& path, const Matrix& m) override;
};

#endif // BASICMATRIXFACTORY_H
