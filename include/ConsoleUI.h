#ifndef CONSOLEUI_H
#define CONSOLEUI_H

#include "UI.h"
#include <iostream>

class ConsoleUI : public UI {
private:
    int currentLang = 0; // 0 for Chinese, 1 for English
    
public:
    void showTips(const std::string& tips) override;
    void showMatrix(const Matrix& m) override;
    int getInt() override;
    double getDouble() override;
    std::string getString() override;
    int getLanguage() const override;
    void setLanguage(int lang) override;
};

#endif // CONSOLEUI_H
