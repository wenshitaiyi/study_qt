#pragma once

#include <QString>
#include <QTextStream>
#include <QFile>

class TextStreamDemo
{
public:
    // 1. QTextStream 格式化流操纵符 (对齐、进制转换、浮点精度与字段填充)
    static void testFormatManipulators();

    // 2. 多字符集编码处理 (UTF-8, GBK, 带/不带 BOM 头)
    static void testEncodingAndBOM();

    // 3. 跨平台换行符自动适配 (CRLF vs LF)
    static void testLineEndings();

    // 4. 实战：CSV 格式报表文件结构化生成与逆向逐行解析
    static void testCsvReportProcessing();

    // 5. 内存字符串流 (QString 内存高效拼接与数字提取)
    static void testMemoryStringStream();
};
