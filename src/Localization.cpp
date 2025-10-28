#include "Localization.h"

using Map = std::unordered_map<std::string, std::string>;

static const Map zh_cn = {
    {"menu.title", "矩阵计算器 菜单:"},
    {"menu.create", "1. 创建矩阵"},
    {"menu.load", "2. 从文件加载矩阵"},
    {"menu.save", "3. 保存当前矩阵到文件"},
    {"menu.display", "4. 显示当前矩阵"},
    {"menu.add", "5. 矩阵加法"},
    {"menu.sub", "6. 矩阵减法"},
    {"menu.mul", "7. 矩阵乘法"},
    {"menu.transpose", "8. 矩阵转置"},
    {"menu.inverse", "9. 矩阵求逆"},
    {"menu.det", "10. 行列式"},
    {"menu.dot", "x. 向量点积"},
    {"menu.cross", "11. 向量叉积(3D)"},
    {"menu.lang", "12. 切换语言"},
    {"menu.exit", "0. 退出"},
    {"prompt.choose", "请选择操作: "},
    {"prompt.rows", "输入行数: "},
    {"prompt.cols", "输入列数: "},
    {"prompt.element", "输入元素 [%d][%d]: "},
    {"prompt.filename", "输入文件名: "},
    {"msg.created", "矩阵已创建。"},
    {"msg.loaded", "矩阵已加载。"},
    {"msg.saved", "矩阵已保存。"},
    {"msg.added", "加法完成。"},
    {"msg.invalid", "无效选择。"},
    {"msg.error", "错误: %s"},
    {"lang.chosen", "当前语言: 中文(简体)"},
    {"lang.switched", "语言已切换为: 中文"}
};

static const Map en_us = {
    {"menu.title", "Matrix Calculator Menu:"},
    {"menu.create", "1. Create Matrix"},
    {"menu.load", "2. Load Matrix from File"},
    {"menu.save", "3. Save Current Matrix to File"},
    {"menu.display", "4. Display Current Matrix"},
    {"menu.add", "5. Add Matrices"},
    {"menu.sub", "6. Subtract Matrices"},
    {"menu.mul", "7. Multiply Matrices"},
    {"menu.transpose", "8. Transpose Matrix"},
    {"menu.inverse", "9. Inverse Matrix"},
    {"menu.det", "10. Determinant"},
    {"menu.dot", "x. Dot Product (Vectors)"},
    {"menu.cross", "11. Cross Product (3D Vectors)"},
    {"menu.lang", "12. Switch Language"},
    {"menu.exit", "0. Exit"},
    {"prompt.choose", "Choose an option: "},
    {"prompt.rows", "Enter rows: "},
    {"prompt.cols", "Enter cols: "},
    {"prompt.element", "Enter element [%d][%d]: "},
    {"prompt.filename", "Enter filename: "},
    {"msg.created", "Matrix created."},
    {"msg.loaded", "Matrix loaded."},
    {"msg.saved", "Matrix saved."},
    {"msg.added", "Addition completed."},
    {"msg.invalid", "Invalid choice."},
    {"msg.error", "Error: %s"},
    {"lang.chosen", "Current language: English"},
    {"lang.switched", "Language switched to: English"}
};

std::string L(const std::string& key, Language lang) {
    const Map* m = (lang == Language::ZH_CN) ? &zh_cn : &en_us;
    auto it = m->find(key);
    if (it != m->end()) return it->second;
    // fallback: key
    return key;
}
