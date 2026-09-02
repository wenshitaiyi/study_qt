#include <QApplication>
#include <QTextStream>
#include <QDebug>

#ifdef Q_OS_WIN
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#endif

#include "main_window.h"

// 全局 UTF-8 终端文本流
static QTextStream *s_outStream = nullptr;

void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(type);
    Q_UNUSED(context);

    if (s_outStream) {
        *s_outStream << msg << QStringLiteral("\n");
        s_outStream->flush();
    }
}

int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
    // 1. 若为 GUI 程序但从命令行启动，主动依附到父进程控制台终端
    if (AttachConsole(ATTACH_PARENT_PROCESS)) {
        FILE *fp = nullptr;
        freopen_s(&fp, "CONOUT$", "w", stdout);
        freopen_s(&fp, "CONOUT$", "w", stderr);
    }

    // 2. 强制设置控制台代码页为 UTF-8 (65001)，彻底消除中文乱码
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    // 3. 配置 QTextStream 为 UTF-8 编码流并接管 Qt 日志输出
    s_outStream = new QTextStream(stdout);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    s_outStream->setCodec("UTF-8");
#endif
    qInstallMessageHandler(customMessageHandler);

    // 启用高分屏适配
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication app(argc, argv);
    app.setApplicationName("demo_signalslot");
    app.setApplicationVersion("1.0.0");

    MainWindow window;
    window.show();

    return app.exec();
}
