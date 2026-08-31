#include "main_dialog.h"
#include "ui_main_dialog.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QColorDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <QDateTime>

MainDialog::MainDialog(QWidget *parent)
    : QDialog(parent)
    , m_ui(std::make_unique<Ui::MainDialog>())
{
    m_ui->setupUi(this);

    connect(m_ui->btnModal, &QPushButton::clicked, this, &MainDialog::onBtnModalClicked);
    connect(m_ui->btnModeless, &QPushButton::clicked, this, &MainDialog::onBtnModelessClicked);
    connect(m_ui->btnMessageBox, &QPushButton::clicked, this, &MainDialog::onBtnMessageBoxClicked);
    connect(m_ui->btnFileDialog, &QPushButton::clicked, this, &MainDialog::onBtnFileDialogClicked);
    connect(m_ui->btnColorDialog, &QPushButton::clicked, this, &MainDialog::onBtnColorDialogClicked);
    connect(m_ui->btnFontDialog, &QPushButton::clicked, this, &MainDialog::onBtnFontDialogClicked);
    connect(m_ui->btnInputDialog, &QPushButton::clicked, this, &MainDialog::onBtnInputDialogClicked);
    connect(m_ui->btnClearLog, &QPushButton::clicked, this, &MainDialog::onBtnClearLogClicked);

    appendLog(QStringLiteral("系统"), QStringLiteral("demo_dialog 启动就绪，请点击上方按钮测试各类型对话框。"));
}

MainDialog::~MainDialog() = default;

void MainDialog::appendLog(const QString &category, const QString &message)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    QString logLine = QStringLiteral("[%1] [%2] %3").arg(timestamp, category, message);
    m_ui->logTextEdit->appendPlainText(logLine);
}

void MainDialog::onBtnClearLogClicked()
{
    m_ui->logTextEdit->clear();
}

void MainDialog::onBtnModalClicked()
{
    appendLog(QStringLiteral("模态"), QStringLiteral("启动模态对话框 (exec 阻塞当前主窗口)..."));

    CustomModalDialog dlg(this);
    dlg.setWindowTitle(QStringLiteral("自定义模态对话框 - 阻塞模式"));
    dlg.setConfig(m_currentConfig);

    int result = dlg.exec();

    if (result == QDialog::Accepted) {
        m_currentConfig = dlg.getConfig();
        appendLog(QStringLiteral("模态"), QStringLiteral("用户点击【确定】(Accepted)！更新配置 -> 用户名: %1, 端口: %2, 模式: %3, 自动重连: %4")
            .arg(m_currentConfig.username)
            .arg(m_currentConfig.port)
            .arg(m_currentConfig.mode)
            .arg(m_currentConfig.autoReconnect ? QStringLiteral("是") : QStringLiteral("否")));
    } else {
        appendLog(QStringLiteral("模态"), QStringLiteral("用户点击【取消】或关闭 (Rejected)，本次输入已丢弃。"));
    }
}

void MainDialog::onBtnModelessClicked()
{
    appendLog(QStringLiteral("非模态"), QStringLiteral("启动非模态对话框 (show 异步独立运行，主窗口仍可响应点击)..."));

    if (!m_modelessDialog) {
        m_modelessDialog = new CustomModalDialog(this);
        m_modelessDialog->setWindowTitle(QStringLiteral("自定义非模态对话框 - 异步模式"));
        m_modelessDialog->setAttribute(Qt::WA_DeleteOnClose);

        connect(m_modelessDialog, &CustomModalDialog::configApplied, this, &MainDialog::onModelessConfigApplied);
        connect(m_modelessDialog, &QObject::destroyed, this, [this]() {
            m_modelessDialog = nullptr;
            appendLog(QStringLiteral("非模态"), QStringLiteral("非模态对话框已关闭并释放内存 (WA_DeleteOnClose)。"));
        });
    }

    m_modelessDialog->setConfig(m_currentConfig);
    m_modelessDialog->show();
    m_modelessDialog->raise();
    m_modelessDialog->activateWindow();
}

void MainDialog::onModelessConfigApplied(const DialogConfig &config)
{
    m_currentConfig = config;
    appendLog(QStringLiteral("非模态"), QStringLiteral("收到非模态对话框信号 (Signal)! 更新配置 -> 用户名: %1, 端口: %2, 模式: %3")
        .arg(m_currentConfig.username)
        .arg(m_currentConfig.port)
        .arg(m_currentConfig.mode));
}

void MainDialog::onBtnMessageBoxClicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        QStringLiteral("操作确认 (QMessageBox)"),
        QStringLiteral("您确定要执行系统自检与数据库重置操作吗？"),
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
        QMessageBox::No
    );

    switch (reply) {
    case QMessageBox::Yes:
        appendLog(QStringLiteral("MessageBox"), QStringLiteral("用户选择：【Yes (确定)】"));
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("自检流程已进入后台排队执行。"));
        break;
    case QMessageBox::No:
        appendLog(QStringLiteral("MessageBox"), QStringLiteral("用户选择：【No (否)】"));
        break;
    case QMessageBox::Cancel:
        appendLog(QStringLiteral("MessageBox"), QStringLiteral("用户选择：【Cancel (取消)】"));
        break;
    default:
        break;
    }
}

void MainDialog::onBtnFileDialogClicked()
{
    QString selectedFile = QFileDialog::getOpenFileName(
        this,
        QStringLiteral("选择目标文件 (QFileDialog)"),
        QString(),
        QStringLiteral("代码文件 (*.cpp *.h *.ui *.json);;文本文件 (*.txt *.md);;所有文件 (*.*)")
    );

    if (!selectedFile.isEmpty()) {
        appendLog(QStringLiteral("FileDialog"), QStringLiteral("已选择文件路径: %1").arg(selectedFile));
    } else {
        appendLog(QStringLiteral("FileDialog"), QStringLiteral("用户取消了文件选择。"));
    }
}

void MainDialog::onBtnColorDialogClicked()
{
    QColor chosenColor = QColorDialog::getColor(Qt::darkCyan, this, QStringLiteral("请选择高亮强调色 (QColorDialog)"));

    if (chosenColor.isValid()) {
        appendLog(QStringLiteral("ColorDialog"), QStringLiteral("选定颜色: Hex = %1, RGB = (%2, %3, %4)")
            .arg(chosenColor.name())
            .arg(chosenColor.red())
            .arg(chosenColor.green())
            .arg(chosenColor.blue()));
    } else {
        appendLog(QStringLiteral("ColorDialog"), QStringLiteral("用户取消了颜色选择。"));
    }
}

void MainDialog::onBtnFontDialogClicked()
{
    bool ok = false;
    QFont currentFont = m_ui->logTextEdit->font();
    QFont chosenFont = QFontDialog::getFont(&ok, currentFont, this, QStringLiteral("配置日志展示字体 (QFontDialog)"));

    if (ok) {
        m_ui->logTextEdit->setFont(chosenFont);
        appendLog(QStringLiteral("FontDialog"), QStringLiteral("成功应用新字体 -> 家族: %1, 字号: %2 pt, 加粗: %3")
            .arg(chosenFont.family())
            .arg(chosenFont.pointSize())
            .arg(chosenFont.bold() ? QStringLiteral("是") : QStringLiteral("否")));
    } else {
        appendLog(QStringLiteral("FontDialog"), QStringLiteral("用户取消了字体选择。"));
    }
}

void MainDialog::onBtnInputDialogClicked()
{
    QStringList items;
    items << QStringLiteral("TCP 长连接通信")
          << QStringLiteral("HTTP REST API 协议")
          << QStringLiteral("WebSocket 实时推送")
          << QStringLiteral("UDP 局域网组播");

    bool ok = false;
    QString selectedItem = QInputDialog::getItem(
        this,
        QStringLiteral("协议模式选择 (QInputDialog)"),
        QStringLiteral("请选择本次任务使用的网络协议类型："),
        items,
        0,
        false,
        &ok
    );

    if (ok && !selectedItem.isEmpty()) {
        appendLog(QStringLiteral("InputDialog"), QStringLiteral("用户录入结果: 【%1】").arg(selectedItem));
    } else {
        appendLog(QStringLiteral("InputDialog"), QStringLiteral("用户取消了录入。"));
    }
}
