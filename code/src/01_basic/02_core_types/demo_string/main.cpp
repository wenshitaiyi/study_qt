#include <QCoreApplication>
#include <iostream>

#include "string_demo.h"

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
    app.setApplicationName("demo_string");
    app.setApplicationVersion("1.0.0");

    std::cout << "############################################################" << std::endl;
    std::cout << "   Qt 字符串与字节流全体系实战 (demo_string)" << std::endl;
    std::cout << "############################################################" << std::endl;

    // 运行 8 大核心测试模块
    StringDemo::testQStringBasics();
    StringDemo::testQStringRef();
    StringDemo::testQStringView();
    StringDemo::testQByteArrayRef();
    StringDemo::testQStringList();
    StringDemo::testQByteArray();
    StringDemo::testEncodingConversion();
    StringDemo::testRegex();

    std::cout << "\n============================================================" << std::endl;
    std::cout << "  [demo_string] 全模块执行完毕。" << std::endl;
    std::cout << "============================================================" << std::endl;

    return 0;
}
