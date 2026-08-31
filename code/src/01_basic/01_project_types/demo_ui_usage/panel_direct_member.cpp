#include "panel_direct_member.h"

PanelDirectMember::PanelDirectMember(QWidget *parent)
    : QWidget(parent)
{
    // 直接使用点操作符调用 setupUi
    m_ui.setupUi(this);

    connect(m_ui.btnAction, &QPushButton::clicked, this, &PanelDirectMember::onButtonClicked);
}

void PanelDirectMember::onButtonClicked()
{
    ++m_counter;
    int value = m_ui.spinValue->value();

    m_ui.labelResult->setText(QStringLiteral("[值对象成员 m_ui.] 第 %1 次触发 | 当前数值: %2")
        .arg(m_counter)
        .arg(value));
}
