#include <QCoreApplication>
#include <QDebug>
#include <QThread>
#include <QFile>
#include <QTextStream>
#include <iostream>
#include <vector>
#include <memory>

#include "logger.h"

// 模拟多线程工作者
class WorkerThread : public QThread
{
public:
    explicit WorkerThread(int id, QObject *parent = nullptr)
        : QThread(parent)
        , m_id(id)
    {
    }

protected:
    void run() override
    {
        qInfo() << QStringLiteral("子工作线程 #%1 启动执行任务...").arg(m_id);
        for (int i = 1; i <= 3; ++i) {
            qDebug() << QStringLiteral("子线程 #%1 正在处理第 %2 批数据项").arg(m_id).arg(i);
            QThread::msleep(30);
        }
        qInfo() << QStringLiteral("子工作线程 #%1 任务圆满完成!").arg(m_id);
    }

private:
    int m_id;
};

#ifdef Q_OS_WIN
#include <windows.h>
#endif

int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    QCoreApplication app(argc, argv);
    app.setApplicationName("demo_debug");
    app.setApplicationVersion("1.0.0");

    std::cout << "================================================================" << std::endl;
    std::cout << "  Qt 日志系统与重定向实战 (demo_debug) - 控制台程序" << std::endl;
    std::cout << "================================================================" << std::endl;

    // 1. 安装全局日志处理器
    Logger::install("logs", true);
    qInfo() << QStringLiteral("=== 全局日志重定向已成功安装 ===");
    qInfo() << QStringLiteral("日志输出文件位置: %1").arg(Logger::getLogFilePath());

    // 2. 测试不同等级的日志输出
    qDebug()    << QStringLiteral("[测试 Debug] 调试跟踪数据: 用户点击了按钮, 坐标 (120, 350)");
    qInfo()     << QStringLiteral("[测试 Info ] 系统运行正常: 已成功连接到本地网关服务器");
    qWarning()  << QStringLiteral("[测试 Warn ] 告警通知: 磁盘空间使用率已达到 82.5%");
    qCritical() << QStringLiteral("[测试 Crit ] 严重错误: 尝试读取缺失的配置文件 config.ini");

    // 3. 测试多线程并发日志落盘安全性
    qInfo() << QStringLiteral("=== 开始多线程并发日志写入测试 ===");
    std::vector<std::unique_ptr<WorkerThread>> threads;
    for (int i = 1; i <= 3; ++i) {
        auto thread = std::make_unique<WorkerThread>(i);
        thread->start();
        threads.push_back(std::move(thread));
    }

    // 等待所有子线程结束
    for (auto &t : threads) {
        t->wait();
    }
    qInfo() << QStringLiteral("=== 多线程并发日志测试完成 ===");

    // 4. 读取并打印落盘的日志文件内容摘要
    QString logFilePath = Logger::getLogFilePath();
    QFile logFile(logFilePath);
    if (logFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&logFile);
        stream.setCodec("UTF-8");
        std::cout << "\n---------------- [读取本地日志文件内容校验] ----------------" << std::endl;
        int lineIndex = 1;
        while (!stream.atEnd() && lineIndex <= 15) {
            std::cout << stream.readLine().toUtf8().constData() << std::endl;
            ++lineIndex;
        }
        std::cout << "------------------------------------------------------------\n" << std::endl;
        logFile.close();
    }

    // 5. 卸载日志处理器
    qInfo() << QStringLiteral("=== 正在卸载全局日志处理器 ===");
    Logger::uninstall();

    std::cout << "[demo_debug] 执行完毕，退出程序。" << std::endl;
    return 0;
}
