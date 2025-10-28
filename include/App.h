#ifndef APP_H
#define APP_H

#include "UI.h"
#include "Matrix.h"
#include "MatrixFactory.h"
#include "MatrixOperations.h"
#include <memory>

class App {
private:
    std::unique_ptr<UI> ui;
    Matrix* currentMatrix;
    std::unique_ptr<MatrixFactory> factory;
    std::unique_ptr<MatrixOperations> operations;

public:
    App(std::unique_ptr<UI> ui, std::unique_ptr<MatrixFactory> factory, std::unique_ptr<MatrixOperations> operations);
    ~App();
    void handleOperation(int choice);
    void changeCurrentMatrix(Matrix* m);
    void loadMatrix();
    void saveMatrix();
    void run();
};

#endif // APP_H
