#pragma once

#include <QString>
#include <QMessageLogContext>
#include <QMutex>

class Logger
{
public:
    // 安装全局日志重定向处理器
    static void install(const QString &logDir = QStringLiteral("logs"), bool outputToConsole = true);

    // 卸载并恢复默认日志处理器
    static void uninstall();

    // 获取当前日志输出文件绝对路径
    static QString getLogFilePath();

private:
    // 自定义消息分发回调
    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private:
    static QMutex s_mutex;
    static QString s_logDir;
    static QString s_currentLogFile;
    static bool s_outputToConsole;
    static bool s_installed;
};
