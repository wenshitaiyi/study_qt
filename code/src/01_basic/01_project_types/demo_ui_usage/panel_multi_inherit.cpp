#include "panel_multi_inherit.h"

PanelMultiInherit::PanelMultiInherit(QWidget *parent)
    : QWidget(parent)
{
    // 直接调用 setupUi，无 ui 前缀
    setupUi(this);

    // 直接访问 UI 控件成员 btnAction
    connect(btnAction, &QPushButton::clicked, this, &PanelMultiInherit::onButtonClicked);
}

void PanelMultiInherit::onButtonClicked()
{
    ++m_counter;
    // 直接访问 UI 控件成员 comboCategory 与 labelResult
    QString category = comboCategory->currentText();

    labelResult->setText(QStringLiteral("[多重继承直接访问] 第 %1 次触发 | 当前类别: %2")
        .arg(m_counter)
        .arg(category));
}
