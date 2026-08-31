#include <QCoreApplication>
#include <iostream>

#include "data_stream_demo.h"

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
    app.setApplicationName("demo_datastream");
    app.setApplicationVersion("1.0.0");

    std::cout << "############################################################" << std::endl;
    std::cout << "   Qt 二进制数据流持久化与版本兼容实战 (demo_datastream)" << std::endl;
    std::cout << "############################################################" << std::endl;

    // 运行 5 大核心二进制序列化测试模块
    DataStreamDemo::testBasicTypesAndEndianness();
    DataStreamDemo::testQtContainersSerialization();
    DataStreamDemo::testCustomProjectSerialization();
    DataStreamDemo::testVersionControlAndCompatibility();
    DataStreamDemo::testMemoryPacketProtocol();

    std::cout << "\n============================================================" << std::endl;
    std::cout << "  [demo_datastream] 全模块执行完毕。" << std::endl;
    std::cout << "============================================================" << std::endl;

    return 0;
}
