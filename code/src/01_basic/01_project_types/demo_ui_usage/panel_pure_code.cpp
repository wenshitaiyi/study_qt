#include "panel_pure_code.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QSlider>

PanelPureCode::PanelPureCode(QWidget *parent)
    : QWidget(parent)
{
    initUiLayout();
}

void PanelPureCode::initUiLayout()
{
    // 主垂直布局
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(12);

    // 1. 原理介绍分组框
    auto *groupIntro = new QGroupBox(QStringLiteral("【形式 4】纯代码手工构建方式 (Pure Code Layout)"), this);
    auto *introLayout = new QVBoxLayout(groupIntro);

    auto *labelIntro = new QLabel(this);
    labelIntro->setWordWrap(true);
    labelIntro->setText(QStringLiteral(
        "<b>原理与特点：</b><br>"
        "• 完全不使用 <code>.ui</code> 文件，无需 <code>uic</code> 编译器预处理。<br>"
        "• 在 C++ 构造函数或私有初始化方法中，直接通过 <code>new</code> 创建各控件与布局管理器（<code>QVBoxLayout</code>, <code>QHBoxLayout</code>）。<br>"
        "• 通过 <code>parent</code> 参数建立 Qt 对象树生命周期，自动管理内存。<br>"
        "• <b>优势</b>：动态构建能力强（可根据后台配置动态增删控件）、无预编译依赖、极适合封装通用独立自定义控件。"
    ));
    introLayout->addWidget(labelIntro);
    mainLayout->addWidget(groupIntro);

    // 2. 交互控制分组框
    auto *groupInteractive = new QGroupBox(QStringLiteral("组件交互区（纯代码创建的 Slider 与 Button）"), this);
    auto *interactiveLayout = new QHBoxLayout(groupInteractive);
    interactiveLayout->setSpacing(10);

    auto *labelSliderHint = new QLabel(QStringLiteral("滑动条调节："), this);
    m_slider = new QSlider(Qt::Horizontal, this);
    m_slider->setRange(0, 100);
    m_slider->setValue(50);

    m_btnReset = new QPushButton(QStringLiteral("重置为 50"), this);

    interactiveLayout->addWidget(labelSliderHint);
    interactiveLayout->addWidget(m_slider, 1);
    interactiveLayout->addWidget(m_btnReset);
    mainLayout->addWidget(groupInteractive);

    // 3. 结果显示
    m_labelResult = new QLabel(QStringLiteral("就绪：当前滑动条数值 = 50%"), this);
    m_labelResult->setStyleSheet(QStringLiteral("QLabel { background: #f4f4f5; border: 1px solid #e9e9eb; padding: 10px; border-radius: 4px; color: #909399; font-family: Consolas; }"));
    mainLayout->addWidget(m_labelResult);

    mainLayout->addStretch(1);

    // 绑定信号槽
    connect(m_slider, &QSlider::valueChanged, this, &PanelPureCode::onSliderValueChanged);
    connect(m_btnReset, &QPushButton::clicked, this, &PanelPureCode::onResetClicked);
}

void PanelPureCode::onSliderValueChanged(int value)
{
    m_labelResult->setText(QStringLiteral("[纯代码布局实时更新] 滑动条当前数值: %1%").arg(value));
}

void PanelPureCode::onResetClicked()
{
    m_slider->setValue(50);
    m_labelResult->setText(QStringLiteral("[纯代码布局] 已重置滑动条数值为 50%"));
}
