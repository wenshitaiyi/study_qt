#include "worker_thread.h"
#include <QCoreApplication>
#include <QMetaObject>

WorkerThread::WorkerThread(QObject *parent)
    : QThread(parent)
{
}

WorkerThread::~WorkerThread()
{
    stopWork();
    wait();
}

void WorkerThread::stopWork()
{
    m_stopRequested = true;
}

void WorkerThread::run()
{
    m_stopRequested = false;
    quintptr currentThreadId = reinterpret_cast<quintptr>(QThread::currentThreadId());

    for (int i = 1; i <= 5; ++i) {
        if (m_stopRequested) break;
        QThread::msleep(300);

        int percent = i * 20;
        emit progressUpdated(percent, currentThreadId);
    }

    if (!m_stopRequested) {
        emit taskCompleted(QStringLiteral("后台高负载数据运算任务顺利完成"), currentThreadId);
    }
}

void WorkerThread::onExecuteTask(int taskId)
{
    quintptr currentThreadId = reinterpret_cast<quintptr>(QThread::currentThreadId());
    Q_UNUSED(taskId);
    emit taskCompleted(QStringLiteral("通过槽函数触发的任务执行完毕"), currentThreadId);
}
