#include <QCoreApplication>
#include <iostream>

#include "datetime_demo.h"

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
    app.setApplicationName("demo_datetime");
    app.setApplicationVersion("1.0.0");

    std::cout << "############################################################" << std::endl;
    std::cout << "   Qt 时间日期与性能基准全体系实战 (demo_datetime)" << std::endl;
    std::cout << "############################################################" << std::endl;

    // 运行 5 大核心测试模块
    DateTimeDemo::testFormattingAndParsing();
    DateTimeDemo::testCalculationsAndCalendar();
    DateTimeDemo::testTimestampConversions();
    DateTimeDemo::testTimeZones();
    DateTimeDemo::testElapsedTimerBenchmark();

    std::cout << "\n============================================================" << std::endl;
    std::cout << "  [demo_datetime] 全模块执行完毕。" << std::endl;
    std::cout << "============================================================" << std::endl;

    return 0;
}
