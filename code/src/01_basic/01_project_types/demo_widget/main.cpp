#include <QApplication>
#include "main_widget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("demo_widget");

    MainWidget widget;
    widget.show();

    return app.exec();
}
