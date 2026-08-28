#include "main_widget.h"
#include "ui_main_widget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , m_ui(std::make_unique<Ui::MainWidget>())
{
    m_ui->setupUi(this);

    connect(m_ui->buttonTest, &QPushButton::clicked, this, &MainWidget::onButtonClicked);
}

MainWidget::~MainWidget() = default;

void MainWidget::onButtonClicked()
{
    ++m_clickCount;
    m_ui->buttonTest->setText(QStringLiteral("已点击 %1 次").arg(m_clickCount));
    m_ui->labelPrompt->setText(QStringLiteral("状态：UI 信号槽触发成功！当前点击计数 = %1").arg(m_clickCount));
}
