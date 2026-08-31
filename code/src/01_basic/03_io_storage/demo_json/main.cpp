#include <QCoreApplication>
#include <iostream>

#include "json_demo.h"

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
    app.setApplicationName("demo_json");
    app.setApplicationVersion("1.0.0");

    std::cout << "############################################################" << std::endl;
    std::cout << "   Qt JSON 树状解析、深度合并与路径检索实战 (demo_json)" << std::endl;
    std::cout << "############################################################" << std::endl;

    // 运行 5 大 JSON 测试模块
    JsonDemo::testBasicConstructionAndFormatting();
    JsonDemo::testParsingAndErrorHandling();
    JsonDemo::testVariantConversion();
    JsonDemo::testJsonDeepMerge();
    JsonDemo::testPathBasedQueryAndUpdate();

    std::cout << "\n============================================================" << std::endl;
    std::cout << "  [demo_json] 全模块执行完毕。" << std::endl;
    std::cout << "============================================================" << std::endl;

    return 0;
}
