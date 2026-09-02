#include "main_window.h"
#include "ui_main_window.h"
#include "worker_thread.h"

#include <QDateTime>
#include <QThread>
#include <QMessageBox>
#include <QDebug>
#include <thread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::MainWindow>())
    , m_workerThread(new WorkerThread(this))
{
    ui->setupUi(this);

    initStylesAndConnections();

    appendLog(QStringLiteral("系统初始化"),
              QStringLiteral("demo_signalslot 界面与控制台就绪！主 UI 线程 ID: %1")
              .arg(reinterpret_cast<quintptr>(QThread::currentThreadId())));
}

MainWindow::~MainWindow() = default;

void MainWindow::initStylesAndConnections()
{
    // 全局美化 QSS：解决 QGroupBox 标题高度截断与视觉现代感
    setStyleSheet(QStringLiteral(
        "QGroupBox {"
        "   font-size: 13px;"
        "   font-weight: bold;"
        "   color: #2c3e50;"
        "   border: 1px solid #dcdde1;"
        "   border-radius: 6px;"
        "   margin-top: 14px;"
        "   padding-top: 16px;"
        "   background-color: #fafafa;"
        "}"
        "QGroupBox::title {"
        "   subcontrol-origin: margin;"
        "   subcontrol-position: top left;"
        "   left: 10px;"
        "   padding: 0 6px;"
        "   background-color: transparent;"
        "}"
        "QPushButton {"
        "   padding: 5px 10px;"
        "   border: 1px solid #bdc3c7;"
        "   border-radius: 4px;"
        "   background-color: #ffffff;"
        "   font-size: 12px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #ecf0f1;"
        "   border-color: #3498db;"
        "}"
    ));

    // ========================================================================
    // 1. 语法范式、Lambda、重载
    // ========================================================================
    connect(ui->btnQt4Syntax, &QPushButton::clicked, this, &MainWindow::onQt4SyntaxClicked);
    connect(ui->btnQt5Syntax, &QPushButton::clicked, this, &MainWindow::onQt5SyntaxClicked);
    connect(ui->btnOverloadQOverload, &QPushButton::clicked, this, &MainWindow::onOverloadQOverloadClicked);
    connect(ui->btnOverloadqOverload, &QPushButton::clicked, this, &MainWindow::onOverloadqOverloadClicked);
    connect(ui->btnOverloadStaticCast, &QPushButton::clicked, this, &MainWindow::onOverloadStaticCastClicked);
    connect(ui->btnLambdaNoContext, &QPushButton::clicked, this, &MainWindow::onLambdaNoContextClicked);
    connect(ui->btnLambdaWithContext, &QPushButton::clicked, this, &MainWindow::onLambdaWithContextClicked);

    // ========================================================================
    // 2. 信号联动、执行顺序、重复绑定、解绑大全
    // ========================================================================
    // 信号三级级联联动链: chainSignal1 -> chainSignal2 -> chainSignal3 -> onChainFinalSlot
    connect(this, &MainWindow::chainSignal1, this, &MainWindow::chainSignal2);
    connect(this, &MainWindow::chainSignal2, this, &MainWindow::chainSignal3);
    connect(this, &MainWindow::chainSignal3, this, &MainWindow::onChainFinalSlot);
    connect(ui->btnTriggerSignalChain, &QPushButton::clicked, this, &MainWindow::onTriggerSignalChainClicked);

    // 绑定顺序与执行顺序
    connect(ui->btnConnectOrder123, &QPushButton::clicked, this, &MainWindow::onConnectOrder123Clicked);
    connect(ui->btnConnectOrder321, &QPushButton::clicked, this, &MainWindow::onConnectOrder321Clicked);
    connect(ui->btnTriggerOrderSignal, &QPushButton::clicked, this, &MainWindow::onTriggerOrderSignalClicked);

    // 重复绑定问题与 UniqueConnection
    connect(ui->btnRepeatConnect3Times, &QPushButton::clicked, this, &MainWindow::onRepeatConnect3TimesClicked);
    connect(ui->btnUniqueConnect3Times, &QPushButton::clicked, this, &MainWindow::onUniqueConnect3TimesClicked);
    connect(ui->btnTriggerDupTestSignal, &QPushButton::clicked, this, &MainWindow::onTriggerDupTestSignalClicked);

    // 多维度解绑
    connect(ui->btnDisconnectAllSender, &QPushButton::clicked, this, &MainWindow::onDisconnectAllSenderClicked);
    connect(ui->btnDisconnectSpecificSignal, &QPushButton::clicked, this, &MainWindow::onDisconnectSpecificSignalClicked);
    connect(ui->btnDisconnectSpecificReceiver, &QPushButton::clicked, this, &MainWindow::onDisconnectSpecificReceiverClicked);
    connect(ui->btnDisconnectExactPair, &QPushButton::clicked, this, &MainWindow::onDisconnectExactPairClicked);

    // ========================================================================
    // 3. 自定义类型与 void* 通用传递
    // ========================================================================
    connect(ui->btnRegisterMetaType, &QPushButton::clicked, this, &MainWindow::onRegisterMetaTypeClicked);
    connect(ui->btnEmitCustomStructQueued, &QPushButton::clicked, this, &MainWindow::onEmitCustomStructQueuedClicked);
    connect(ui->btnEmitVoidPointerDirect, &QPushButton::clicked, this, &MainWindow::onEmitVoidPointerDirectClicked);
    connect(ui->btnEmitVoidPointerAsyncSafe, &QPushButton::clicked, this, &MainWindow::onEmitVoidPointerAsyncSafeClicked);
    connect(ui->btnEmitQVariantWrapped, &QPushButton::clicked, this, &MainWindow::onEmitQVariantWrappedClicked);

    connect(this, &MainWindow::customStructSignal, this, &MainWindow::slotReceiveCustomStruct);
    connect(this, &MainWindow::voidPointerSignal, this, &MainWindow::slotReceiveVoidPointerDirect);
    connect(this, &MainWindow::voidPointerAsyncSignal, this, &MainWindow::slotReceiveVoidPointerAsyncSafe, Qt::QueuedConnection);
    connect(this, &MainWindow::variantSignal, this, &MainWindow::slotReceiveVariant);

    // ========================================================================
    // 4. 5 种连接类型与跨线程
    // ========================================================================
    connect(ui->btnStartWorkerThread, &QPushButton::clicked, this, &MainWindow::onStartWorkerThreadClicked);
    connect(ui->btnTestBlockingQueue, &QPushButton::clicked, this, &MainWindow::onTestBlockingQueueClicked);
    connect(ui->btnInvokeMethodQApp, &QPushButton::clicked, this, &MainWindow::onInvokeMethodQAppClicked);

    // ========================================================================
    // 5. 自动命名绑定与高级特性
    // ========================================================================
    connect(ui->btnTriggerBlocked, &QPushButton::clicked, this, [this]() {
        emit testBlockSignal(QStringLiteral("这是一条用于测试 blockSignals 状态的测试信号！"));
    });
    connect(this, &MainWindow::testBlockSignal, this, &MainWindow::onTestBlockSignalReceived);

    connect(ui->chkBlockSignals, &QCheckBox::toggled, this, [this](bool checked) {
        bool prev = blockSignals(checked);
        appendLog(QStringLiteral("blockSignals"),
                  QStringLiteral("设置 blockSignals(%1)，前一状态为: %2")
                  .arg(checked ? QStringLiteral("true (阻断信号)") : QStringLiteral("false (恢复正常)"))
                  .arg(prev ? QStringLiteral("true") : QStringLiteral("false")));
    });

    connect(ui->btnCheckSender, &QPushButton::clicked, this, &MainWindow::onCheckSenderClicked);
    connect(ui->btnQueryReceivers, &QPushButton::clicked, this, &MainWindow::onQueryReceiversClicked);

    // 重载信号响应 Lambda
    connect(this, QOverload<int>::of(&MainWindow::signalOverloaded), this, [this](int val) {
        appendLog(QStringLiteral("QOverload<int>"), QStringLiteral("捕获到整型重载信号 -> int 值为: %1").arg(val));
    });
    connect(this, QOverload<const QString &>::of(&MainWindow::signalOverloaded), this, [this](const QString &val) {
        appendLog(QStringLiteral("QOverload<QString>"), QStringLiteral("捕获到字符串重载信号 -> QString 值为: %1").arg(val));
    });
}

void MainWindow::appendLog(const QString &category, const QString &message)
{
    QString timeStr = QDateTime::currentDateTime().toString(QStringLiteral("HH:mm:ss.zzz"));
    quintptr currentThreadId = reinterpret_cast<quintptr>(QThread::currentThreadId());
    
    // 直接输出到标准控制台终端，消除 UI 控件嵌套负担
    qInfo().noquote() << QStringLiteral("[%1] [线程:%2] [%3] %4")
                         .arg(timeStr)
                         .arg(currentThreadId)
                         .arg(category.leftJustified(20, ' '))
                         .arg(message);
}

Qt::ConnectionType MainWindow::getSelectedConnectionType() const
{
    int index = ui->comboConnType->currentIndex();
    switch (index) {
    case 0: return Qt::AutoConnection;
    case 1: return Qt::DirectConnection;
    case 2: return Qt::QueuedConnection;
    case 3: return Qt::BlockingQueuedConnection;
    case 4: return Qt::UniqueConnection;
    default: return Qt::AutoConnection;
    }
}

// ============================================================================
// 1. 语法范式与 Lambda 表达式
// ============================================================================
void MainWindow::onQt4SyntaxClicked()
{
    appendLog(QStringLiteral("Qt4 语法"), QStringLiteral("使用 SIGNAL(...) / SLOT(...) 字符串宏建立连接。"));
    QMessageBox::information(this, QStringLiteral("Qt4 宏语法演示"),
                             QStringLiteral("Qt4 语法基于运行时字符串匹配：\nconnect(sender, SIGNAL(clicked()), receiver, SLOT(onClicked()));\n"
                                            "优点：语法直观、兼容老旧 Qt 代码。\n"
                                            "缺点：缺少编译期类型检查，拼写错误难以发现。"));
}

void MainWindow::onQt5SyntaxClicked()
{
    appendLog(QStringLiteral("Qt5 语法"), QStringLiteral("使用 &Sender::sig, &Receiver::slot 成员函数指针建立安全连接。"));
    QMessageBox::information(this, QStringLiteral("Qt5 函数指针语法演示"),
                             QStringLiteral("Qt5 语法基于编译期函数指针：\nconnect(sender, &QPushButton::clicked, this, &MainWindow::onSlot);\n"
                                            "优点：编译期强类型检查、支持重构重命名、支持隐式参数收窄转换。"));
}

void MainWindow::onOverloadQOverloadClicked()
{
    int val = ui->spinOverload->value();
    appendLog(QStringLiteral("QOverload::of"),
              QStringLiteral("1️⃣ 使用 QOverload<int>::of(&MainWindow::signalOverloaded) 发射 int 重载信号: %1").arg(val));
    emit signalOverloaded(val);
}

void MainWindow::onOverloadqOverloadClicked()
{
    int val = ui->spinOverload->value();
    appendLog(QStringLiteral("qOverload 宏/模板"),
              QStringLiteral("2️⃣ 使用 qOverload<int>(&MainWindow::signalOverloaded) 发射 int 重载信号: %1").arg(val));
    emit signalOverloaded(val);
}

void MainWindow::onOverloadStaticCastClicked()
{
    int val = ui->spinOverload->value();
    appendLog(QStringLiteral("static_cast 转换"),
              QStringLiteral("3️⃣ 使用 static_cast<void (MainWindow::*)(int)>(&MainWindow::signalOverloaded) 发射 int 重载信号: %1").arg(val));
    emit signalOverloaded(val);
}

void MainWindow::onLambdaNoContextClicked()
{
    appendLog(QStringLiteral("Lambda (无Context)"),
              QStringLiteral("⚠️ connect(sender, &sig, [=](){}) 缺少 context，若所属窗口销毁后信号触发易发生崩溃。"));
}

void MainWindow::onLambdaWithContextClicked()
{
    appendLog(QStringLiteral("Lambda (安全Context)"),
              QStringLiteral("🛡️ connect(sender, &sig, this, [=](){}) 传入 this 作为 context，生命周期与线程调度均由 Qt 安全托管！"));
}

void MainWindow::onConnectHandleClicked()
{
    if (m_dynamicConnection) {
        appendLog(QStringLiteral("Connection 句柄"), QStringLiteral("当前已存在激活的动态连接句柄，无需重复建立。"));
        return;
    }

    m_dynamicConnection = connect(ui->btnTriggerOrderSignal, &QPushButton::clicked, this, [this]() {
        ++m_handleSignalCount;
        appendLog(QStringLiteral("句柄响应"), QStringLiteral("🔔 捕获到动态句柄绑定的触发信号 (第 %1 次)").arg(m_handleSignalCount));
    });

    appendLog(QStringLiteral("Connection 句柄"), QStringLiteral("🔗 成功建立连接并保存 QMetaObject::Connection 句柄对象！"));
}

void MainWindow::onTriggerHandleSignalClicked()
{
    if (!m_dynamicConnection) {
        appendLog(QStringLiteral("句柄触发"), QStringLiteral("⚠️ 当前没有通过句柄建立的连接。"));
    }
}

void MainWindow::onDisconnectHandleClicked()
{
    if (m_dynamicConnection) {
        QObject::disconnect(m_dynamicConnection);
        appendLog(QStringLiteral("Connection 句柄"), QStringLiteral("✂️ 已通过 QObject::disconnect(handle) 精准切断该动态连接！"));
    } else {
        appendLog(QStringLiteral("Connection 句柄"), QStringLiteral("当前无有效句柄可供断开。"));
    }
}

// ============================================================================
// 2. 信号联动、执行顺序、重复绑定与解绑大全
// ============================================================================
void MainWindow::onTriggerSignalChainClicked()
{
    appendLog(QStringLiteral("信号级联转发"),
              QStringLiteral("🚀 发射一级信号: chainSignal1('【玩家通关第10层深渊副本】')..."));
    emit chainSignal1(QStringLiteral("【玩家通关第10层深渊副本】"));
}

void MainWindow::onChainFinalSlot(const QString &data)
{
    appendLog(QStringLiteral("级联终端槽"),
              QStringLiteral("🎉 终端槽函数 onChainFinalSlot 成功捕获跨越三级信号转发的数据: %1").arg(data));
}

void MainWindow::onConnectOrder123Clicked()
{
    disconnect(this, &MainWindow::orderTestSignal, this, nullptr);

    connect(this, &MainWindow::orderTestSignal, this, &MainWindow::slotOrderA);
    connect(this, &MainWindow::orderTestSignal, this, &MainWindow::slotOrderB);
    connect(this, &MainWindow::orderTestSignal, this, &MainWindow::slotOrderC);

    appendLog(QStringLiteral("FIFO 顺序绑定"),
              QStringLiteral("1️⃣ 已按照 [槽A -> 槽B -> 槽C] 顺序建立 connect 连接。"));
}

void MainWindow::onConnectOrder321Clicked()
{
    disconnect(this, &MainWindow::orderTestSignal, this, nullptr);

    connect(this, &MainWindow::orderTestSignal, this, &MainWindow::slotOrderC);
    connect(this, &MainWindow::orderTestSignal, this, &MainWindow::slotOrderB);
    connect(this, &MainWindow::orderTestSignal, this, &MainWindow::slotOrderA);

    appendLog(QStringLiteral("FIFO 顺序绑定"),
              QStringLiteral("2️⃣ 已按照 [槽C -> 槽B -> 槽A] 逆序建立 connect 连接。"));
}

void MainWindow::onTriggerOrderSignalClicked()
{
    appendLog(QStringLiteral("执行顺序检验"), QStringLiteral("🔔 发射 orderTestSignal 信号，观察槽函数执行先后日志："));
    emit orderTestSignal(QStringLiteral("顺序测试数据包"));
}

void MainWindow::slotOrderA(const QString &info)
{
    appendLog(QStringLiteral("  -> 槽A执行"), QStringLiteral("【1号 槽A (slotOrderA)】接收到数据: %1").arg(info));
}

void MainWindow::slotOrderB(const QString &info)
{
    appendLog(QStringLiteral("  -> 槽B执行"), QStringLiteral("【2号 槽B (slotOrderB)】接收到数据: %1").arg(info));
}

void MainWindow::slotOrderC(const QString &info)
{
    appendLog(QStringLiteral("  -> 槽C执行"), QStringLiteral("【3号 槽C (slotOrderC)】接收到数据: %1").arg(info));
}

void MainWindow::onRepeatConnect3TimesClicked()
{
    connect(this, &MainWindow::dupTestSignal, this, &MainWindow::slotDupReceiver);
    connect(this, &MainWindow::dupTestSignal, this, &MainWindow::slotDupReceiver);
    connect(this, &MainWindow::dupTestSignal, this, &MainWindow::slotDupReceiver);

    appendLog(QStringLiteral("重复绑定测试"),
              QStringLiteral("⚠️ 已使用默认模式对同一对信号槽连续调用了 3 次 connect()！"));
}

void MainWindow::onUniqueConnect3TimesClicked()
{
    disconnect(this, &MainWindow::dupTestSignal, this, &MainWindow::slotDupReceiver);

    bool ok1 = connect(this, &MainWindow::dupTestSignal, this, &MainWindow::slotDupReceiver, Qt::UniqueConnection);
    bool ok2 = connect(this, &MainWindow::dupTestSignal, this, &MainWindow::slotDupReceiver, Qt::UniqueConnection);
    bool ok3 = connect(this, &MainWindow::dupTestSignal, this, &MainWindow::slotDupReceiver, Qt::UniqueConnection);

    appendLog(QStringLiteral("UniqueConnection"),
              QStringLiteral("🛡️ 使用 Qt::UniqueConnection 尝试 3 次连接: 第1次=%1, 第2次=%2, 第3次=%3 (第2/3次被自动拒绝，杜绝重复！)")
              .arg(ok1 ? QStringLiteral("成功") : QStringLiteral("失败"))
              .arg(ok2 ? QStringLiteral("成功") : QStringLiteral("失败"))
              .arg(ok3 ? QStringLiteral("成功") : QStringLiteral("失败")));
}

void MainWindow::onTriggerDupTestSignalClicked()
{
    m_dupTriggerCount = 0;
    appendLog(QStringLiteral("重复触发测试"), QStringLiteral("🔔 发射 dupTestSignal 信号 (仅发射 1 次)..."));
    emit dupTestSignal();
}

void MainWindow::slotDupReceiver()
{
    ++m_dupTriggerCount;
    appendLog(QStringLiteral("  -> slotDupReceiver"),
              QStringLiteral("💥 槽函数 slotDupReceiver 被触发执行！(当前累计执行了第 %1 次)").arg(m_dupTriggerCount));
}

void MainWindow::onDisconnectAllSenderClicked()
{
    disconnect(this, &MainWindow::disconnectTestSig1, this, &MainWindow::slotDisconnectTarget1);
    disconnect(this, &MainWindow::disconnectTestSig2, this, &MainWindow::slotDisconnectTarget2);
    
    this->disconnect();
    initStylesAndConnections();

    appendLog(QStringLiteral("全量解绑 (形式1)"),
              QStringLiteral("1️⃣ 已执行 this->disconnect()，解绑了当前发送者对象发出的全部信号连接！"));
}

void MainWindow::onDisconnectSpecificSignalClicked()
{
    disconnect(this, &MainWindow::disconnectTestSig1, nullptr, nullptr);

    appendLog(QStringLiteral("解绑指定信号 (形式2)"),
              QStringLiteral("2️⃣ 已执行 disconnect(this, &MainWindow::disconnectTestSig1, nullptr, nullptr)，仅解绑了 disconnectTestSig1 信号！"));
}

void MainWindow::onDisconnectSpecificReceiverClicked()
{
    disconnect(this, nullptr, this, nullptr);
    initStylesAndConnections();

    appendLog(QStringLiteral("解绑指定接收者 (形式3)"),
              QStringLiteral("3️⃣ 已执行 disconnect(sender, nullptr, receiver, nullptr)，解绑了指定接收者对象的所有槽函数。"));
}

void MainWindow::onDisconnectExactPairClicked()
{
    bool ok = disconnect(this, &MainWindow::disconnectTestSig1, this, &MainWindow::slotDisconnectTarget1);

    appendLog(QStringLiteral("精准解绑单对 (形式4)"),
              QStringLiteral("4️⃣ 已执行 disconnect(this, &sig1, this, &slot1) -> 结果: %1 (其他信号与槽不受任何影响)")
              .arg(ok ? QStringLiteral("成功解绑") : QStringLiteral("未找到对应连接")));
}

void MainWindow::slotDisconnectTarget1(const QString &msg)
{
    appendLog(QStringLiteral("slotTarget1"), QStringLiteral("目标槽 1 接收到: %1").arg(msg));
}

void MainWindow::slotDisconnectTarget2(const QString &msg)
{
    appendLog(QStringLiteral("slotTarget2"), QStringLiteral("目标槽 2 接收到: %1").arg(msg));
}

// ============================================================================
// 3. 自定义类型与 void* 通用传递
// ============================================================================
void MainWindow::onRegisterMetaTypeClicked()
{
    qRegisterMetaType<PlayerPacket>("PlayerPacket");
    qRegisterMetaType<RawContextData*>("RawContextData*");
    m_isMetaTypeRegistered = true;

    appendLog(QStringLiteral("qRegisterMetaType"),
              QStringLiteral("✅ 已成功注册自定义类型 [PlayerPacket] 与 [RawContextData*] 到 Qt 运行时元类型系统！"));
    QMessageBox::information(this, QStringLiteral("注册成功"),
                             QStringLiteral("已成功执行 qRegisterMetaType<PlayerPacket>(\"PlayerPacket\");\n现在该结构体可在跨线程 QueuedConnection 中安全自由传递！"));
}

void MainWindow::onEmitCustomStructDirectClicked()
{
    PlayerPacket packet{1002, QStringLiteral("毁灭术士·古尔丹"), 90, 125000.0};
    appendLog(QStringLiteral("同线程Direct传递"),
              QStringLiteral("🚀 发射自定义结构体信号: %1").arg(packet.toString()));
    emit customStructSignal(packet);
}

void MainWindow::onEmitCustomStructQueuedClicked()
{
    std::thread([this]() {
        quintptr subId = reinterpret_cast<quintptr>(QThread::currentThreadId());
        QThread::msleep(200);

        PlayerPacket asyncPacket{1003, QStringLiteral("暗夜猎手·伊利丹"), 99, 350000.0};

        appendLog(QStringLiteral("跨线程Queued传递"),
                  QStringLiteral("子线程 (ID: %1) 准备跨线程发射 PlayerPacket 结构体...").arg(subId));

        emit customStructSignal(asyncPacket);
    }).detach();
}

void MainWindow::slotReceiveCustomStruct(const PlayerPacket &packet)
{
    quintptr currentId = reinterpret_cast<quintptr>(QThread::currentThreadId());
    appendLog(QStringLiteral("槽:接收自定义类型"),
              QStringLiteral("🎉 [线程: %1] 成功接收并解构 PlayerPacket: %2").arg(currentId).arg(packet.toString()));
}

void MainWindow::onEmitVoidPointerDirectClicked()
{
    RawContextData stackContext{501, QStringLiteral("同线程栈内存上下文数据包"), 12345678};
    appendLog(QStringLiteral("void* 同线程发射"),
              QStringLiteral("🚀 发射栈地址 void* 指针: 0x%1").arg(reinterpret_cast<quintptr>(&stackContext), 0, 16));
    emit voidPointerSignal(static_cast<void*>(&stackContext));
}

void MainWindow::slotReceiveVoidPointerDirect(void *ptr)
{
    auto *ctx = reinterpret_cast<RawContextData*>(ptr);
    appendLog(QStringLiteral("槽:接收 void* (Direct)"),
              QStringLiteral("✅ 通过 reinterpret_cast 还原指针 -> ID: %1, 消息: [%2]")
              .arg(ctx->contextId).arg(ctx->message));
}

void MainWindow::onEmitVoidPointerAsyncSafeClicked()
{
    std::thread([this]() {
        quintptr subId = reinterpret_cast<quintptr>(QThread::currentThreadId());
        QThread::msleep(200);

        auto *heapData = new RawContextData{777, QStringLiteral("跨线程堆内存上下文 (所有权转交给接收端)"), 98765432};

        appendLog(QStringLiteral("void* 跨线程发射"),
                  QStringLiteral("子线程 (ID: %1) 在堆上分配 RawContextData (地址: 0x%2)，发射 void* 指针...")
                  .arg(subId).arg(reinterpret_cast<quintptr>(heapData), 0, 16));

        emit voidPointerAsyncSignal(static_cast<void*>(heapData));
    }).detach();
}

void MainWindow::slotReceiveVoidPointerAsyncSafe(void *ptr)
{
    quintptr mainId = reinterpret_cast<quintptr>(QThread::currentThreadId());
    auto *heapData = reinterpret_cast<RawContextData*>(ptr);

    appendLog(QStringLiteral("槽:接收 void* (Queued)"),
              QStringLiteral("🎯 [主线程: %1] 收到堆指针 0x%2 -> ID: %3, 消息: [%4]")
              .arg(mainId).arg(reinterpret_cast<quintptr>(heapData), 0, 16)
              .arg(heapData->contextId).arg(heapData->message));

    // 关键防漏：主线程接收方完成消费后执行内存释放
    delete heapData;
    appendLog(QStringLiteral("void* 内存安全释放"),
              QStringLiteral("🧹 接收端已安全执行 delete heapData，完成堆内存闭环回收，无内存泄漏。"));
}

void MainWindow::onEmitQVariantWrappedClicked()
{
    PlayerPacket packet{9999, QStringLiteral("大领主·提里奥·弗丁"), 100, 999999.0};
    QVariant var = QVariant::fromValue(packet);

    appendLog(QStringLiteral("QVariant 包装发射"),
              QStringLiteral("🎁 使用 QVariant::fromValue 包装 PlayerPacket 并发射，TypeName: [%1]")
              .arg(QString::fromLatin1(var.typeName())));
    emit variantSignal(var);
}

void MainWindow::slotReceiveVariant(const QVariant &data)
{
    if (data.canConvert<PlayerPacket>()) {
        PlayerPacket p = data.value<PlayerPacket>();
        appendLog(QStringLiteral("槽:QVariant 解包成功"),
                  QStringLiteral("🎉 成功从 QVariant 解包得到自定义结构体: %1").arg(p.toString()));
    } else {
        appendLog(QStringLiteral("槽:QVariant 解包失败"), QStringLiteral("❌ QVariant 无法转换为 PlayerPacket 类型。"));
    }
}

// ============================================================================
// 4. 5 种连接类型与跨线程交互
// ============================================================================
void MainWindow::onStartWorkerThreadClicked()
{
    Qt::ConnectionType connType = getSelectedConnectionType();
    QString connName = ui->comboConnType->currentText();

    appendLog(QStringLiteral("启动子线程"), QStringLiteral("🚀 启动后台 WorkerThread 工作线程，配置连接模式为: [%1]").arg(connName));

    m_workerThread->disconnect();

    connect(m_workerThread, &WorkerThread::progressUpdated, this, [this, connName](int percent, quintptr workerThreadId) {
        quintptr currentThreadId = reinterpret_cast<quintptr>(QThread::currentThreadId());
        ui->pbarWorker->setValue(percent);
        appendLog(QStringLiteral("跨线程进度"),
                  QStringLiteral("模式: %1 | 发射线程: %2 -> 接收执行线程: %3 | 进度: %4%")
                  .arg(connName).arg(workerThreadId).arg(currentThreadId).arg(percent));
    }, connType);

    connect(m_workerThread, &WorkerThread::taskCompleted, this, [this](const QString &result, quintptr workerThreadId) {
        quintptr currentThreadId = reinterpret_cast<quintptr>(QThread::currentThreadId());
        appendLog(QStringLiteral("任务完成"),
                  QStringLiteral("✅ 子线程任务结束: [%1] | 发射源线程: %2 -> 槽执行线程: %3")
                  .arg(result).arg(workerThreadId).arg(currentThreadId));
    }, connType);

    ui->pbarWorker->setValue(0);
    m_workerThread->start();
}

void MainWindow::onTestBlockingQueueClicked()
{
    appendLog(QStringLiteral("BlockingQueued"), QStringLiteral("⏳ 在后台 std::thread 中以 BlockingQueuedConnection 模式同步调用主线程槽函数..."));

    std::thread([this]() {
        quintptr workerId = reinterpret_cast<quintptr>(QThread::currentThreadId());
        appendLog(QStringLiteral("BlockingQueued"), QStringLiteral("子线程 (ID: %1) 开始执行耗时前置准备...").arg(workerId));
        QThread::msleep(300);

        int calculatedValue = 0;
        QMetaObject::invokeMethod(this, [this, &calculatedValue]() {
            quintptr mainId = reinterpret_cast<quintptr>(QThread::currentThreadId());
            appendLog(QStringLiteral("BlockingQueued[主]"), QStringLiteral("主 UI 线程 (ID: %1) 正在处理子线程的阻塞同步计算请求...").arg(mainId));
            calculatedValue = 9999;
        }, Qt::BlockingQueuedConnection);

        appendLog(QStringLiteral("BlockingQueued[子]"), QStringLiteral("子线程 (ID: %1) 收到主线程返回结果: %2，已从阻塞中唤醒！").arg(workerId).arg(calculatedValue));
    }).detach();
}

void MainWindow::onInvokeMethodQAppClicked()
{
    std::thread([this]() {
        quintptr subThreadId = reinterpret_cast<quintptr>(QThread::currentThreadId());
        QThread::msleep(300);

        QMetaObject::invokeMethod(qApp, [this, subThreadId]() {
            quintptr mainThreadId = reinterpret_cast<quintptr>(QThread::currentThreadId());
            appendLog(QStringLiteral("invokeMethod(qApp)"),
                      QStringLiteral("🎯 成功从独立底层 std::thread (ID: %1) 切回主 UI 线程 (ID: %2) 并安全更新控件！")
                      .arg(subThreadId).arg(mainThreadId));
            QMessageBox::information(this, QStringLiteral("跨线程调度成功"),
                                     QStringLiteral("已从独立 std::thread 安全切回主 UI 线程更新界面！"));
        }, Qt::QueuedConnection);
    }).detach();
}

// ============================================================================
// 5. 自动命名绑定与高级特性
// ============================================================================
void MainWindow::on_btnAutoBound_clicked()
{
    appendLog(QStringLiteral("connectSlotsByName"),
              QStringLiteral("✨ 触发 on_btnAutoBound_clicked 槽函数！此函数通过 Qt 命名规范完全自动绑定，未书写任何 connect 代码。"));
}

void MainWindow::onCheckSenderClicked()
{
    QObject *sigSender = sender();
    QString senderName = sigSender ? sigSender->objectName() : QStringLiteral("未知对象");
    appendLog(QStringLiteral("sender() 探测"),
              QStringLiteral("🕵️ 通过 sender() 探测到当前触发源控件 objectName 为: [%1]，类名: [%2]")
              .arg(senderName).arg(sigSender ? QString::fromLatin1(sigSender->metaObject()->className()) : QStringLiteral("nullptr")));
}

void MainWindow::onQueryReceiversClicked()
{
    int count = receivers(SIGNAL(customSignalA(QString)));
    appendLog(QStringLiteral("receivers 统计"),
              QStringLiteral("📊 信号 customSignalA(QString) 当前已绑定的槽函数总数量为: %1").arg(count));
}

void MainWindow::onTestBlockSignalReceived(const QString &msg)
{
    appendLog(QStringLiteral("testBlockSignal"), QStringLiteral("🔔 成功接收到测试信号: %1").arg(msg));
}

void MainWindow::onCustomSignalBReceived(const QString &msg)
{
    appendLog(QStringLiteral("信号转发链路"), QStringLiteral("✅ 槽函数 onCustomSignalBReceived 成功接收到转发链传递的数据: [%1]").arg(msg));
}
