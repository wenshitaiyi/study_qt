#include <QCoreApplication>
#include <iostream>

#include "color_demo.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif

int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    QCoreApplication app(argc, argv);
    app.setApplicationName("demo_color");
    app.setApplicationVersion("1.0.0");

    std::cout << "############################################################" << std::endl;
    std::cout << "   Qt 颜色空间与无符号整型转换实战 (demo_color)" << std::endl;
    std::cout << "############################################################" << std::endl;

    // 运行 5 大核心色彩测试模块
    ColorDemo::testBasicConstructionAndHex();
    ColorDemo::testUintConversion();
    ColorDemo::testBitwiseChannelExtraction();
    ColorDemo::testColorSpaces();
    ColorDemo::testLuminanceAndContrast();

    std::cout << "\n============================================================" << std::endl;
    std::cout << "  [demo_color] 全模块执行完毕。" << std::endl;
    std::cout << "============================================================" << std::endl;

    return 0;
}
