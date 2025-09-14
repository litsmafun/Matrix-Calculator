#ifndef DENSEMATRIX_H
#define DENSEMATRIX_H

#include "Matrix.h"
#include <vector>
#include <memory>
class SparseMatrix;

// ≥Ì√‹æÿ’Û
class DenseMatrix:public Matrix{
private:
    std::vector<std::vector<double>> data;

public:
    DenseMatrix(const std::vector<std::vector<double>>& d);
    double get(int i, int j) const override;
    void set(int i, int j, double value) override;
    int nnz() const override;
    
    std::vector<std::vector<double>> getData() const;
    std::shared_ptr<SparseMatrix> toSparse() const;
};

#endif // DENSEMATRIX_H