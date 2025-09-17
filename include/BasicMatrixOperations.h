#pragma once
#include<Matrix.h>
#include<MatrixOperations.h>

class BasicMatrixOperations:public MatrixOperations {
public:
    Matrix* add(const Matrix& a, const Matrix& b) override;
    Matrix* sub(const Matrix& a, const Matrix& b) override;
    Matrix* dot(const Matrix& a, const Matrix& b) override;
    Matrix* cross(const Matrix& a, const Matrix& b) override;
    Matrix* inverse(const Matrix& m) override;
    Matrix* transpose(const Matrix& m) override;
    double determinant(const Matrix& m) override;
};
