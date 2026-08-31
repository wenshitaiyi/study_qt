#include <QCoreApplication>
#include <iostream>

#include "url_demo.h"

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
    app.setApplicationName("demo_url");
    app.setApplicationVersion("1.0.0");

    std::cout << "############################################################" << std::endl;
    std::cout << "   Qt URL 解析与网络参数构建实战 (demo_url)" << std::endl;
    std::cout << "############################################################" << std::endl;

    // 运行 5 大 URL 处理测试模块
    UrlDemo::testUrlComponents();
    UrlDemo::testUrlQuery();
    UrlDemo::testLocalFileAndRelativePath();
    UrlDemo::testPercentEncoding();
    UrlDemo::testUrlBuilderPattern();

    std::cout << "\n============================================================" << std::endl;
    std::cout << "  [demo_url] 全模块执行完毕。" << std::endl;
    std::cout << "============================================================" << std::endl;

    return 0;
}
