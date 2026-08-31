#include "main_window.h"
#include "ui_main_window.h"

#include <QMessageBox>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::MainWindow>())
{
    ui->setupUi(this);

    initLayoutsConfig();
}

MainWindow::~MainWindow() = default;

void MainWindow::initLayoutsConfig()
{
    // ========================================================================
    // 1. QHBoxLayout & QVBoxLayout (水平/垂直线性布局)
    // 
    // 【核心知识点与 API 说明】：
    //  - setSpacing(int spacing): 设置布局内子部件之间的间距（像素）。
    //  - setContentsMargins(l, t, r, b): 设置布局边缘与父容器边缘的外边距。
    //  - addStretch(int stretch): 添加弹性拉伸占位符（弹簧），将后续部件顶向对侧。
    //  - setStretch(int index, int stretch): 设置第 index 个子项的拉伸权重比例。
    //  - setAlignment(QWidget *w, Qt::Alignment alignment): 设置特定子控件的单向对齐。
    // 
    // 【注意事项】：
    //  1. 若控件的 SizePolicy 设置为 Fixed，即使设置了 Stretch，该控件也不会放大。
    //  2. 弹簧 spacer 的本质是 QSpacerItem，拉伸比例优先分配给 Stretch 因子大于 0 的项。
    // ========================================================================
    ui->hlayoutDemo->setStretch(0, 1);
    ui->hlayoutDemo->setStretch(1, 2); // 中间按钮获得 2 倍的拉伸权重
    ui->hlayoutDemo->setStretch(2, 1);

    // 垂直布局内设置单项对齐方式
    ui->vlayoutDemo->setAlignment(ui->btnVLeft, Qt::AlignLeft);
    ui->vlayoutDemo->setAlignment(ui->btnVCenter, Qt::AlignHCenter);
    ui->vlayoutDemo->setAlignment(ui->btnVRight, Qt::AlignRight);

    connect(ui->btnH1, &QPushButton::clicked, this, &MainWindow::onHBoxButtonClicked);
    connect(ui->btnH2, &QPushButton::clicked, this, &MainWindow::onHBoxButtonClicked);
    connect(ui->btnH3, &QPushButton::clicked, this, &MainWindow::onHBoxButtonClicked);
    connect(ui->btnRight, &QPushButton::clicked, this, &MainWindow::onHBoxButtonClicked);

    connect(ui->btnVLeft, &QPushButton::clicked, this, &MainWindow::onVBoxButtonClicked);
    connect(ui->btnVCenter, &QPushButton::clicked, this, &MainWindow::onVBoxButtonClicked);
    connect(ui->btnVRight, &QPushButton::clicked, this, &MainWindow::onVBoxButtonClicked);

    // ========================================================================
    // 2. QGridLayout (网格布局)
    // 
    // 【核心知识点与 API 说明】：
    //  - addWidget(widget, row, col): 在指定行 row、列 col 放置控件（索引从 0 开始）。
    //  - addWidget(widget, row, col, rowSpan, colSpan): 跨行跨列合并放置。
    //    * rowSpan: 纵向跨越的行数（>= 1）
    //    * colSpan: 横向跨越的列数（>= 1）
    //  - setRowStretch(row, stretch): 设置某一行的垂直拉伸权重。
    //  - setColumnStretch(col, stretch): 设置某一列的水平拉伸权重。
    //  - setHorizontalSpacing(int) / setVerticalSpacing(int): 独立设置行列间距。
    // 
    // 【注意事项】：
    //  跨行跨列时，严禁在被覆盖的单元格坐标上重复放置其他控件，否则会导致界面重叠渲染混乱。
    // ========================================================================
    connect(ui->btnGridSpanRow, &QPushButton::clicked, this, &MainWindow::onGridCellClicked);
    connect(ui->btnGrid01, &QPushButton::clicked, this, &MainWindow::onGridCellClicked);
    connect(ui->btnGrid02, &QPushButton::clicked, this, &MainWindow::onGridCellClicked);
    connect(ui->btnGridSpanCol, &QPushButton::clicked, this, &MainWindow::onGridCellClicked);
    connect(ui->btnGridFooter, &QPushButton::clicked, this, &MainWindow::onGridCellClicked);

    // ========================================================================
    // 3. QFormLayout (表单布局)
    // 
    // 【核心知识点与 API 说明】：
    //  - addRow(QLabel/QString, QWidget*): 快速添加标准的“标签 + 输入字段”表单行。
    //  - addRow(QWidget*): 添加跨满整行的单一控件（如底部提交按钮）。
    //  - setRowWrapPolicy(RowWrapPolicy): 响应式换行策略（如 DontWrapRows / WrapLongRows）。
    //  - setLabelAlignment(Qt::Alignment): 统一设置左侧所有标签的对齐方式（如右对齐）。
    //  - setFieldGrowthPolicy(FieldGrowthPolicy): 输入控件在窗口拉伸时的扩展策略。
    // 
    // 【注意事项】：
    //  开发登录、注册、参数配置等经典表单时，优先使用 QFormLayout 代替 QGridLayout，
    //  它能自动根据当前操作系统平台规范处理标签与输入框的间距与对齐。
    // ========================================================================
    connect(ui->btnFormSubmit, &QPushButton::clicked, this, &MainWindow::onFormSubmitClicked);

    // ========================================================================
    // 4. QStackedWidget / QStackedLayout (堆叠布局与向导)
    // 
    // 【核心知识点与 API 说明】：
    //  - addWidget(QWidget*) / insertWidget(index, QWidget*): 添加/插入子页面。
    //  - setCurrentIndex(int index): 切换到指定下标的页面（从 0 到 count()-1）。
    //  - setCurrentWidget(QWidget*): 直接通过控件指针切换当前可见页面。
    //  - currentIndex(): 获取当前激活页面的索引。
    //  - currentChanged(int index): 页面切换触发的信号。
    // 
    // 【注意事项】：
    //  QStackedWidget 在任何时刻只有一个子页面处于可见（Visible）状态，其余页面自动隐藏。
    //  常用于分步骤安装向导（Wizard）、多标签内容切换区、单窗口多业务场景切换。
    // ========================================================================
    connect(ui->btnWizardPrev, &QPushButton::clicked, this, &MainWindow::onWizardPrevClicked);
    connect(ui->btnWizardNext, &QPushButton::clicked, this, &MainWindow::onWizardNextClicked);
    connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, &MainWindow::onWizardPageChanged);
    updateWizardNavigation();

    // ========================================================================
    // 5. 动态参数调谐实验室
    // 
    // 【核心知识点与 API 说明】：
    //  运行时动态调用 setContentsMargins、setSpacing、setStretch 会立即触发 Qt 布局
    //  重新计算几何尺寸并刷新绘制（Invalidate & Relayout）。
    // ========================================================================
    connect(ui->sliderMargin, &QSlider::valueChanged, this, &MainWindow::onMarginSliderChanged);
    connect(ui->sliderSpacing, &QSlider::valueChanged, this, &MainWindow::onSpacingSliderChanged);
    connect(ui->btnStretch111, &QPushButton::clicked, this, &MainWindow::onStretch111Clicked);
    connect(ui->btnStretch121, &QPushButton::clicked, this, &MainWindow::onStretch121Clicked);
    connect(ui->btnStretch132, &QPushButton::clicked, this, &MainWindow::onStretch132Clicked);
}

void MainWindow::setStatusTip(const QString &tip)
{
    QString timeStr = QDateTime::currentDateTime().toString(QStringLiteral("HH:mm:ss"));
    ui->labelStatus->setText(QStringLiteral("[%1] %2").arg(timeStr).arg(tip));
}

// ----------------------------------------------------------------------------
// Tab 1 事件
// ----------------------------------------------------------------------------
void MainWindow::onHBoxButtonClicked()
{
    auto *btn = qobject_cast<QPushButton *>(sender());
    if (btn) {
        setStatusTip(QStringLiteral("点击了水平布局按钮：%1").arg(btn->text()));
    }
}

void MainWindow::onVBoxButtonClicked()
{
    auto *btn = qobject_cast<QPushButton *>(sender());
    if (btn) {
        setStatusTip(QStringLiteral("点击了垂直布局按钮：%1").arg(btn->text()));
    }
}

// ----------------------------------------------------------------------------
// Tab 2 事件
// ----------------------------------------------------------------------------
void MainWindow::onGridCellClicked()
{
    auto *btn = qobject_cast<QPushButton *>(sender());
    if (btn) {
        setStatusTip(QStringLiteral("点击了网格布局单元格：%1").arg(btn->text()));
    }
}

// ----------------------------------------------------------------------------
// Tab 3 事件
// ----------------------------------------------------------------------------
void MainWindow::onFormSubmitClicked()
{
    QString info = QStringLiteral(
        "【QFormLayout 表单数据提交成功】\n\n"
        "• 用户名：%1\n"
        "• 角色权限：%2\n"
        "• 年龄：%3 岁\n"
        "• 备注信息：%4"
    ).arg(ui->editUsername->text())
     .arg(ui->comboRole->currentText())
     .arg(ui->spinAge->value())
     .arg(ui->editRemark->text().isEmpty() ? QStringLiteral("（无）") : ui->editRemark->text());

    QMessageBox::information(this, QStringLiteral("表单提交"), info);
    setStatusTip(QStringLiteral("表单数据提交成功：用户 %1").arg(ui->editUsername->text()));
}

// ----------------------------------------------------------------------------
// Tab 4 事件 (向导)
// ----------------------------------------------------------------------------
void MainWindow::onWizardPrevClicked()
{
    int current = ui->stackedWidget->currentIndex();
    if (current > 0) {
        ui->stackedWidget->setCurrentIndex(current - 1);
    }
}

void MainWindow::onWizardNextClicked()
{
    int current = ui->stackedWidget->currentIndex();
    int count = ui->stackedWidget->count();

    if (current < count - 1) {
        ui->stackedWidget->setCurrentIndex(current + 1);
    } else {
        QMessageBox::information(this, QStringLiteral("向导完成"),
                                 QStringLiteral("恭喜！已成功走完全部向导步骤 (QStackedWidget)！"));
        setStatusTip(QStringLiteral("配置向导流程已顺利结束。"));
    }
}

void MainWindow::onWizardPageChanged(int index)
{
    Q_UNUSED(index);
    updateWizardNavigation();
}

void MainWindow::updateWizardNavigation()
{
    int current = ui->stackedWidget->currentIndex();
    int count = ui->stackedWidget->count();

    ui->labelStepIndicator->setText(QStringLiteral("当前页面: %1 / %2").arg(current + 1).arg(count));
    ui->btnWizardPrev->setEnabled(current > 0);

    if (current == count - 1) {
        ui->btnWizardNext->setText(QStringLiteral("✔ 完成向导"));
    } else {
        ui->btnWizardNext->setText(QStringLiteral("下一步 ▶"));
    }

    setStatusTip(QStringLiteral("向导切换至第 %1 页 (共 %2 页)").arg(current + 1).arg(count));
}

// ----------------------------------------------------------------------------
// Tab 5 事件 (动态调参)
// ----------------------------------------------------------------------------
void MainWindow::onMarginSliderChanged(int value)
{
    ui->labelMarginValue->setText(QStringLiteral("%1 px").arg(value));
    // 动态更新布局外边距
    ui->dynamicTargetLayout->setContentsMargins(value, value, value, value);
    setStatusTip(QStringLiteral("调用 setContentsMargins(%1, %1, %1, %1) 动态更新外边距").arg(value));
}

void MainWindow::onSpacingSliderChanged(int value)
{
    ui->labelSpacingValue->setText(QStringLiteral("%1 px").arg(value));
    // 动态更新子项间距
    ui->dynamicTargetLayout->setSpacing(value);
    setStatusTip(QStringLiteral("调用 setSpacing(%1) 动态更新子项间距").arg(value));
}

void MainWindow::onStretch111Clicked()
{
    ui->dynamicTargetLayout->setStretch(0, 1);
    ui->dynamicTargetLayout->setStretch(1, 1);
    ui->dynamicTargetLayout->setStretch(2, 1);
    ui->btnBoxA->setText(QStringLiteral("部件 A (Stretch 1)"));
    ui->btnBoxB->setText(QStringLiteral("部件 B (Stretch 1)"));
    ui->btnBoxC->setText(QStringLiteral("部件 C (Stretch 1)"));
    setStatusTip(QStringLiteral("调用 setStretch 设置均分权重比例 1 : 1 : 1"));
}

void MainWindow::onStretch121Clicked()
{
    ui->dynamicTargetLayout->setStretch(0, 1);
    ui->dynamicTargetLayout->setStretch(1, 2);
    ui->dynamicTargetLayout->setStretch(2, 1);
    ui->btnBoxA->setText(QStringLiteral("部件 A (Stretch 1)"));
    ui->btnBoxB->setText(QStringLiteral("部件 B (Stretch 2，占 50% 空间)"));
    ui->btnBoxC->setText(QStringLiteral("部件 C (Stretch 1)"));
    setStatusTip(QStringLiteral("调用 setStretch 设置主要权重比例 1 : 2 : 1"));
}

void MainWindow::onStretch132Clicked()
{
    ui->dynamicTargetLayout->setStretch(0, 1);
    ui->dynamicTargetLayout->setStretch(1, 3);
    ui->dynamicTargetLayout->setStretch(2, 2);
    ui->btnBoxA->setText(QStringLiteral("部件 A (Stretch 1)"));
    ui->btnBoxB->setText(QStringLiteral("部件 B (Stretch 3 核心区)"));
    ui->btnBoxC->setText(QStringLiteral("部件 C (Stretch 2 侧边栏)"));
    setStatusTip(QStringLiteral("调用 setStretch 设置阶梯权重比例 1 : 3 : 2"));
}
