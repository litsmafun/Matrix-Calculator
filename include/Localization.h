#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include <string>
#include <unordered_map>

enum class Language { ZH_CN, EN_US };

// 获取本地化字符串，key为标识符，lang为目标语言
std::string L(const std::string& key, Language lang);

#endif // LOCALIZATION_H
