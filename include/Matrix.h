#ifndef MATRIX_H
#define MATRIX_H

// 矩阵基类
class Matrix {
protected:
    int row;
    int col;

public:
    Matrix(int r,int c);
    virtual ~Matrix()=default;

    int rows() const;
    int cols() const;

    virtual double get(int i,int j) const=0;
    virtual void set(int i,int j,double value);
    virtual int nnz() const=0;
};

#endif // MATRIX_H