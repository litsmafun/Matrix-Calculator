#include "Matrix.h"

//矩阵基类
Matrix::Matrix(int r,int c):row(r),col(c){}

int Matrix::rows() const{
    return row;
}

int Matrix::cols() const{
    return col;
}

void Matrix::set(int i,int j,double value){
    (void)i; (void)j; (void)value;
}


