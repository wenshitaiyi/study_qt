#include "page_groupbox.h"
#include "ui_page_groupbox.h"

PageGroupBox::PageGroupBox(QWidget *parent)
    : QWidget(parent)
    , ui(std::make_unique<Ui::PageGroupBox>())
{
    ui->setupUi(this);

    // ========================================================================
    // QGroupBox 核心知识点与 API 说明：
    //  - setCheckable(true): 使分组框标题左侧显示复选框。
    //  - setChecked(bool): 设置勾选状态。
    //  - toggled(bool): 勾选状态改变时发射信号。
    //  - setFlat(bool): 移除 3D 外框，仅保留顶部标题分割线。
    // 
    // 【核心特性与注意事项】：
    //  当 Checkable GroupBox 被取消勾选（unchecked）时，Qt 会自动将其内部容纳的
    //  所有子控件批量设置为禁用状态（setEnabled(false)），无需手动遍历子控件！
    // ========================================================================
    connect(ui->groupCheckable, &QGroupBox::toggled, this, &PageGroupBox::onCheckableToggled);

    connect(ui->btnFlatAction, &QPushButton::clicked, this, [this]() {
        emit logMessage(QStringLiteral("QGroupBox"), QStringLiteral("点击了扁平分组框内的操作按钮"));
    });
}

PageGroupBox::~PageGroupBox() = default;

void PageGroupBox::onCheckableToggled(bool checked)
{
    emit logMessage(QStringLiteral("QGroupBox"),
                    QStringLiteral("代理分组框勾选状态变更: %1 (内部子控件自动批量 %2)")
                    .arg(checked ? QStringLiteral("已启用 (Checked)") : QStringLiteral("已禁用 (Unchecked)"))
                    .arg(checked ? QStringLiteral("启用") : QStringLiteral("置灰禁用")));
}
