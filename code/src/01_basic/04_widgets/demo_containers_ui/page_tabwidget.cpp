#include "page_tabwidget.h"
#include "ui_page_tabwidget.h"

#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

PageTabWidget::PageTabWidget(QWidget *parent)
    : QWidget(parent)
    , ui(std::make_unique<Ui::PageTabWidget>())
{
    ui->setupUi(this);

    initCornerWidget();

    // ========================================================================
    // QTabWidget 核心知识点与 API 说明：
    //  - setTabPosition(TabPosition): 设置标签头所在方位（North/South/West/East）。
    //  - setTabsClosable(bool): 开启每个 Tab 标签头右侧的“×”关闭按钮。
    //  - setMovable(bool): 允许用户鼠标拖拽 Tab 标签头调整顺序。
    //  - setCornerWidget(QWidget*, Qt::Corner): 在 Tab 栏的右上角/左上角放置自定义控件。
    //  - tabCloseRequested(int index): 用户点击“×”时触发此信号。
    // 
    // 【核心特性与注意事项】：
    //  触发 tabCloseRequested 信号时，Qt 默认不会自动销毁删除该 Widget，
    //  开发者必须在槽函数中显式调用 removeTab(index) 并处理内存释放（delete widget）。
    // ========================================================================
    connect(ui->comboTabPos, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PageTabWidget::onTabPositionChanged);
    connect(ui->chkClosable, &QCheckBox::toggled, this, &PageTabWidget::onClosableToggled);
    connect(ui->chkMovable, &QCheckBox::toggled, this, &PageTabWidget::onMovableToggled);
    connect(ui->btnAddNewTab, &QPushButton::clicked, this, &PageTabWidget::onAddNewTabClicked);
    connect(ui->tabWidgetDemo, &QTabWidget::tabCloseRequested, this, &PageTabWidget::onTabCloseRequested);
    connect(ui->tabWidgetDemo, &QTabWidget::currentChanged, this, &PageTabWidget::onCurrentTabChanged);
}

PageTabWidget::~PageTabWidget() = default;

void PageTabWidget::initCornerWidget()
{
    // 在标签栏右上角放置一个固定的小快捷按钮
    auto *cornerBtn = new QPushButton(QStringLiteral("⚙ 设置"), this);
    cornerBtn->setStyleSheet(QStringLiteral("padding: 2px 8px; font-size: 11px;"));
    connect(cornerBtn, &QPushButton::clicked, this, [this]() {
        emit logMessage(QStringLiteral("QTabWidget"), QStringLiteral("点击了通过 setCornerWidget 放置的右上角角标设置按钮"));
    });
    ui->tabWidgetDemo->setCornerWidget(cornerBtn, Qt::TopRightCorner);
}

void PageTabWidget::onTabPositionChanged(int index)
{
    switch (index) {
    case 0: ui->tabWidgetDemo->setTabPosition(QTabWidget::North); break;
    case 1: ui->tabWidgetDemo->setTabPosition(QTabWidget::South); break;
    case 2: ui->tabWidgetDemo->setTabPosition(QTabWidget::West);  break;
    case 3: ui->tabWidgetDemo->setTabPosition(QTabWidget::East);  break;
    default: break;
    }
    emit logMessage(QStringLiteral("QTabWidget"), QStringLiteral("标签方位切换为: %1").arg(ui->comboTabPos->currentText()));
}

void PageTabWidget::onClosableToggled(bool checked)
{
    ui->tabWidgetDemo->setTabsClosable(checked);
    emit logMessage(QStringLiteral("QTabWidget"), QStringLiteral("设置 setTabsClosable(%1)").arg(checked ? QStringLiteral("true") : QStringLiteral("false")));
}

void PageTabWidget::onMovableToggled(bool checked)
{
    ui->tabWidgetDemo->setMovable(checked);
    emit logMessage(QStringLiteral("QTabWidget"), QStringLiteral("设置 setMovable(%1)").arg(checked ? QStringLiteral("true") : QStringLiteral("false")));
}

void PageTabWidget::onAddNewTabClicked()
{
    auto *newTab = new QWidget(this);
    auto *layout = new QVBoxLayout(newTab);
    auto *label = new QLabel(QStringLiteral("这是动态新建的第 %1 个自定义页面内容").arg(m_newTabCounter), newTab);
    layout->addWidget(label);
    layout->addStretch();

    int newIdx = ui->tabWidgetDemo->addTab(newTab, QStringLiteral("动态Tab #%1").arg(m_newTabCounter));
    ui->tabWidgetDemo->setCurrentIndex(newIdx);

    emit logMessage(QStringLiteral("QTabWidget"), QStringLiteral("调用 addTab() 成功添加新标签页: 动态Tab #%1 (索引: %2)").arg(m_newTabCounter).arg(newIdx));
    ++m_newTabCounter;
}

void PageTabWidget::onTabCloseRequested(int index)
{
    QString tabText = ui->tabWidgetDemo->tabText(index);
    QWidget *tabWidget = ui->tabWidgetDemo->widget(index);

    // 显式移除并释放内存
    ui->tabWidgetDemo->removeTab(index);
    if (tabWidget) {
        tabWidget->deleteLater();
    }

    emit logMessage(QStringLiteral("QTabWidget"), QStringLiteral("响应 tabCloseRequested 信号，成功移除并释放标签页: %1 (原索引: %2)").arg(tabText).arg(index));
}

void PageTabWidget::onCurrentTabChanged(int index)
{
    if (index >= 0) {
        emit logMessage(QStringLiteral("QTabWidget"), QStringLiteral("当前激活标签页变更 -> [%1] (索引: %2)").arg(ui->tabWidgetDemo->tabText(index)).arg(index));
    }
}
