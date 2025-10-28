#ifndef BASICMATRIXFACTORY_H
#define BASICMATRIXFACTORY_H

#include "MatrixFactory.h"
#include <vector>
#include <memory>

class BasicMatrixFactory : public MatrixFactory {
private:
    double calculateSparsity(const std::vector<std::vector<double>>& data);
    
public:
    std::shared_ptr<Matrix> createMatrixAuto(int rows, int cols, const std::string& data) override;
    std::shared_ptr<Matrix> createMatrixAuto(const Matrix& m) override;
    std::shared_ptr<Matrix> createMatrixAuto(const std::vector<std::vector<double>>& data);
    std::shared_ptr<Matrix> loadFromFile(const std::string& path) override;
    void saveToFile(const std::string& path, const Matrix& m) override;
};

#endif // BASICMATRIXFACTORY_H
