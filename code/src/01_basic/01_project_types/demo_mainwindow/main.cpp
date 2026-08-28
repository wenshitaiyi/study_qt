#include <QApplication>
#include "main_window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("demo_mainwindow");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("StudyQt");

    MainWindow window;
    window.show();

    return app.exec();
}
