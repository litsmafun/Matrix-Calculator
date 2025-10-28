#include "App.h"
#include "ConsoleUI.h"
#include "BasicMatrixFactory.h"
#include "BasicMatrixOperations.h"
#include <memory>
#include <windows.h>
#include <iostream>

int main() {
    // 设置控制台代码页为 UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    // 设置 cout 使用 UTF-8（使用 try-catch 避免 locale 错误）
    try {
        std::cout.imbue(std::locale(""));
    } catch (...) {
        // 如果设置 locale 失败，使用默认设置
        std::cout.imbue(std::locale::classic());
    }
    
    // 创建用户界面组件
    auto ui = std::make_unique<ConsoleUI>();

    // 创建矩阵工厂组件
    auto factory = std::make_unique<BasicMatrixFactory>();

    // 创建矩阵运算组件
    auto operations = std::make_unique<BasicMatrixOperations>();

    // 创建并运行应用程序
    App app(std::move(ui), std::move(factory), std::move(operations));
    app.run();

    return 0;
}
