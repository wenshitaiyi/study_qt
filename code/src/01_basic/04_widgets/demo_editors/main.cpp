#include <QApplication>

#include "main_window.h"

int main(int argc, char *argv[])
{
    // 启用高分屏适配
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication app(argc, argv);
    app.setApplicationName("demo_editors");
    app.setApplicationVersion("1.0.0");

    MainWindow window;
    window.show();

    return app.exec();
}
