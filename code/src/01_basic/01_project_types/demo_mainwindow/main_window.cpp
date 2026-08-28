#include "main_window.h"
#include "ui_main_window.h"

#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QListWidgetItem>
#include <QTextBlock>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(std::make_unique<Ui::MainWindow>())
{
    m_ui->setupUi(this);

    setupUiCustomizations();

    // Connect Actions
    connect(m_ui->actionNew, &QAction::triggered, this, &MainWindow::onActionNewTriggered);
    connect(m_ui->actionOpen, &QAction::triggered, this, &MainWindow::onActionOpenTriggered);
    connect(m_ui->actionSave, &QAction::triggered, this, &MainWindow::onActionSaveTriggered);
    connect(m_ui->actionExit, &QAction::triggered, this, &MainWindow::onActionExitTriggered);
    connect(m_ui->actionAbout, &QAction::triggered, this, &MainWindow::onActionAboutTriggered);
    connect(m_ui->actionAboutQt, &QAction::triggered, this, &MainWindow::onActionAboutQtTriggered);

    // Connect Central Text Editor
    connect(m_ui->textEditor, &QPlainTextEdit::textChanged, this, &MainWindow::onTextEditorChanged);

    // Connect Dock List Widget
    connect(m_ui->listWidgetProjects, &QListWidget::itemDoubleClicked, this, &MainWindow::onListItemDoubleClicked);

    // Initial Status
    statusBar()->showMessage(QStringLiteral("就绪"), 3000);
    updateStatusBarInfo();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUiCustomizations()
{
    // Add Dock view toggle action to View menu
    m_ui->menuView->addAction(m_ui->dockWidgetProjects->toggleViewAction());

    // Add permanent status bar widgets
    m_statusInfoLabel = new QLabel(this);
    m_statusInfoLabel->setMinimumWidth(180);
    m_statusInfoLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    m_statusEncodingLabel = new QLabel(QStringLiteral(" UTF-8 "), this);
    m_statusEncodingLabel->setStyleSheet(QStringLiteral("QLabel { color: #2b579a; font-weight: bold; }"));

    statusBar()->addPermanentWidget(m_statusInfoLabel);
    statusBar()->addPermanentWidget(m_statusEncodingLabel);
}

void MainWindow::updateStatusBarInfo()
{
    if (!m_statusInfoLabel || !m_ui->textEditor) {
        return;
    }

    int lineCount = m_ui->textEditor->document()->blockCount();
    int charCount = m_ui->textEditor->toPlainText().length();

    m_statusInfoLabel->setText(QStringLiteral("行数: %1 | 字符: %2").arg(lineCount).arg(charCount));
}

void MainWindow::onActionNewTriggered()
{
    m_ui->textEditor->clear();
    m_currentFilePath.clear();
    setWindowTitle(QStringLiteral("Qt 主窗口框架演示 (demo_mainwindow) - [新建文档]"));
    statusBar()->showMessage(QStringLiteral("已新建空白文档"), 3000);
    updateStatusBarInfo();
}

void MainWindow::onActionOpenTriggered()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        QStringLiteral("打开文本文件"),
        QString(),
        QStringLiteral("文本文件 (*.txt *.md *.json *.cpp *.h);;所有文件 (*.*)")
    );

    if (filePath.isEmpty()) {
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, QStringLiteral("打开失败"), QStringLiteral("无法读取文件:\n%1").arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");
    m_ui->textEditor->setPlainText(in.readAll());
    file.close();

    m_currentFilePath = filePath;
    QFileInfo fileInfo(filePath);
    setWindowTitle(QStringLiteral("Qt 主窗口框架演示 - [%1]").arg(fileInfo.fileName()));
    statusBar()->showMessage(QStringLiteral("成功打开文件: %1").arg(fileInfo.fileName()), 3000);
    updateStatusBarInfo();
}

void MainWindow::onActionSaveTriggered()
{
    QString savePath = m_currentFilePath;
    if (savePath.isEmpty()) {
        savePath = QFileDialog::getSaveFileName(
            this,
            QStringLiteral("保存文件"),
            QStringLiteral("untitled.txt"),
            QStringLiteral("文本文件 (*.txt);;Markdown 文件 (*.md);;所有文件 (*.*)")
        );
    }

    if (savePath.isEmpty()) {
        return;
    }

    QFile file(savePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, QStringLiteral("保存失败"), QStringLiteral("无法写入文件:\n%1").arg(file.errorString()));
        return;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << m_ui->textEditor->toPlainText();
    file.close();

    m_currentFilePath = savePath;
    QFileInfo fileInfo(savePath);
    setWindowTitle(QStringLiteral("Qt 主窗口框架演示 - [%1]").arg(fileInfo.fileName()));
    statusBar()->showMessage(QStringLiteral("文件保存成功: %1").arg(fileInfo.fileName()), 3000);
}

void MainWindow::onActionExitTriggered()
{
    close();
}

void MainWindow::onActionAboutTriggered()
{
    QMessageBox::about(
        this,
        QStringLiteral("关于 demo_mainwindow"),
        QStringLiteral("<h3>Qt 标准桌面主窗口框架演示</h3>"
                       "<p>本工程系统演示了 <b>QMainWindow</b> 各核心组成部件：</p>"
                       "<ul>"
                       "<li><b>QMenuBar (菜单栏)</b>：文件、视图与帮助菜单</li>"
                       "<li><b>QToolBar (工具栏)</b>：快捷操作与可拖拽工具栏</li>"
                       "<li><b>QStatusBar (状态栏)</b>：临时消息与永久状态指示</li>"
                       "<li><b>QDockWidget (停靠窗口)</b>：可移动、浮动与停靠的侧边面板</li>"
                       "<li><b>CentralWidget (中心部件)</b>：文本编辑工作区</li>"
                       "</ul>")
    );
}

void MainWindow::onActionAboutQtTriggered()
{
    QMessageBox::aboutQt(this, QStringLiteral("关于 Qt 框架"));
}

void MainWindow::onTextEditorChanged()
{
    updateStatusBarInfo();
}

void MainWindow::onListItemDoubleClicked(QListWidgetItem *item)
{
    if (!item) {
        return;
    }

    QString text = QStringLiteral("// === 双击加载了面板文件: %1 ===\n// 时间: 自动生成演示数据\n\n").arg(item->text());
    m_ui->textEditor->appendPlainText(text);
    statusBar()->showMessage(QStringLiteral("已追加面板内容: %1").arg(item->text()), 3000);
}
