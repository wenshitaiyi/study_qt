#include <QCoreApplication>
#include <iostream>

#include "text_stream_demo.h"

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
    app.setApplicationName("demo_text_stream");
    app.setApplicationVersion("1.0.0");

    std::cout << "############################################################" << std::endl;
    std::cout << "   Qt 文本流读写与格式化实战 (demo_text_stream)" << std::endl;
    std::cout << "############################################################" << std::endl;

    // 运行 5 大文本流核心模块
    TextStreamDemo::testFormatManipulators();
    TextStreamDemo::testEncodingAndBOM();
    TextStreamDemo::testLineEndings();
    TextStreamDemo::testCsvReportProcessing();
    TextStreamDemo::testMemoryStringStream();

    std::cout << "\n============================================================" << std::endl;
    std::cout << "  [demo_text_stream] 全模块执行完毕。" << std::endl;
    std::cout << "============================================================" << std::endl;

    return 0;
}
