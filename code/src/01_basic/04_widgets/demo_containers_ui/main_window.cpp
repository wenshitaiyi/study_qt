#include "main_window.h"
#include "ui_main_window.h"

#include "page_groupbox.h"
#include "page_tabwidget.h"
#include "page_toolbox.h"
#include "page_scrollarea.h"
#include "page_splitter.h"

#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::MainWindow>())
{
    ui->setupUi(this);

    initSubPages();

    connect(ui->comboNavigation, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onNavigationChanged);
    connect(ui->btnClearLog, &QPushButton::clicked, ui->textLog, &QPlainTextEdit::clear);

    onSubPageLogReceived(QStringLiteral("系统"), QStringLiteral("容器控件多 UI 模块架构初始化完成。通过顶部下拉框切换各容器视图。"));
}

MainWindow::~MainWindow() = default;

void MainWindow::initSubPages()
{
    // 实例化 5 个独立的子 UI 页面并挂载到中央 QStackedWidget
    m_pageGroupBox   = new PageGroupBox(this);
    m_pageTabWidget  = new PageTabWidget(this);
    m_pageToolBox    = new PageToolBox(this);
    m_pageScrollArea = new PageScrollArea(this);
    m_pageSplitter   = new PageSplitter(this);

    ui->stackedContainer->addWidget(m_pageGroupBox);   // Index 0
    ui->stackedContainer->addWidget(m_pageTabWidget);  // Index 1
    ui->stackedContainer->addWidget(m_pageToolBox);    // Index 2
    ui->stackedContainer->addWidget(m_pageScrollArea); // Index 3
    ui->stackedContainer->addWidget(m_pageSplitter);   // Index 4

    // 绑定各子 UI 页面的日志信号
    connect(m_pageGroupBox, &PageGroupBox::logMessage, this, &MainWindow::onSubPageLogReceived);
    connect(m_pageTabWidget, &PageTabWidget::logMessage, this, &MainWindow::onSubPageLogReceived);
    connect(m_pageToolBox, &PageToolBox::logMessage, this, &MainWindow::onSubPageLogReceived);
    connect(m_pageScrollArea, &PageScrollArea::logMessage, this, &MainWindow::onSubPageLogReceived);
    connect(m_pageSplitter, &PageSplitter::logMessage, this, &MainWindow::onSubPageLogReceived);
}

void MainWindow::onNavigationChanged(int index)
{
    ui->stackedContainer->setCurrentIndex(index);
    onSubPageLogReceived(QStringLiteral("导航"),
                         QStringLiteral("顶部导航切换至: %1 (页面索引: %2)")
                         .arg(ui->comboNavigation->currentText())
                         .arg(index));
}

void MainWindow::onSubPageLogReceived(const QString &category, const QString &message)
{
    QString timeStr = QDateTime::currentDateTime().toString(QStringLiteral("HH:mm:ss.zzz"));
    QString line = QStringLiteral("[%1] [%2] %3").arg(timeStr).arg(category.leftJustified(11, ' ')).arg(message);
    ui->textLog->appendPlainText(line);
}
