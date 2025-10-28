#pragma once
#include <Matrix.h>
#include <memory>

class MatrixOperations {
public:
    virtual std::shared_ptr<Matrix> add(const Matrix& a, const Matrix& b)=0;
    virtual std::shared_ptr<Matrix> sub(const Matrix& a, const Matrix& b)=0;
    virtual std::shared_ptr<Matrix> dot(const Matrix& a, const Matrix& b)=0;
    virtual std::shared_ptr<Matrix> cross(const Matrix& a, const Matrix& b)=0;
    virtual std::shared_ptr<Matrix> inverse(const Matrix& m)=0;
    virtual std::shared_ptr<Matrix> transpose(const Matrix& m)=0;
    virtual double determinant(const Matrix& m)=0;
};
