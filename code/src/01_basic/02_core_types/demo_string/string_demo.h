#pragma once

#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QStringView>
#include <QStringRef>

#include "byte_array_ref.h"

class StringDemo
{
public:
    // 1. QString 常用基础与高级格式化/切片
    static void testQStringBasics();

    // 2. QStringRef (Qt 5 经典子串引用视图)
    static void testQStringRef();

    // 3. QStringView (C++17 现代零拷贝视图)
    static void testQStringView();

    // 4. 自定义 QByteArrayRef (二进制字节流零拷贝切片视图)
    static void testQByteArrayRef();

    // 5. QStringList 列表操作与过滤
    static void testQStringList();

    // 6. QByteArray 字节流、HEX 与 Base64 编解码
    static void testQByteArray();

    // 7. 字符集编码转换 (UTF-8, GBK, Local8Bit) 与乱码防范
    static void testEncodingConversion();

    // 8. QRegularExpression 正则表达式提取与替换
    static void testRegex();
};
