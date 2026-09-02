#pragma once

#include <QMainWindow>
#include <QMetaObject>
#include <QVariant>
#include <memory>

#include "custom_types.h"

namespace Ui {
class MainWindow;
}

class WorkerThread;

/**
 * @brief Qt 信号与槽机制深度探索与全景实战主窗口
 * 
 * 核心演示要点：
 * 1. 语法范式与二义性消除：Qt4 字符串宏 vs Qt5 函数指针 vs QOverload<T>::of / qOverload<T> / static_cast 3 种重载方案
 * 2. Lambda 表达式安全：无 Context 的野指针风险 vs 传入 Context (this) 的生命周期与线程安全自动托管
 * 3. 信号与信号联动 (Signal-to-Signal)：三级信号级联冒泡转发链
 * 4. 绑定顺序与执行顺序：严格验证 Qt 同线程下的 FIFO 先入先出执行顺序
 * 5. 重复绑定与防重：演示重复 connect 导致 N 次触发的经典 Bug，并使用 Qt::UniqueConnection 完美解决
 * 6. 解绑大全：全面演示 5 种 QObject::disconnect 解绑形式与句柄精准断开
 * 7. 自定义类型与 qRegisterMetaType：跨线程 QueuedConnection 传递自定义结构体、void* 通用指针与 QVariant 统一包装
 * 8. 5 种连接模式 (Qt::ConnectionType)：AutoConnection, DirectConnection, QueuedConnection, BlockingQueuedConnection (死锁防范), UniqueConnection
 * 9. 跨线程交互与 qApp 调度：在任意底层 std::thread 中使用 QMetaObject::invokeMethod(qApp, ...) 极速安全切回主 UI 线程
 * 10. 高级特性：connectSlotsByName 自动命名绑定、blockSignals 信号静音阻断、sender() 发送者探测与 receivers 计数
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

signals:
    // 基础演示信号
    void customSignalA(const QString &msg);
    void customSignalB(const QString &msg);
    void signalOverloaded(int value);
    void signalOverloaded(const QString &value);
    void testBlockSignal(const QString &msg);

    // 1. 信号三级级联联动
    void chainSignal1(const QString &data);
    void chainSignal2(const QString &data);
    void chainSignal3(const QString &data);

    // 2. 顺序测试信号
    void orderTestSignal(const QString &info);

    // 3. 重复绑定测试信号
    void dupTestSignal();

    // 4. 解绑演示测试信号
    void disconnectTestSig1(const QString &msg);
    void disconnectTestSig2(const QString &msg);

    // 5. 自定义类型与通用 void* / QVariant 信号
    void customStructSignal(const PlayerPacket &packet);
    void voidPointerSignal(void *contextPtr);
    void voidPointerAsyncSignal(void *heapPtr);
    void variantSignal(const QVariant &data);

private slots:
    // Tab 1: 语法范式与 Lambda
    void onQt4SyntaxClicked();
    void onQt5SyntaxClicked();
    void onOverloadQOverloadClicked();
    void onOverloadqOverloadClicked();
    void onOverloadStaticCastClicked();
    void onLambdaNoContextClicked();
    void onLambdaWithContextClicked();
    void onConnectHandleClicked();
    void onTriggerHandleSignalClicked();
    void onDisconnectHandleClicked();

    // Tab 2: 联动、顺序、重复绑定与解绑大全
    void onTriggerSignalChainClicked();
    void onChainFinalSlot(const QString &data);

    void onConnectOrder123Clicked();
    void onConnectOrder321Clicked();
    void onTriggerOrderSignalClicked();
    void slotOrderA(const QString &info);
    void slotOrderB(const QString &info);
    void slotOrderC(const QString &info);

    void onRepeatConnect3TimesClicked();
    void onUniqueConnect3TimesClicked();
    void onTriggerDupTestSignalClicked();
    void slotDupReceiver();

    void onDisconnectAllSenderClicked();
    void onDisconnectSpecificSignalClicked();
    void onDisconnectSpecificReceiverClicked();
    void onDisconnectExactPairClicked();
    void slotDisconnectTarget1(const QString &msg);
    void slotDisconnectTarget2(const QString &msg);

    // Tab 3: 自定义类型与 void* 通用传递
    void onRegisterMetaTypeClicked();
    void onEmitCustomStructDirectClicked();
    void onEmitCustomStructQueuedClicked();
    void onEmitVoidPointerDirectClicked();
    void onEmitVoidPointerAsyncSafeClicked();
    void onEmitQVariantWrappedClicked();

    void slotReceiveCustomStruct(const PlayerPacket &packet);
    void slotReceiveVoidPointerDirect(void *ptr);
    void slotReceiveVoidPointerAsyncSafe(void *ptr);
    void slotReceiveVariant(const QVariant &data);

    // Tab 4: 5 种连接类型与跨线程
    void onStartWorkerThreadClicked();
    void onTestBlockingQueueClicked();
    void onInvokeMethodQAppClicked();

    // Tab 5: 自动命名绑定与高级特性 (遵循 on_<objectName>_<signalName> 命名规范)
    void on_btnAutoBound_clicked();
    void onCheckSenderClicked();
    void onQueryReceiversClicked();

    // 内部通用槽
    void onCustomSignalBReceived(const QString &msg);
    void onTestBlockSignalReceived(const QString &msg);

private:
    void initStylesAndConnections();
    void appendLog(const QString &category, const QString &message);
    Qt::ConnectionType getSelectedConnectionType() const;

    std::unique_ptr<Ui::MainWindow> ui;
    WorkerThread *m_workerThread{nullptr};
    QMetaObject::Connection m_dynamicConnection;
    int m_handleSignalCount{0};
    int m_dupTriggerCount{0};
    bool m_isMetaTypeRegistered{false};
};
