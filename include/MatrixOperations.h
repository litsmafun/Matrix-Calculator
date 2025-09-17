#pragma once
#include<Matrix.h>

class MatrixOperations {
public:
    virtual Matrix* add(const Matrix& a, const Matrix& b)=0;
    virtual Matrix* sub(const Matrix& a, const Matrix& b)=0;
    virtual Matrix* dot(const Matrix& a, const Matrix& b)=0;
    virtual Matrix* cross(const Matrix& a, const Matrix& b)=0;
    virtual Matrix* inverse(const Matrix& m)=0;
    virtual Matrix* transpose(const Matrix& m)=0;
    virtual double determinant(const Matrix& m)=0;
};
