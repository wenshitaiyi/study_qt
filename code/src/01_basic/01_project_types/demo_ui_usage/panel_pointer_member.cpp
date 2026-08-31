#include "panel_pointer_member.h"
#include "ui_panel_pointer_member.h"

PanelPointerMember::PanelPointerMember(QWidget *parent)
    : QWidget(parent)
    , m_ui(std::make_unique<Ui::PanelPointerMember>())
{
    // 调用指针成员的 setupUi
    m_ui->setupUi(this);

    connect(m_ui->btnAction, &QPushButton::clicked, this, &PanelPointerMember::onButtonClicked);
}

PanelPointerMember::~PanelPointerMember() = default;

void PanelPointerMember::onButtonClicked()
{
    ++m_counter;
    QString text = m_ui->editInput->text().trimmed();
    if (text.isEmpty()) {
        text = QStringLiteral("(空内容)");
    }

    m_ui->labelResult->setText(QStringLiteral("[指针成员 m_ui->] 第 %1 次触发 | 输入内容: %2")
        .arg(m_counter)
        .arg(text));
}
