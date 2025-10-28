#include "ConsoleUI.h"
#include "Matrix.h"
#include <iostream>
#include <limits>

void ConsoleUI::showTips(const std::string& tips) {
    std::cout << tips << std::endl;
}

void ConsoleUI::showMatrix(const Matrix& matrix) {
    std::cout << std::endl;
    for (int i = 0; i < matrix.rows(); ++i) {
        std::cout << "| ";
        for (int j = 0; j < matrix.cols(); ++j) {
            std::cout << matrix.get(i, j) << " ";
        }
        std::cout << "|";
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int ConsoleUI::getInt() {
    int value;
    while (!(std::cin >> value)) {
        // 清除错误标志
        std::cin.clear();
        // 忽略错误输入直到换行符
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << (currentLang == 0 ? "输入错误，请输入一个整数: " : "Invalid input, please enter an integer: ");
    }
    // 清除输入缓冲区中的剩余字符
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return value;
}

double ConsoleUI::getDouble() {
    double value;
    while (!(std::cin >> value)) {
        // 清除错误标志
        std::cin.clear();
        // 忽略错误输入直到换行符
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << (currentLang == 0 ? "输入错误，请输入一个数字: " : "Invalid input, please enter a number: ");
    }
    // 清除输入缓冲区中的剩余字符
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
