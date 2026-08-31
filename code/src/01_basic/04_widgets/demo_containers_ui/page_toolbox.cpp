#include "page_toolbox.h"
#include "ui_page_toolbox.h"

#include <QLabel>
#include <QVBoxLayout>

PageToolBox::PageToolBox(QWidget *parent)
    : QWidget(parent)
    , ui(std::make_unique<Ui::PageToolBox>())
{
    ui->setupUi(this);

    // ========================================================================
    // QToolBox 核心知识点与 API 说明：
    //  - addItem(QWidget *w, const QString &text): 追加一个抽屉项。
    //  - insertItem(int index, QWidget *w, const QString &text): 在指定位置插入抽屉。
    //  - removeItem(int index): 移除指定抽屉（注意：不会自动 delete 该 Widget 内存）。
    //  - setCurrentIndex(int index): 代码控制展开指定抽屉。
    //  - currentChanged(int index): 抽屉展开切换时发射的信号。
    // ========================================================================
    connect(ui->toolBoxDemo, &QToolBox::currentChanged, this, &PageToolBox::onToolboxCurrentChanged);
    connect(ui->btnSwitchNextDrawer, &QPushButton::clicked, this, &PageToolBox::onSwitchNextDrawerClicked);
    connect(ui->btnAddDrawer, &QPushButton::clicked, this, &PageToolBox::onAddDrawerClicked);

    connect(ui->btnFriend1, &QPushButton::clicked, this, [this]() { emit logMessage(QStringLiteral("QToolBox"), QStringLiteral("点击好友：张三 (在线)")); });
    connect(ui->btnFriend2, &QPushButton::clicked, this, [this]() { emit logMessage(QStringLiteral("QToolBox"), QStringLiteral("点击好友：李四 (在线)")); });
    connect(ui->btnFriend3, &QPushButton::clicked, this, [this]() { emit logMessage(QStringLiteral("QToolBox"), QStringLiteral("点击好友：王五 (离线)")); });
    connect(ui->btnGroup1, &QPushButton::clicked, this, [this]() { emit logMessage(QStringLiteral("QToolBox"), QStringLiteral("进入群组：C++ / Qt 核心技术架构组")); });
    connect(ui->btnGroup2, &QPushButton::clicked, this, [this]() { emit logMessage(QStringLiteral("QToolBox"), QStringLiteral("进入群组：UI / UX 交互设计规范组")); });
}

PageToolBox::~PageToolBox() = default;

void PageToolBox::onToolboxCurrentChanged(int index)
{
    if (index >= 0) {
        emit logMessage(QStringLiteral("QToolBox"),
                        QStringLiteral("抽屉面板展开项切换 -> [%1] (索引: %2)")
                        .arg(ui->toolBoxDemo->itemText(index))
                        .arg(index));
    }
}

void PageToolBox::onSwitchNextDrawerClicked()
{
    int current = ui->toolBoxDemo->currentIndex();
    int count = ui->toolBoxDemo->count();
    int next = (current + 1) % count;
    ui->toolBoxDemo->setCurrentIndex(next);
}

void PageToolBox::onAddDrawerClicked()
{
    auto *newWidget = new QWidget(this);
    auto *layout = new QVBoxLayout(newWidget);
    auto *label = new QLabel(QStringLiteral("这是动态添加的第 %1 个抽屉内容区域").arg(m_drawerCounter), newWidget);
    layout->addWidget(label);
    layout->addStretch();

    int newIdx = ui->toolBoxDemo->addItem(newWidget, QStringLiteral("📁 自定义分类 #%1").arg(m_drawerCounter));
    ui->toolBoxDemo->setCurrentIndex(newIdx);

    emit logMessage(QStringLiteral("QToolBox"), QStringLiteral("调用 addItem() 追加新抽屉: 自定义分类 #%1 (索引: %2)").arg(m_drawerCounter).arg(newIdx));
    ++m_drawerCounter;
}
