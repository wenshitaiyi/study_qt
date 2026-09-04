#include "main_window.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("demo_coordinate_system"));
    app.setApplicationDisplayName(QStringLiteral("2D 空间矩阵变换与视口裁剪交互平台"));

    MainWindow window;
    window.resize(1180, 800);
    window.show();

    return app.exec();
}
