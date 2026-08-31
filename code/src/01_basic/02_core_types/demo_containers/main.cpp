#include <QCoreApplication>
#include <iostream>

#include "container_demo.h"

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
    app.setApplicationName("demo_containers");
    app.setApplicationVersion("1.0.0");

    std::cout << "############################################################" << std::endl;
    std::cout << "   Qt 容器与迭代器全体系实战 (demo_containers)" << std::endl;
    std::cout << "############################################################" << std::endl;

    // 依次运行 8 大核心测试模块
    ContainerDemo::testSequenceContainers();
    ContainerDemo::testAssociativeContainers();
    ContainerDemo::testSetOperations();
    ContainerDemo::testIterators();
    ContainerDemo::testImplicitSharing();
    ContainerDemo::testForLoopStyles();
    ContainerDemo::testQtContainerMacrosAndAlgorithms();
    ContainerDemo::testForwardDeclarations();

    std::cout << "\n============================================================" << std::endl;
    std::cout << "  [demo_containers] 全模块执行完毕。" << std::endl;
    std::cout << "============================================================" << std::endl;

    return 0;
}
