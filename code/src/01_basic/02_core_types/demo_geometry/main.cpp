#include <QCoreApplication>
#include <iostream>

#include "geometry_demo.h"

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
    app.setApplicationName("demo_geometry");
    app.setApplicationVersion("1.0.0");

    std::cout << "############################################################" << std::endl;
    std::cout << "   Qt 几何与空间计算全体系实战 (demo_geometry)" << std::endl;
    std::cout << "############################################################" << std::endl;

    // 运行 5 大几何空间测试模块
    GeometryDemo::testPointsAndDistances();
    GeometryDemo::testSizesAndScaling();
    GeometryDemo::testRectanglesAndCollisions();
    GeometryDemo::testMarginsAndPadding();
    GeometryDemo::testPolygonAndSelection();

    std::cout << "\n============================================================" << std::endl;
    std::cout << "  [demo_geometry] 全模块执行完毕。" << std::endl;
    std::cout << "============================================================" << std::endl;

    return 0;
}
