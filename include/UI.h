#ifndef UI_H
#define UI_H

#include <string>

class Matrix; // 前向声明

class UI {
public:
    virtual ~UI() = default;
    virtual void showTips(const std::string& tips) = 0;
    virtual void showMatrix(const Matrix& m) = 0;
    virtual int getInt() = 0;
    virtual std::string getString() = 0;
};

#endif // UI_H
