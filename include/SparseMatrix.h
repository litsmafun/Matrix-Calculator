#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

#include "Matrix.h"
#include <vector>
#include <memory>
class DenseMatrix;


// œ° Ëæÿ’Û¿‡£®CSR∏Ò Ω£©
class SparseMatrix:public Matrix{
private:
    std::vector<double> values;
    std::vector<int> colIndices;
    std::vector<int> rowPointers;

public:
    SparseMatrix(const std::vector<double>& vals,const std::vector<int>& colInds,const std::vector<int>& rowPtrs,int r,int c);
    
    double get(int i,int j) const override;
    void set(int i,int j,double value) override;
    int nnz() const override;
    
    std::vector<double> getValues() const;
    std::vector<int> getColIndices() const;
    std::vector<int> getRowPointers() const;
    
    std::shared_ptr<DenseMatrix> toDense() const;

private:
    void insertElement(int row, int col, double value, int pos);
    void removeElement(int row, int pos);
};

#endif // SPARSEMATRIX_H