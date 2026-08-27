#include "main_widget.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFont>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(QStringLiteral("Qt 基础窗口演示 (demo_widget)"));
    resize(400, 250);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(15);

    m_label = new QLabel(QStringLiteral("欢迎进入 Qt 学习体系！点击下方按钮测试："), this);
    QFont font = m_label->font();
    font.setPointSize(11);
    m_label->setFont(font);
    m_label->setAlignment(Qt::AlignCenter);

    m_button = new QPushButton(QStringLiteral("点击测试 (0)"), this);
    m_button->setFixedHeight(36);

    layout->addStretch();
    layout->addWidget(m_label);
    layout->addWidget(m_button);
    layout->addStretch();

    connect(m_button, &QPushButton::clicked, this, &MainWidget::onButtonClicked);
}

void MainWidget::onButtonClicked()
{
    ++m_clickCount;
    m_button->setText(QStringLiteral("已点击 %1 次").arg(m_clickCount));
    m_label->setText(QStringLiteral("状态：信号槽触发成功！当前点击计数 = %1").arg(m_clickCount));
}
