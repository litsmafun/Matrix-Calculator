#ifndef CONSOLEUI_H
#define CONSOLEUI_H

#include "UI.h"
#include <iostream>

class ConsoleUI : public UI {
public:
    void showTips(const std::string& tips) override;
    void showMatrix(const Matrix& m) override;
    int getInt() override;
    std::string getString() override;
};

#endif // CONSOLEUI_H
