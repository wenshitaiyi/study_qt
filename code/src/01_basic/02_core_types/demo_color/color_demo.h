#pragma once

#include <QString>
#include <QColor>

class ColorDemo
{
public:
    // 1. QColor 基础构造、命名色与 16 进制字符串 (#RRGGBB / #AARRGGBB) 互转
    static void testBasicConstructionAndHex();

    // 2. QColor 与 QRgb / uint32_t 无符号整型互转
    static void testUintConversion();

    // 3. ARGB 与 RGBA 字节序差异、通道取色与位运算合成
    static void testBitwiseChannelExtraction();

    // 4. HSV / HSL 色彩空间变换与色相调色
    static void testColorSpaces();

    // 5. 颜色明暗调节 (lighter / darker) 与自动对比度文字反色判定
    static void testLuminanceAndContrast();
};
