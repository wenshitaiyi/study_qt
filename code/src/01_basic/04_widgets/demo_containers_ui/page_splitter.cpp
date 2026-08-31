#include "page_splitter.h"
#include "ui_page_splitter.h"

PageSplitter::PageSplitter(QWidget *parent)
    : QWidget(parent)
    , ui(std::make_unique<Ui::PageSplitter>())
{
    ui->setupUi(this);

    // ========================================================================
    // QSplitter 核心知识点与 API 说明：
    //  - setOrientation(Qt::Horizontal / Qt::Vertical): 设置分割条水平或垂直拆分。
    //  - setStretchFactor(int index, int stretch): 设置第 index 个子面板的拉伸权重。
    //  - setCollapsible(int index, bool collapsible): 设置是否允许用户将该面板拖拽收缩到 0 像素。
    //  - setSizes(QList<int>): 直接以像素列表设置各子面板的初始具体宽度/高度。
    //  - setHandleWidth(int): 设置拖拽把手分割条的像素宽度。
    //  - splitterMoved(int pos, int index): 用户拖动分割条时的实时信号。
    // ========================================================================
    ui->splitterHorizontal->setStretchFactor(0, 1);
    ui->splitterHorizontal->setStretchFactor(1, 2); // 中央编辑区权重占 2
    ui->splitterHorizontal->setStretchFactor(2, 1);

    // 默认禁止收缩至 0 像素
    ui->splitterHorizontal->setCollapsible(0, false);
    ui->splitterHorizontal->setCollapsible(1, false);
    ui->splitterHorizontal->setCollapsible(2, false);

    connect(ui->btnResetRatio, &QPushButton::clicked, this, &PageSplitter::onResetRatioClicked);
    connect(ui->chkCollapsible, &QCheckBox::toggled, this, &PageSplitter::onCollapsibleToggled);
    connect(ui->splitterHorizontal, &QSplitter::splitterMoved, this, &PageSplitter::onSplitterMoved);
}

PageSplitter::~PageSplitter() = default;

void PageSplitter::onResetRatioClicked()
{
    // 重置分栏像素尺寸 (例如 200px, 400px, 200px)
    QList<int> sizes = {200, 400, 200};
    ui->splitterHorizontal->setSizes(sizes);
    emit logMessage(QStringLiteral("QSplitter"), QStringLiteral("调用 setSizes({200, 400, 200}) 重置分栏初始像素尺寸"));
}

void PageSplitter::onCollapsibleToggled(bool checked)
{
    ui->splitterHorizontal->setCollapsible(0, checked);
    ui->splitterHorizontal->setCollapsible(1, checked);
    ui->splitterHorizontal->setCollapsible(2, checked);
    emit logMessage(QStringLiteral("QSplitter"), QStringLiteral("设置 setCollapsible(%1)").arg(checked ? QStringLiteral("true (允许拖拽折叠至0)") : QStringLiteral("false (禁止折叠)")));
}

void PageSplitter::onSplitterMoved(int pos, int index)
{
    QList<int> currentSizes = ui->splitterHorizontal->sizes();
    QString sizeStr;
    for (int s : currentSizes) {
        sizeStr += QString::number(s) + QStringLiteral("px, ");
    }
    if (sizeStr.endsWith(QStringLiteral(", "))) {
        sizeStr.chop(2);
    }

    emit logMessage(QStringLiteral("QSplitter"),
                    QStringLiteral("分割条移动 (把手索引: %1, 坐标: %2) -> 当前各面板宽度: [%3]")
                    .arg(index).arg(pos).arg(sizeStr));
}
