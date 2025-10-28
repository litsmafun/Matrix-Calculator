#include "ConsoleUI.h"
#include "Matrix.h"
#include <iostream>

void ConsoleUI::showTips(const std::string& tips) {
    std::cout << tips << std::endl;
}

void ConsoleUI::showMatrix(const Matrix& matrix) {
    for (int i = 0; i < matrix.rows(); ++i) {
        for (int j = 0; j < matrix.cols(); ++j) {
            std::cout << matrix.get(i, j) << " ";
        }
        std::cout << std::endl;
    }
}

int ConsoleUI::getInt() {
    int value;
    std::cin >> value;
    return value;
}

std::string ConsoleUI::getString() {
    std::string value;
    std::cin >> value;
    return value;
}

void ConsoleUI::setLanguage(int lang) {
    currentLang = lang;
}

int ConsoleUI::getLanguage() const {
    return currentLang;
}
