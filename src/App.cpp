#include "App.h"
#include "Localization.h"
#include <iostream>
#include <memory>

App::App(std::unique_ptr<UI> ui, std::unique_ptr<MatrixFactory> factory, std::unique_ptr<MatrixOperations> operations)
    : ui(std::move(ui)), factory(std::move(factory)), operations(std::move(operations)) {}

void App::run() {
    while (true) {
        // 根据UI的语言显示本地化菜单
        int lang = ui->getLanguage();
        ui->showTips(L("menu.title", lang == 0 ? Language::ZH_CN : Language::EN_US));
        ui->showTips(L("menu.create", lang == 0 ? Language::ZH_CN : Language::EN_US));
        ui->showTips(L("menu.load", lang == 0 ? Language::ZH_CN : Language::EN_US));
        ui->showTips(L("menu.save", lang == 0 ? Language::ZH_CN : Language::EN_US));
        ui->showTips(L("menu.display", lang == 0 ? Language::ZH_CN : Language::EN_US));
        ui->showTips(L("menu.add", lang == 0 ? Language::ZH_CN : Language::EN_US));
        ui->showTips(L("menu.sub", lang == 0 ? Language::ZH_CN : Language::EN_US));
        ui->showTips(L("menu.mul", lang == 0 ? Language::ZH_CN : Language::EN_US));
        ui->showTips(L("menu.transpose", lang == 0 ? Language::ZH_CN : Language::EN_US));
        ui->showTips(L("menu.inverse", lang == 0 ? Language::ZH_CN : Language::EN_US));
        ui->showTips(L("menu.det", lang == 0 ? Language::ZH_CN : Language::EN_US));
        ui->showTips(L("menu.dot", lang == 0 ? Language::ZH_CN : Language::EN_US));
        ui->showTips(L("menu.cross", lang == 0 ? Language::ZH_CN : Language::EN_US));
        ui->showTips(L("menu.lang", lang == 0 ? Language::ZH_CN : Language::EN_US));
        ui->showTips(L("menu.exit", lang == 0 ? Language::ZH_CN : Language::EN_US));
        ui->showTips(L("prompt.choose", lang == 0 ? Language::ZH_CN : Language::EN_US));
        int choice = ui->getInt();
        handleOperation(choice);
        if (choice == 0) break;
    }
}

void App::handleOperation(int choice) {
    try {
        switch (choice) {
            case 1: {
        ui->showTips(L("prompt.rows", ui->getLanguage() == 0 ? Language::ZH_CN : Language::EN_US));
        int rows = ui->getInt();
        ui->showTips(L("prompt.cols", ui->getLanguage() == 0 ? Language::ZH_CN : Language::EN_US));
        int cols = ui->getInt();
                std::vector<std::vector<double>> data(rows, std::vector<double>(cols));
                for (int i = 0; i < rows; ++i) {
                    for (int j = 0; j < cols; ++j) {
            char buf[128];
            snprintf(buf, sizeof(buf), L("prompt.element", ui->getLanguage() == 0 ? Language::ZH_CN : Language::EN_US).c_str(), i, j);
            ui->showTips(std::string(buf));
            data[i][j] = ui->getInt(); // 简化，假设整数
                    }
                }
                currentMatrix = factory->createMatrixAuto(data);
        ui->showTips(L("msg.created", ui->getLanguage() == 0 ? Language::ZH_CN : Language::EN_US));
                break;
            }
            case 2:
                loadMatrix();
                break;
            case 3:
                saveMatrix();
                break;
            case 4:
                if (currentMatrix) {
                    ui->showMatrix(*currentMatrix);
                } else {
                    ui->showTips("No current matrix.");
                }
                break;
            case 5: {
                if (!currentMatrix) {
                    ui->showTips(L("msg.invalid", ui->getLanguage() == 0 ? Language::ZH_CN : Language::EN_US));
                    break;
                }
                ui->showTips("Enter second matrix file: ");
                std::string filename = ui->getString();
                auto b = factory->loadFromFile(filename);
                currentMatrix = operations->add(*currentMatrix, *b);
                ui->showTips(L("msg.added", ui->getLanguage() == 0 ? Language::ZH_CN : Language::EN_US));
                break;
            }
            case 13: {
                // 切换语言
                int cur = ui->getLanguage();
                int next = (cur == 0) ? 1 : 0;
                ui->setLanguage(next);
                // 通知用户
                if (next == 0) ui->showTips(L("lang.switched", Language::ZH_CN));
                else ui->showTips(L("lang.switched", Language::EN_US));
                break;
            }
            // 类似处理其他运算
            case 0:
                break;
            default:
                ui->showTips("Invalid choice.");
        }
    } catch (const std::exception& e) {
        ui->showTips("Error: " + std::string(e.what()));
    }
}

void App::loadMatrix() {
    ui->showTips("Enter filename: ");
    std::string filename = ui->getString();
    currentMatrix = factory->loadFromFile(filename);
    ui->showTips("Matrix loaded.");
}

void App::saveMatrix() {
    if (!currentMatrix) {
        ui->showTips("No current matrix.");
        return;
    }
    ui->showTips("Enter filename: ");
    std::string filename = ui->getString();
    factory->saveToFile(*currentMatrix, filename);
    ui->showTips("Matrix saved.");
}
