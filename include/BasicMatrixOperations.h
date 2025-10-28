#pragma once
#include <Matrix.h>
#include <MatrixOperations.h>
#include <memory>

class BasicMatrixOperations:public MatrixOperations {
public:
    std::shared_ptr<Matrix> add(const Matrix& a, const Matrix& b) override;
    std::shared_ptr<Matrix> sub(const Matrix& a, const Matrix& b) override;
    std::shared_ptr<Matrix> dot(const Matrix& a, const Matrix& b) override;
    std::shared_ptr<Matrix> cross(const Matrix& a, const Matrix& b) override;
    std::shared_ptr<Matrix> inverse(const Matrix& m) override;
    std::shared_ptr<Matrix> transpose(const Matrix& m) override;
    double determinant(const Matrix& m) override;
};
