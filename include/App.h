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
    std::shared_ptr<Matrix> currentMatrix;
    std::unique_ptr<MatrixFactory> factory;
    std::unique_ptr<MatrixOperations> operations;

public:
    App(std::unique_ptr<UI> ui, std::unique_ptr<MatrixFactory> factory, std::unique_ptr<MatrixOperations> operations);
    ~App() = default;
    void handleOperation(int choice);
    void changeCurrentMatrix(std::shared_ptr<Matrix> m);
    void loadMatrix();
    void saveMatrix();
    void run();
};

#endif // APP_H
