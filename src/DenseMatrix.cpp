#include "DenseMatrix.h"
#include "SparseMatrix.h"
#include <stdexcept>
#include <cmath>

//≥Ì√‹æÿ’Û
DenseMatrix::DenseMatrix(const std::vector<std::vector<double>>& d):Matrix(d.size(),d[0].size()),data(d){}

double DenseMatrix::get(int i,int j) const{
    if(i<0||i>=row||j<0||j>=col)
        throw std::out_of_range("Index out of range");
    return data[i][j];
}

void DenseMatrix::set(int i,int j,double value){
    if(i<0||i>=row||j<0||j>=col)
        throw std::out_of_range("Index out of range");
    data[i][j]=value;
}

int DenseMatrix::nnz() const{
    int count=0;
    for(const auto& r:data)
        for(double val:r)
            if(std::abs(val)>1e-10) count++;
    return count;
}

std::vector<std::vector<double>> DenseMatrix::getData() const{
    return data;
}

std::shared_ptr<SparseMatrix> DenseMatrix::toSparse() const{
    std::vector<double> values;
    std::vector<int> colIndices;
    std::vector<int> rowPointers(row+1,0);

    for(int i=0;i<row;++i){
        for(int j=0;j<col;++j){
            double val=data[i][j];
            if(std::abs(val)>1e-10){
                values.push_back(val);
                colIndices.push_back(j);
                rowPointers[i+1]++;
            }
        }
    }

    for(int i=1;i<=row;++i)
        rowPointers[i]+=rowPointers[i-1];

    return std::make_shared<SparseMatrix>(values,colIndices,rowPointers,row,col);
}