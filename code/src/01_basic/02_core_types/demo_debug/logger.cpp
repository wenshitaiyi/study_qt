#include "logger.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QThread>
#include <iostream>

QMutex Logger::s_mutex;
QString Logger::s_logDir;
QString Logger::s_currentLogFile;
bool Logger::s_outputToConsole = true;
bool Logger::s_installed = false;

void Logger::install(const QString &logDir, bool outputToConsole)
{
    QMutexLocker locker(&s_mutex);
    if (s_installed) {
        return;
    }

    s_outputToConsole = outputToConsole;

    // 确定日志目录路径（默认相对于当前程序运行目录）
    if (QDir::isRelativePath(logDir)) {
        s_logDir = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath(logDir);
    } else {
        s_logDir = logDir;
    }

    QDir dir(s_logDir);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    // 按日期创建日志文件名
    QString dateStr = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    s_currentLogFile = dir.absoluteFilePath(QStringLiteral("app_%1.log").arg(dateStr));

    qInstallMessageHandler(Logger::messageHandler);
    s_installed = true;
}

void Logger::uninstall()
{
    QMutexLocker locker(&s_mutex);
    if (!s_installed) {
        return;
    }

    qInstallMessageHandler(nullptr);
    s_installed = false;
}

QString Logger::getLogFilePath()
{
    QMutexLocker locker(&s_mutex);
    return s_currentLogFile;
}

void Logger::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // 线程安全互斥锁
    QMutexLocker locker(&s_mutex);

    QString levelStr;
    QString ansiColor;
    const QString ansiReset = QStringLiteral("\033[0m");

    switch (type) {
    case QtDebugMsg:
        levelStr = QStringLiteral("DEBUG");
        ansiColor = QStringLiteral("\033[36m"); // Cyan
        break;
    case QtInfoMsg:
        levelStr = QStringLiteral("INFO ");
        ansiColor = QStringLiteral("\033[32m"); // Green
        break;
    case QtWarningMsg:
        levelStr = QStringLiteral("WARN ");
        ansiColor = QStringLiteral("\033[33m"); // Yellow
        break;
    case QtCriticalMsg:
        levelStr = QStringLiteral("CRIT ");
        ansiColor = QStringLiteral("\033[31m"); // Red
        break;
    case QtFatalMsg:
        levelStr = QStringLiteral("FATAL");
        ansiColor = QStringLiteral("\033[35m"); // Magenta
        break;
    }

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    QString threadId = QStringLiteral("0x%1").arg(reinterpret_cast<quintptr>(QThread::currentThreadId()), 0, 16);

    // 提取文件名（去除绝对路径冗余）
    QString fileName = context.file ? QFileInfo(QString::fromUtf8(context.file)).fileName() : QStringLiteral("unknown");
    int line = context.line;

    // 格式化日志内容
    QString formattedLog = QStringLiteral("[%1] [%2] [Thread:%3] [%4:%5] %6")
        .arg(timestamp, levelStr, threadId, fileName)
        .arg(line)
        .arg(msg);

    // 1. 写入本地文件
    QFile file(s_currentLogFile);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        stream << formattedLog << "\n";
        file.flush();
        file.close();
    }

    // 2. 控制台输出
    if (s_outputToConsole) {
        std::cout << ansiColor.toStdString()
                  << formattedLog.toUtf8().constData()
                  << ansiReset.toStdString()
                  << std::endl;
    }

    if (type == QtFatalMsg) {
        abort();
    }
}
