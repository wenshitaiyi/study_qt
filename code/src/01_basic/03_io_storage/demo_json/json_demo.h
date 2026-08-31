#pragma once

#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

class JsonDemo
{
public:
    // 1. JSON 基础构建与格式化输出 (Indented vs Compact)
    static void testBasicConstructionAndFormatting();

    // 2. JSON 字符串解析与语法错误捕获 (QJsonParseError)
    static void testParsingAndErrorHandling();

    // 3. QVariant 与 JSON 容器无缝互转 (toVariantMap / fromVariantList)
    static void testVariantConversion();

    // 4. 高级工具实战：JSON 深度递归合并 (Deep Merge 差异补丁合并)
    static void testJsonDeepMerge();

    // 5. 高级工具实战：点分路径式数据读取与动态更新 (getByPath / setByPath)
    static void testPathBasedQueryAndUpdate();
};
