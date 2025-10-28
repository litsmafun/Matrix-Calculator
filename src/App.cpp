#include "App.h"
#include "Localization.h"
#include "BasicMatrixFactory.h"
#include <iostream>
#include <memory>
#include <vector>

App::App(std::unique_ptr<UI> ui, std::unique_ptr<MatrixFactory> factory, std::unique_ptr<MatrixOperations> operations)
    : ui(std::move(ui)), currentMatrix(nullptr), factory(std::move(factory)), operations(std::move(operations)) {}

App::~App() {
    delete currentMatrix;
}

void App::run() {
    while (true) {
        // 根据UI的语言显示本地化菜单
        int lang = ui->getLanguage();
        
        // 顶部边框
        ui->showTips("============================================");
        ui->showTips("||  " + L("menu.title", lang == 0 ? Language::ZH_CN : Language::EN_US) + "  ||");
        ui->showTips("============================================");
        
        // 菜单选项
        ui->showTips("|| " + L("menu.create", lang == 0 ? Language::ZH_CN : Language::EN_US));
        ui->showTips("|| " + L("menu.load", lang == 0 ? Language::ZH_CN : Language::EN_US));
        ui->showTips("|| " + L("menu.save", lang == 0 ? Language::ZH_CN : Language::EN_US));
        ui->showTips("|| " + L("menu.display", lang == 0 ? Language::ZH_CN : Language::EN_US));
        ui->showTips("|| " + L("menu.add", lang == 0 ? Language::ZH_CN : Language::EN_US));
        ui->showTips("|| " + L("menu.sub", lang == 0 ? Language::ZH_CN : Language::EN_US));
        ui->showTips("|| " + L("menu.mul", lang == 0 ? Language::ZH_CN : Language::EN_US));
        ui->showTips("|| " + L("menu.transpose", lang == 0 ? Language::ZH_CN : Language::EN_US));
        ui->showTips("|| " + L("menu.inverse", lang == 0 ? Language::ZH_CN : Language::EN_US));
        ui->showTips("|| " + L("menu.det", lang == 0 ? Language::ZH_CN : Language::EN_US));
        ui->showTips("|| " + L("menu.dot", lang == 0 ? Language::ZH_CN : Language::EN_US));
        ui->showTips("|| " + L("menu.cross", lang == 0 ? Language::ZH_CN : Language::EN_US));
        ui->showTips("|| " + L("menu.lang", lang == 0 ? Language::ZH_CN : Language::EN_US));
        ui->showTips("|| " + L("menu.exit", lang == 0 ? Language::ZH_CN : Language::EN_US));
        
        // 底部边框
        ui->showTips("============================================");
        ui->showTips(L("prompt.choose", lang == 0 ? Language::ZH_CN : Language::EN_US));
        
        int choice = ui->getInt();
        handleOperation(choice);
        if (choice == 0) break;
    }
}

void App::handleOperation(int choice) {
    try {
        Language currentLang = ui->getLanguage() == 0 ? Language::ZH_CN : Language::EN_US;
        
        switch (choice) {
            case 1: { // 创建矩阵
                ui->showTips(L("prompt.rows", currentLang));
                int rows = ui->getInt();
                ui->showTips(L("prompt.cols", currentLang));
                int cols = ui->getInt();
                std::string dataStr;
                for (int i = 0; i < rows; ++i) {
                    for (int j = 0; j < cols; ++j) {
                        char buf[128];
                        snprintf(buf, sizeof(buf), L("prompt.element", currentLang).c_str(), i, j);
                        ui->showTips(std::string(buf));
                        double val = ui->getDouble();
                        dataStr += std::to_string(val) + " ";
                    }
                }
                delete currentMatrix;
                currentMatrix = factory->createMatrixAuto(rows, cols, dataStr);
                ui->showTips(L("msg.created", currentLang));
                break;
            }
            
            case 2: // 从文件加载矩阵
                loadMatrix();
                break;
                
            case 3: // 保存当前矩阵到文件
                saveMatrix();
                break;
                
            case 4: // 显示当前矩阵
                if (currentMatrix) {
                    ui->showMatrix(*currentMatrix);
                } else {
                    ui->showTips(L("msg.invalid", currentLang));
                }
                break;
                
            case 5: { // 矩阵加法
                if (!currentMatrix) {
                    ui->showTips(L("msg.invalid", currentLang));
                    break;
                }
                ui->showTips(L("prompt.filename", currentLang));
                std::string filename = ui->getString();
                Matrix* b = factory->loadFromFile(filename);
                Matrix* result = operations->add(*currentMatrix, *b);
                delete b;
                delete currentMatrix;
                // 自动判断结果矩阵是否需要转换类型
                currentMatrix = factory->createMatrixAuto(*result);
                delete result;
                ui->showTips(L("msg.added", currentLang));
                break;
            }
            
            case 6: { // 矩阵减法
                if (!currentMatrix) {
                    ui->showTips(L("msg.invalid", currentLang));
                    break;
                }
                ui->showTips(L("prompt.filename", currentLang));
                std::string filename = ui->getString();
                Matrix* b = factory->loadFromFile(filename);
                Matrix* result = operations->sub(*currentMatrix, *b);
                delete b;
                delete currentMatrix;
                // 自动判断结果矩阵是否需要转换类型
                currentMatrix = factory->createMatrixAuto(*result);
                delete result;
                ui->showTips(currentLang == Language::ZH_CN ? "减法完成。" : "Subtraction completed.");
                break;
            }
            
            case 7: { // 矩阵乘法
                if (!currentMatrix) {
                    ui->showTips(L("msg.invalid", currentLang));
                    break;
                }
                ui->showTips(L("prompt.filename", currentLang));
                std::string filename = ui->getString();
                Matrix* b = factory->loadFromFile(filename);
                Matrix* result = operations->dot(*currentMatrix, *b);
                delete b;
                delete currentMatrix;
                // 自动判断结果矩阵是否需要转换类型
                currentMatrix = factory->createMatrixAuto(*result);
                delete result;
                ui->showTips(currentLang == Language::ZH_CN ? "乘法完成。" : "Multiplication completed.");
                break;
            }
            
            case 8: { // 矩阵转置
                if (!currentMatrix) {
                    ui->showTips(L("msg.invalid", currentLang));
                    break;
                }
                Matrix* result = operations->transpose(*currentMatrix);
                delete currentMatrix;
                // 自动判断结果矩阵是否需要转换类型
                currentMatrix = factory->createMatrixAuto(*result);
                delete result;
                ui->showTips(currentLang == Language::ZH_CN ? "转置完成。" : "Transpose completed.");
                break;
            }
            
            case 9: { // 矩阵求逆
                if (!currentMatrix) {
                    ui->showTips(L("msg.invalid", currentLang));
                    break;
                }
                Matrix* result = operations->inverse(*currentMatrix);
                delete currentMatrix;
                // 自动判断结果矩阵是否需要转换类型
                currentMatrix = factory->createMatrixAuto(*result);
                delete result;
                ui->showTips(currentLang == Language::ZH_CN ? "求逆完成。" : "Inverse completed.");
                break;
            }
            
            case 10: { // 行列式
                if (!currentMatrix) {
                    ui->showTips(L("msg.invalid", currentLang));
                    break;
                }
                double det = operations->determinant(*currentMatrix);
                ui->showTips((currentLang == Language::ZH_CN ? "行列式: " : "Determinant: ") + std::to_string(det));
                break;
            }
            
            case 11: { // 向量点积
                if (!currentMatrix) {
                    ui->showTips(L("msg.invalid", currentLang));
                    break;
                }
                ui->showTips(L("prompt.filename", currentLang));
                std::string filename = ui->getString();
                Matrix* b = factory->loadFromFile(filename);
                Matrix* result = operations->dot(*currentMatrix, *b);
                delete b;
                ui->showTips(currentLang == Language::ZH_CN ? "点积结果: " : "Dot product result: ");
                ui->showMatrix(*result);
                delete result;
                break;
            }
            
            case 12: { // 向量叉积(3D)
                if (!currentMatrix) {
                    ui->showTips(L("msg.invalid", currentLang));
                    break;
                }
                ui->showTips(L("prompt.filename", currentLang));
                std::string filename = ui->getString();
                Matrix* b = factory->loadFromFile(filename);
                Matrix* result = operations->cross(*currentMatrix, *b);
                delete b;
                delete currentMatrix;
                // 自动判断结果矩阵是否需要转换类型
                currentMatrix = factory->createMatrixAuto(*result);
                delete result;
                ui->showTips(currentLang == Language::ZH_CN ? "叉积完成。" : "Cross product completed.");
                break;
            }
            
            case 13: { // 切换语言
                int cur = ui->getLanguage();
                int next = (cur == 0) ? 1 : 0;
                ui->setLanguage(next);
                ui->showTips(L("lang.switched", next == 0 ? Language::ZH_CN : Language::EN_US));
                break;
            }
            
            case 0: // 退出
                ui->showTips(currentLang == Language::ZH_CN ? "再见！" : "Goodbye!");
                break;
                
            default:
                ui->showTips(L("msg.invalid", currentLang));
        }
    } catch (const std::exception& e) {
        ui->showTips("Error: " + std::string(e.what()));
    }
}

void App::loadMatrix() {
    Language currentLang = ui->getLanguage() == 0 ? Language::ZH_CN : Language::EN_US;
    ui->showTips(L("prompt.filename", currentLang));
    std::string filename = ui->getString();
    delete currentMatrix;
    currentMatrix = factory->loadFromFile(filename);
    ui->showTips(L("msg.loaded", currentLang));
}

void App::saveMatrix() {
    Language currentLang = ui->getLanguage() == 0 ? Language::ZH_CN : Language::EN_US;
    if (!currentMatrix) {
        ui->showTips(L("msg.invalid", currentLang));
        return;
    }
    ui->showTips(L("prompt.filename", currentLang));
    std::string filename = ui->getString();
    factory->saveToFile(filename, *currentMatrix);
    ui->showTips(L("msg.saved", currentLang));
}
