#include "main_window.h"
#include "ui_main_window.h"

#include <QMessageBox>
#include <QPushButton>
#include <QDateTime>
#include <QVector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::MainWindow>())
{
    ui->setupUi(this);

    // 1. 初始化 Tab 1 动作栏事件
    connect(ui->btnSlot1, &QPushButton::clicked, this, &MainWindow::onSkillSlotClicked);
    connect(ui->btnSlot2, &QPushButton::clicked, this, &MainWindow::onSkillSlotClicked);
    connect(ui->btnSlot3, &QPushButton::clicked, this, &MainWindow::onSkillSlotClicked);
    connect(ui->btnSlot4, &QPushButton::clicked, this, &MainWindow::onSkillSlotClicked);
    connect(ui->btnSlotPotion, &QPushButton::clicked, this, &MainWindow::onSkillSlotClicked);

    // 2. 初始化 Tab 2 4x4 背包网格布局
    initInventoryGrid();

    // 3. 初始化 Tab 3 角色表单
    connect(ui->btnCreateChar, &QPushButton::clicked, this, &MainWindow::onCreateCharClicked);

    // 4. 初始化 Tab 4 向导
    connect(ui->btnWizardPrev, &QPushButton::clicked, this, &MainWindow::onWizardPrev);
    connect(ui->btnWizardNext, &QPushButton::clicked, this, &MainWindow::onWizardNext);
    updateWizardUI();

    // 5. 初始化 Tab 5 动态布局沙盒
    connect(ui->sliderMargin, &QSlider::valueChanged, this, &MainWindow::onMarginChanged);
    connect(ui->sliderSpacing, &QSlider::valueChanged, this, &MainWindow::onSpacingChanged);
    connect(ui->btnStretch111, &QPushButton::clicked, this, &MainWindow::onStretch111);
    connect(ui->btnStretch121, &QPushButton::clicked, this, &MainWindow::onStretch121);
    connect(ui->btnStretch132, &QPushButton::clicked, this, &MainWindow::onStretch132);

    // 设置 Tab 1 装备按钮事件
    connect(ui->btnRing1, &QPushButton::clicked, this, [this]() { setStatus(QStringLiteral("查看饰品: 【誓约之戒】 (暴击率 +5%)")); });
    connect(ui->btnRing2, &QPushButton::clicked, this, [this]() { setStatus(QStringLiteral("查看饰品: 【生命之戒】 (生命上限 +1,200)")); });
    connect(ui->btnTrinket, &QPushButton::clicked, this, [this]() { setStatus(QStringLiteral("查看饰品: 【远古龙晶吊坠】 (伤害加成 +15%)")); });

    setStatus(QStringLiteral("欢迎进入布局演示！点击各个 Tab 查看不同布局管理器的实际应用场景。"));
}

MainWindow::~MainWindow() = default;

void MainWindow::setStatus(const QString &msg)
{
    QString timeStr = QDateTime::currentDateTime().toString(QStringLiteral("HH:mm:ss"));
    ui->labelStatusBar->setText(QStringLiteral("[%1] %2").arg(timeStr).arg(msg));
}

// ============================================================================
// Tab 1: 技能快捷栏
// ============================================================================
void MainWindow::onSkillSlotClicked()
{
    auto *btn = qobject_cast<QPushButton *>(sender());
    if (btn) {
        setStatus(QStringLiteral("释放快捷栏技能: %1").arg(btn->text()));
    }
}

// ============================================================================
// Tab 2: 4x4 背包网格布局 (QGridLayout)
// ============================================================================
void MainWindow::initInventoryGrid()
{
    const QVector<QString> itemNames = {
        QStringLiteral("🗡️ 霜火之刃"), QStringLiteral("🛡️ 守护者之盾"), QStringLiteral("🧪 强效魔力水"), QStringLiteral("📜 传送卷轴"),
        QStringLiteral("💍 秘银戒指"), QStringLiteral("💎 虚空结晶"), QStringLiteral("🍖 烤野猪肉"),   QStringLiteral("🏹 精灵强弓"),
        QStringLiteral("🔮 预言法球"), QStringLiteral("👑 统治王冠"), QStringLiteral("👢 疾风之靴"),   QStringLiteral("🧤 力量护手"),
        QStringLiteral("🪙 古代银币"), QStringLiteral("🗝️ 地牢钥匙"), QStringLiteral("🪵 远古神木"),   QStringLiteral("🪓 狂暴战斧")
    };

    int index = 0;
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            if (index < itemNames.size()) {
                auto *itemBtn = new QPushButton(itemNames[index], this);
                itemBtn->setMinimumSize(90, 60);
                itemBtn->setStyleSheet(QStringLiteral("font-size: 11px; background-color: #ecf0f1; border: 1px solid #bdc3c7; border-radius: 4px;"));
                connect(itemBtn, &QPushButton::clicked, this, &MainWindow::onInventoryItemClicked);

                // 核心：使用 QGridLayout::addWidget 添加到指定行列
                ui->gridLayoutInventory->addWidget(itemBtn, row, col);
                ++index;
            }
        }
    }
}

void MainWindow::onInventoryItemClicked()
{
    auto *btn = qobject_cast<QPushButton *>(sender());
    if (btn) {
        setStatus(QStringLiteral("点击了背包物品: %1").arg(btn->text()));
    }
}

// ============================================================================
// Tab 3: 角色创建表单 (QFormLayout)
// ============================================================================
void MainWindow::onCreateCharClicked()
{
    QString name = ui->editCharName->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("输入错误"), QStringLiteral("角色昵称不能为空！"));
        return;
    }

    QString info = QStringLiteral(
        "🎉 英雄角色注册创建成功！\n\n"
        "• 角色昵称：%1\n"
        "• 初始职业：%2\n"
        "• 力量属性：%3 点\n"
        "• 敏捷属性：%4 点\n"
        "• 智力属性：%5 点\n"
        "• 英雄传记：%6"
    ).arg(name)
     .arg(ui->comboClass->currentText())
     .arg(ui->spinStrength->value())
     .arg(ui->spinAgility->value())
     .arg(ui->spinIntellect->value())
     .arg(ui->editBio->text().isEmpty() ? QStringLiteral("（无传记）") : ui->editBio->text());

    QMessageBox::information(this, QStringLiteral("角色创建成功"), info);
    setStatus(QStringLiteral("成功创建新英雄角色：%1 (%2)").arg(name).arg(ui->comboClass->currentText()));
}

// ============================================================================
// Tab 4: 职业二转觉醒向导 (QStackedLayout / QStackedWidget)
// ============================================================================
void MainWindow::onWizardPrev()
{
    int current = ui->stackedWidgetWizard->currentIndex();
    if (current > 0) {
        ui->stackedWidgetWizard->setCurrentIndex(current - 1);
        updateWizardUI();
    }
}

void MainWindow::onWizardNext()
{
    int current = ui->stackedWidgetWizard->currentIndex();
    int count = ui->stackedWidgetWizard->count();

    if (current < count - 1) {
        ui->stackedWidgetWizard->setCurrentIndex(current + 1);
        updateWizardUI();
    } else {
        // 最后一步点击完成
        QString spec = QStringLiteral("未知");
        if (ui->radioSpecRetribution->isChecked()) spec = QStringLiteral("⚔️ 惩戒骑士");
        else if (ui->radioSpecProtection->isChecked()) spec = QStringLiteral("🛡️ 防护领主");
        else if (ui->radioSpecHoly->isChecked()) spec = QStringLiteral("✨ 神圣祭司");

        QMessageBox::information(this, QStringLiteral("二转觉醒完成"),
                                 QStringLiteral("恭喜！您已成功完成二转誓约仪式，晋升为【%1】！").arg(spec));
        setStatus(QStringLiteral("完成二转觉醒向导流程，当前专精：%1").arg(spec));
    }
}

void MainWindow::updateWizardUI()
{
    int current = ui->stackedWidgetWizard->currentIndex();
    int count = ui->stackedWidgetWizard->count();

    ui->labelStepIndicator->setText(QStringLiteral("步骤: %1 / %2").arg(current + 1).arg(count));
    ui->btnWizardPrev->setEnabled(current > 0);

    if (current == count - 1) {
        ui->btnWizardNext->setText(QStringLiteral("👑 完成誓约"));
    } else {
        ui->btnWizardNext->setText(QStringLiteral("下一步 ▶"));
    }

    setStatus(QStringLiteral("觉醒向导切换到第 %1 步").arg(current + 1));
}

// ============================================================================
// Tab 5: 动态布局调谐实验室
// ============================================================================
void MainWindow::onMarginChanged(int value)
{
    ui->labelMarginVal->setText(QStringLiteral("%1 px").arg(value));
    ui->sandboxTargetLayout->setContentsMargins(value, value, value, value);
    setStatus(QStringLiteral("沙盒外边距 (ContentsMargins) 调整为: %1 px").arg(value));
}

void MainWindow::onSpacingChanged(int value)
{
    ui->labelSpacingVal->setText(QStringLiteral("%1 px").arg(value));
    ui->sandboxTargetLayout->setSpacing(value);
    setStatus(QStringLiteral("沙盒部件间距 (Spacing) 调整为: %1 px").arg(value));
}

void MainWindow::onStretch111()
{
    ui->sandboxTargetLayout->setStretch(0, 1);
    ui->sandboxTargetLayout->setStretch(1, 1);
    ui->sandboxTargetLayout->setStretch(2, 1);
    ui->btnSandbox1->setText(QStringLiteral("模块 A (Stretch 1)"));
    ui->btnSandbox2->setText(QStringLiteral("模块 B (Stretch 1)"));
    ui->btnSandbox3->setText(QStringLiteral("模块 C (Stretch 1)"));
    setStatus(QStringLiteral("应用均分拉伸比: 1 : 1 : 1"));
}

void MainWindow::onStretch121()
{
    ui->sandboxTargetLayout->setStretch(0, 1);
    ui->sandboxTargetLayout->setStretch(1, 2);
    ui->sandboxTargetLayout->setStretch(2, 1);
    ui->btnSandbox1->setText(QStringLiteral("模块 A (Stretch 1)"));
    ui->btnSandbox2->setText(QStringLiteral("模块 B (Stretch 2 中间主区域)"));
    ui->btnSandbox3->setText(QStringLiteral("模块 C (Stretch 1)"));
    setStatus(QStringLiteral("应用主次拉伸比: 1 : 2 : 1 (中间部件占 50% 宽度)"));
}

void MainWindow::onStretch132()
{
    ui->sandboxTargetLayout->setStretch(0, 1);
    ui->sandboxTargetLayout->setStretch(1, 3);
    ui->sandboxTargetLayout->setStretch(2, 2);
    ui->btnSandbox1->setText(QStringLiteral("模块 A (Stretch 1)"));
    ui->btnSandbox2->setText(QStringLiteral("模块 B (Stretch 3 核心区)"));
    ui->btnSandbox3->setText(QStringLiteral("模块 C (Stretch 2 侧边栏)"));
    setStatus(QStringLiteral("应用阶梯拉伸比: 1 : 3 : 2"));
}
