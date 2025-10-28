#include "ConsoleUI.h"
#include "Matrix.h"
#include "SparseMatrix.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <cmath>
#include <sstream>
#include <algorithm>
#include <vector>
#include <string>

void ConsoleUI::showTips(const std::string& tips) {
    std::cout << tips << std::endl;
}

void ConsoleUI::showMatrix(const Matrix& matrix) {
    std::cout << std::endl;
    
    // 第一步：收集所有元素并转换为字符串，找到最大宽度
    std::vector<std::vector<std::string>> strMatrix(matrix.rows(), std::vector<std::string>(matrix.cols()));
    int maxWidth = 0;
    
    for (int i = 0; i < matrix.rows(); ++i) {
        for (int j = 0; j < matrix.cols(); ++j) {
            double val = matrix.get(i, j);
            std::ostringstream oss;
            
            // 判断数值范围，选择合适的格式
            if (std::abs(val) < 1e-10) {
                // 接近0的值显示为0
                oss << "0";
            } else if (std::abs(val) >= 1e6 || (std::abs(val) < 1e-3 && std::abs(val) > 1e-10)) {
                // 非常大或非常小的数使用科学计数法
                oss << std::scientific << std::setprecision(4) << val;
            } else {
                // 普通数值：智能显示
                // 检查是否为整数
                if (std::abs(val - std::round(val)) < 1e-10) {
                    // 是整数，不显示小数部分
                    oss << static_cast<long long>(std::round(val));
                } else {
                    // 有小数部分，去除尾部的0
                    oss << std::fixed << std::setprecision(4) << val;
                    std::string str = oss.str();
                    // 移除尾部的0
                    str.erase(str.find_last_not_of('0') + 1, std::string::npos);
                    // 如果最后一个字符是小数点，也移除
                    if (str.back() == '.') {
                        str.pop_back();
                    }
                    oss.str("");
                    oss << str;
                }
            }
            
            strMatrix[i][j] = oss.str();
            maxWidth = std::max(maxWidth, static_cast<int>(strMatrix[i][j].length()));
        }
    }
    
    // 第二步：输出对齐的矩阵
    for (int i = 0; i < matrix.rows(); ++i) {
        std::cout << "| ";
        for (int j = 0; j < matrix.cols(); ++j) {
            std::cout << std::setw(maxWidth) << std::right << strMatrix[i][j];
            if (j < matrix.cols() - 1) {
                std::cout << " ";
            }
        }
        std::cout << " |";
        std::cout << std::endl;
    }
    std::cout << std::endl;
    const SparseMatrix* sparse = dynamic_cast<const SparseMatrix*>(&matrix);
    if(sparse){
        std::cout<<(currentLang==0 ? "（稀疏矩阵）" : "(Sparse Matrix)")<<std::endl;
    }
    else{
        std::cout<<(currentLang==0 ? "（稠密矩阵）" : "(Dense Matrix)")<<std::endl;
    }
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
