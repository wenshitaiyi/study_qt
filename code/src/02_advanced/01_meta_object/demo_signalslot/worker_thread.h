#pragma once

#include <QThread>
#include <QString>

/**
 * @brief 模拟后台计算工作线程 (用于测试跨线程信号槽与 5 种连接模式)
 */
class WorkerThread : public QThread
{
    Q_OBJECT

public:
    explicit WorkerThread(QObject *parent = nullptr);
    ~WorkerThread() override;

    void stopWork();

signals:
    void progressUpdated(int percent, quintptr threadId);
    void taskCompleted(const QString &result, quintptr threadId);
    int requestMainThreadCalculation(int a, int b); // 用于测试 BlockingQueuedConnection

public slots:
    void onExecuteTask(int taskId);

protected:
    void run() override;

private:
    bool m_stopRequested{false};
};
