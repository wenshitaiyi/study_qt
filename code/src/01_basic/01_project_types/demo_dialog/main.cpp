#include <QApplication>
#include "main_dialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("demo_dialog");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("StudyQt");

    MainDialog dialog;
    dialog.show();

    return app.exec();
}
