#include <QCoreApplication>
#include <iostream>

#include "xml_stream_demo.h"

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
    app.setApplicationName("demo_xml");
    app.setApplicationVersion("1.0.0");

    std::cout << "############################################################" << std::endl;
    std::cout << "   Qt XML 流式高性能解析与生成实战 (demo_xml)" << std::endl;
    std::cout << "############################################################" << std::endl;

    // 运行 4 大游戏 XML 流式测试模块
    XmlStreamDemo::testStreamWriting();
    XmlStreamDemo::testStreamReading();
    XmlStreamDemo::testErrorHandling();
    XmlStreamDemo::testNpcDialogueTreeStreaming();

    std::cout << "\n============================================================" << std::endl;
    std::cout << "  [demo_xml] 全模块执行完毕。" << std::endl;
    std::cout << "============================================================" << std::endl;

    return 0;
}
