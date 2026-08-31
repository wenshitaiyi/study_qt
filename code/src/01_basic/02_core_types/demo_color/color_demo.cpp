#include "color_demo.h"

#include <QRgb>
#include <iostream>
#include <iomanip>

static void printTitle(const QString &title)
{
    std::cout << "\n============================================================" << std::endl;
    std::cout << "  " << title.toUtf8().constData() << std::endl;
    std::cout << "============================================================" << std::endl;
}

static void printLine(const QString &title, const QString &content)
{
    std::cout << QStringLiteral("  • %1 : %2")
                    .arg(title.leftJustified(28, ' '))
                    .arg(content)
                    .toUtf8().constData()
              << std::endl;
}

// ARGB (0xAARRGGBB) 转换为 RGBA (0xRRGGBBAA)
static uint32_t convertArgbToRgba(uint32_t argb) noexcept
{
    uint32_t a = (argb >> 24) & 0xFF;
    uint32_t rgb = argb & 0x00FFFFFF;
    return (rgb << 8) | a;
}

// RGBA (0xRRGGBBAA) 转换为 ARGB (0xAARRGGBB)
static uint32_t convertRgbaToArgb(uint32_t rgba) noexcept
{
    uint32_t a = rgba & 0xFF;
    uint32_t rgb = (rgba >> 8) & 0x00FFFFFF;
    return (a << 24) | rgb;
}

void ColorDemo::testBasicConstructionAndHex()
{
    printTitle(QStringLiteral("[模块 1] QColor 基础构造、命名色与 16 进制字符串"));

    // 1. 命名色构造
    QColor steelBlue(QStringLiteral("steelblue"));
    printLine(QStringLiteral("命名色 'steelblue' RGB"), QStringLiteral("R:%1, G:%2, B:%3, A:%4")
        .arg(steelBlue.red()).arg(steelBlue.green()).arg(steelBlue.blue()).arg(steelBlue.alpha()));

    // 2. 十六进制字符串 (#RRGGBB 与 #AARRGGBB) 解析
    QColor hexColor1(QStringLiteral("#3498db"));
    QColor hexColor2(QStringLiteral("#80e74c3c")); // 半透明红色 (Alpha = 0x80 = 128)

    printLine(QStringLiteral("解析 #3498db (HexRgb)"), hexColor1.name(QColor::HexRgb));
    printLine(QStringLiteral("解析 #80e74c3c (HexArgb)"), hexColor2.name(QColor::HexArgb));
    printLine(QStringLiteral("hexColor2 透明度通道"), QStringLiteral("Alpha = %1 / 255 (约 %2%)")
        .arg(hexColor2.alpha())
        .arg(hexColor2.alphaF() * 100.0, 0, 'f', 1));
}

void ColorDemo::testUintConversion()
{
    printTitle(QStringLiteral("[模块 2] QColor 与 QRgb / uint32_t 无符号整型互转"));

    // 1. 从 uint32_t / QRgb (0xAARRGGBB) 构建 QColor
    // 假设无符号整型为 0xCC2ECC71 (翠绿色，Alpha = 0xCC = 204)
    uint32_t rawUintColor = 0xCC2ECC71;
    QColor colorFromUint = QColor::fromRgba(rawUintColor);

    printLine(QStringLiteral("原始 uint32_t (16进制)"), QStringLiteral("0x%1").arg(rawUintColor, 8, 16, QChar('0')).toUpper());
    printLine(QStringLiteral("转换为 QColor 分量"), QStringLiteral("R:%1, G:%2, B:%3, A:%4")
        .arg(colorFromUint.red())
        .arg(colorFromUint.green())
        .arg(colorFromUint.blue())
        .arg(colorFromUint.alpha()));

    // 2. QColor 导出为 uint32_t (rgba() 保留 Alpha，rgb() 忽略 Alpha/设为 0xFF)
    QRgb exportedRgba = colorFromUint.rgba();
    QRgb exportedRgb = colorFromUint.rgb();

    printLine(QStringLiteral("导出 color.rgba() (ARGB)"), QStringLiteral("0x%1").arg(exportedRgba, 8, 16, QChar('0')).toUpper());
    printLine(QStringLiteral("导出 color.rgb() (不透明)"), QStringLiteral("0x%1").arg(exportedRgb, 8, 16, QChar('0')).toUpper());
}

void ColorDemo::testBitwiseChannelExtraction()
{
    printTitle(QStringLiteral("[模块 3] ARGB 与 RGBA 字节序差异、通道取色与位运算合成"));

    uint32_t argb = 0x80FF55AA; // A=0x80(128), R=0xFF(255), G=0x55(85), B=0xAA(170)

    // 1. 使用 Qt 官方宏通道取色
    int q_a = qAlpha(argb);
    int q_r = qRed(argb);
    int q_g = qGreen(argb);
    int q_b = qBlue(argb);
    printLine(QStringLiteral("Qt 宏取色 (qAlpha/qRed...)"), QStringLiteral("A:%1, R:%2, G:%3, B:%4").arg(q_a).arg(q_r).arg(q_g).arg(q_b));

    // 2. 手动位运算提取各通道
    uint8_t bit_a = (argb >> 24) & 0xFF;
    uint8_t bit_r = (argb >> 16) & 0xFF;
    uint8_t bit_g = (argb >> 8) & 0xFF;
    uint8_t bit_b = argb & 0xFF;
    printLine(QStringLiteral("位运算提取 (>> 24, 16, 8)"), QStringLiteral("A:%1, R:%2, G:%3, B:%4").arg(bit_a).arg(bit_r).arg(bit_g).arg(bit_b));

    // 3. ARGB (Qt/GDI 默认) 与 RGBA (OpenGL/Web/CSS 默认) 字节序互转
    uint32_t rgba = convertArgbToRgba(argb);
    uint32_t restoredArgb = convertRgbaToArgb(rgba);

    printLine(QStringLiteral("ARGB 格式 (0xAARRGGBB)"), QStringLiteral("0x%1").arg(argb, 8, 16, QChar('0')).toUpper());
    printLine(QStringLiteral("RGBA 格式 (0xRRGGBBAA)"), QStringLiteral("0x%1").arg(rgba, 8, 16, QChar('0')).toUpper());
    printLine(QStringLiteral("RGBA 还原回 ARGB 校验"), QStringLiteral("0x%1 (一致: %2)")
        .arg(restoredArgb, 8, 16, QChar('0')).toUpper()
        .arg(restoredArgb == argb ? QStringLiteral("True") : QStringLiteral("False")));

    // 4. 手动位运算重组合成颜色
    uint32_t composedArgb = qRgba(bit_r, bit_g, bit_b, bit_a);
    printLine(QStringLiteral("qRgba 重组合成结果"), QStringLiteral("0x%1").arg(composedArgb, 8, 16, QChar('0')).toUpper());
}

void ColorDemo::testColorSpaces()
{
    printTitle(QStringLiteral("[模块 4] HSV / HSL 色彩空间变换与调色"));

    QColor baseColor(231, 76, 60); // 暖红色

    // 1. 转为 HSV 色彩空间 (Hue 0-359, Saturation 0-255, Value 0-255)
    int h, s, v;
    baseColor.getHsv(&h, &s, &v);
    printLine(QStringLiteral("原始 RGB 暖红色"), baseColor.name());
    printLine(QStringLiteral("HSV 分量 (色相/饱和/明度)"), QStringLiteral("H:%1° (色相), S:%2, V:%3").arg(h).arg(s).arg(v));

    // 2. 互补色计算 (色相旋转 180 度)
    int complementHue = (h + 180) % 360;
    QColor complementColor = QColor::fromHsv(complementHue, s, v);
    printLine(QStringLiteral("180° 互补色 (HSV计算)"), QStringLiteral("%1 (青绿色，H=%2°)").arg(complementColor.name()).arg(complementHue));

    // 3. 类似色计算 (+30 度)
    int analogHue = (h + 30) % 360;
    QColor analogColor = QColor::fromHsv(analogHue, s, v);
    printLine(QStringLiteral("30° 类似色 (HSV计算)"), QStringLiteral("%1 (橙黄色，H=%2°)").arg(analogColor.name()).arg(analogHue));
}

void ColorDemo::testLuminanceAndContrast()
{
    printTitle(QStringLiteral("[模块 5] 明暗调节 (lighter / darker) 与自动文字反色"));

    QColor themeColor(QStringLiteral("#3498db")); // 经典蓝色

    // 1. 增亮与变暗
    QColor lightColor = themeColor.lighter(130); // 增亮 30%
    QColor darkColor = themeColor.darker(130);   // 变暗 30%

    printLine(QStringLiteral("主题基准色"), themeColor.name());
    printLine(QStringLiteral("lighter(130) 增亮色"), lightColor.name());
    printLine(QStringLiteral("darker(130)  变暗色"), darkColor.name());

    // 2. 感知亮度与智能文字前景色判定 (ITU-R BT.601 亮度公式: 0.299R + 0.587G + 0.114B)
    auto getBestTextColor = [](const QColor &bg) -> QString {
        double luminance = 0.299 * bg.red() + 0.587 * bg.green() + 0.114 * bg.blue();
        return (luminance < 128.0) ? QStringLiteral("#FFFFFF (白色文字)") : QStringLiteral("#000000 (黑色文字)");
    };

    QColor darkBg(QStringLiteral("#1e272e")); // 暗夜黑
    QColor lightBg(QStringLiteral("#f5f6fa")); // 明亮灰白

    printLine(QStringLiteral("暗色背景最佳文字色"), QStringLiteral("背景 %1 -> 推荐 %2").arg(darkBg.name(), getBestTextColor(darkBg)));
    printLine(QStringLiteral("亮色背景最佳文字色"), QStringLiteral("背景 %1 -> 推荐 %2").arg(lightBg.name(), getBestTextColor(lightBg)));
}
