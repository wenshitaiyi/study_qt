#include "page_scrollarea.h"
#include "ui_page_scrollarea.h"

#include <QScrollBar>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>

PageScrollArea::PageScrollArea(QWidget *parent)
    : QWidget(parent)
    , ui(std::make_unique<Ui::PageScrollArea>())
{
    ui->setupUi(this);

    // ========================================================================
    // QScrollArea 核心知识点与 API 说明：
    //  - setWidget(QWidget *w): 将需要滚动的目标大控件放入滚动视口。
    //  - setWidgetResizable(bool):
    //    * true: 视口变大时，内部控件会自动随之拉伸填满视口空间。
    //    * false: 内部控件严格保持其自身的大小提示（SizeHint），可能出现大片空白。
    //  - setHorizontalScrollBarPolicy / setVerticalScrollBarPolicy:
    //    * Qt::ScrollBarAsNeeded (按需自动出现)
    //    * Qt::ScrollBarAlwaysOn (始终常开)
    //    * Qt::ScrollBarAlwaysOff (始终关闭)
    // 
    // 【核心特性与注意事项】：
    //  若使用 setWidgetResizable(true)，内部部件必须配置完备的布局管理器（如 QVBoxLayout），
    //  并且建议在最底部添加一个 addStretch() 弹簧，防止少量内容被非预期拉伸过大。
    // ========================================================================
    populateScrollCards();

    connect(ui->chkWidgetResizable, &QCheckBox::toggled, this, &PageScrollArea::onWidgetResizableToggled);
    connect(ui->comboHBarPolicy, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PageScrollArea::onHBarPolicyChanged);
    connect(ui->btnScrollToBottom, &QPushButton::clicked, this, &PageScrollArea::onScrollToBottomClicked);
}

PageScrollArea::~PageScrollArea() = default;

void PageScrollArea::populateScrollCards()
{
    for (int i = 1; i <= 15; ++i) {
        auto *card = new QGroupBox(QStringLiteral("模块卡片 #%1").arg(i), ui->scrollAreaWidgetContents);
        auto *cardLayout = new QHBoxLayout(card);
        auto *label = new QLabel(QStringLiteral("这是位于滚动区域内的第 %1 个独立卡片容器，包含描述文本与交互按钮。").arg(i), card);
        auto *btn = new QPushButton(QStringLiteral("卡片 #%1 操作").arg(i), card);

        connect(btn, &QPushButton::clicked, this, [this, i]() {
            emit logMessage(QStringLiteral("QScrollArea"), QStringLiteral("点击了滚动卡片 #%1 中的操作按钮").arg(i));
        });

        cardLayout->addWidget(label);
        cardLayout->addWidget(btn);
        ui->vboxScrollContent->addWidget(card);
    }
}

void PageScrollArea::onWidgetResizableToggled(bool checked)
{
    ui->scrollAreaDemo->setWidgetResizable(checked);
    emit logMessage(QStringLiteral("QScrollArea"), QStringLiteral("设置 setWidgetResizable(%1)").arg(checked ? QStringLiteral("true") : QStringLiteral("false")));
}

void PageScrollArea::onHBarPolicyChanged(int index)
{
    switch (index) {
    case 0: ui->scrollAreaDemo->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded); break;
    case 1: ui->scrollAreaDemo->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn); break;
    case 2: ui->scrollAreaDemo->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); break;
    default: break;
    }
    emit logMessage(QStringLiteral("QScrollArea"), QStringLiteral("水平滚动条策略变更为: %1").arg(ui->comboHBarPolicy->currentText()));
}

void PageScrollArea::onScrollToBottomClicked()
{
    QScrollBar *vbar = ui->scrollAreaDemo->verticalScrollBar();
    if (vbar) {
        vbar->setValue(vbar->maximum());
        emit logMessage(QStringLiteral("QScrollArea"), QStringLiteral("垂直滚动条滚动至最大值 (底部: %1)").arg(vbar->maximum()));
    }
}
