#ifndef APP_H
#define APP_H

#include "UI.h"
#include "Matrix.h"
#include "MatrixFactory.h"
#include "MatrixOperations.h"

class App {
private:
    UI* ui;
    Matrix* currentMatrix;
    MatrixFactory* factory;
    MatrixOperations* operations;

public:
    App(UI* ui, MatrixFactory* factory, MatrixOperations* operations);
    ~App();
    void handleOperation(int choice);
    void changeCurrentMatrix(Matrix* m);
    Matrix* loadMatrix(int choice);
    void saveMatrix(const Matrix& m);
    void run();
};

#endif // APP_H
