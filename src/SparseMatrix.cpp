#include "SparseMatrix.h"
#include "DenseMatrix.h"
#include <stdexcept>
#include <cmath>

//œ° Ëæÿ’Û
SparseMatrix::SparseMatrix(const std::vector<double>& vals, const std::vector<int>& colInds,const std::vector<int>& rowPtrs,int r,int c)
    :Matrix(r, c),values(vals),colIndices(colInds),rowPointers(rowPtrs){}

double SparseMatrix::get(int i, int j) const{
    if(i<0||i>=row||j<0||j>=col)
        throw std::out_of_range("Index out of range");
    
    for(int idx=rowPointers[i];idx<rowPointers[i+1];++idx)
        if(colIndices[idx]==j)
            return values[idx];
    return 0.0;
}

void SparseMatrix::set(int i,int j,double value){
    if(i<0||i>=row||j<0||j>=col)
        throw std::out_of_range("Index out of bounds");

    int start=rowPointers[i];
    int end=rowPointers[i+1];
    int pos=start;
    bool found=false;

    while(pos<end&&colIndices[pos]<j)
        pos++;
    if(pos<end&&colIndices[pos]==j)
        found = true;

    if(found){
        if(std::abs(value)>1e-10)
            values[pos]=value;
        else
            removeElement(i,pos);
    } 
    else if(std::abs(value)>1e-10) 
        insertElement(i,j,value,pos);
}

void SparseMatrix::insertElement(int row,int col,double value,int pos){
    values.insert(values.begin()+pos,value);
    colIndices.insert(colIndices.begin()+pos,col);
    for(int r=row+1;r<=this->row;r++)
        rowPointers[r]++;
}

void SparseMatrix::removeElement(int row,int pos){
    values.erase(values.begin()+pos);
    colIndices.erase(colIndices.begin()+pos);
    for(int r=row+1;r<=this->row;r++) 
        rowPointers[r]--;
}

int SparseMatrix::nnz() const{
    return values.size();
}

std::vector<double> SparseMatrix::getValues() const{
    return values;
}

std::vector<int> SparseMatrix::getColIndices() const{
    return colIndices;
}

std::vector<int> SparseMatrix::getRowPointers() const{
    return rowPointers;
}

std::shared_ptr<DenseMatrix> SparseMatrix::toDense() const{
    std::vector<std::vector<double>> denseData(row,std::vector<double>(col, 0.0));
    for(int i=0;i<row;++i){
        for(int idx=rowPointers[i];idx<rowPointers[i+1];++idx){
            int j=colIndices[idx];
            denseData[i][j]=values[idx];
        }
    }
    return std::make_shared<DenseMatrix>(denseData);
}