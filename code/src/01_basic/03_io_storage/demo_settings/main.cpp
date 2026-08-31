#include <QApplication>

#include "settings_dialog.h"

int main(int argc, char *argv[])
{
    // 启用高分屏支持
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication app(argc, argv);
    app.setApplicationName("demo_settings");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("StudyQt");

    SettingsDialog dialog;
    dialog.show();

    return app.exec();
}
