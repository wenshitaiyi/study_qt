#include <QCoreApplication>
#include <iostream>

#include "file_demo.h"

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
    app.setApplicationName("demo_file");
    app.setApplicationVersion("1.0.0");

    std::cout << "############################################################" << std::endl;
    std::cout << "   Qt 文件读写与目录遍历实战 (demo_file)" << std::endl;
    std::cout << "############################################################" << std::endl;

    // 运行 5 大核心文件与目录测试模块
    FileDemo::testFileReadWrite();
    FileDemo::testFileLifecycle();
    FileDemo::testFileInfoInspection();
    FileDemo::testDirOperations();
    FileDemo::testRecursiveDirectoryScan();

    std::cout << "\n============================================================" << std::endl;
    std::cout << "  [demo_file] 全模块执行完毕。" << std::endl;
    std::cout << "============================================================" << std::endl;

    return 0;
}
