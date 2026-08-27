#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QDateTime>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName("demo_console");
    app.setApplicationVersion("1.0.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Qt Console Application Demo");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption nameOption(QStringList() << "n" << "name", "User name to greet.", "name", "Qt Developer");
    parser.addOption(nameOption);

    parser.process(app);

    QString name = parser.value(nameOption);
    qDebug() << "========================================";
    qDebug() << "[demo_console] Qt Core Application Started!";
    qDebug() << "Current Time:" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    qDebug() << "Hello," << name << "!";
    qDebug() << "========================================";

    // Direct return or return app.exec() for long-running event loops
    return 0;
}
