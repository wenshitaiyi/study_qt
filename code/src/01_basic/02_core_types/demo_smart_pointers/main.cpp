#include <QCoreApplication>
#include <iostream>

#include "smart_pointers_demo.h"

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
    app.setApplicationName("demo_smart_pointers");
    app.setApplicationVersion("1.0.0");

    std::cout << "############################################################" << std::endl;
    std::cout << "   Qt 与 C++ STL 智能指针全体系实战 (demo_smart_pointers)" << std::endl;
    std::cout << "############################################################" << std::endl;

    // 运行 5 大智能指针核心模块
    SmartPointersDemo::testQPointerForQObject();
    SmartPointersDemo::testScopedVsUniquePointer();
    SmartPointersDemo::testSharedAndWeakPointersComparison();
    SmartPointersDemo::testCustomImplicitSharingCOW();
    SmartPointersDemo::testSmartPointerSelectionGuide();

    std::cout << "\n============================================================" << std::endl;
    std::cout << "  [demo_smart_pointers] 全模块执行完毕。" << std::endl;
    std::cout << "============================================================" << std::endl;

    return 0;
}
