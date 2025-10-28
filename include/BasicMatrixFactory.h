#ifndef BASICMATRIXFACTORY_H
#define BASICMATRIXFACTORY_H

#include "MatrixFactory.h"
#include <vector>

class BasicMatrixFactory : public MatrixFactory {
private:
    double calculateSparsity(const std::vector<std::vector<double>>& data);
    
public:
    Matrix* createMatrixAuto(int rows, int cols, const std::string& data) override;
    Matrix* createMatrixAuto(const Matrix& m) override;
    Matrix* createMatrixAuto(const std::vector<std::vector<double>>& data);
    Matrix* loadFromFile(const std::string& path) override;
    void saveToFile(const std::string& path, const Matrix& m) override;
};

#endif // BASICMATRIXFACTORY_H
