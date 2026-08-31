#include "main_window.h"
#include "ui_main_window.h"

#include <QDateTime>
#include <QMessageBox>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::MainWindow>())
{
    ui->setupUi(this);

    initMenusAndButtons();

    appendLog(QStringLiteral("系统"), QStringLiteral("游戏控制台初始化就绪，欢迎来到艾泽拉斯大陆！"));
}

MainWindow::~MainWindow() = default;

void MainWindow::initMenusAndButtons()
{
    // ========================================================================
    // 1. QPushButton 初始化与信号槽
    // ========================================================================
    connect(ui->btnNormalAttack, &QPushButton::clicked, this, &MainWindow::onNormalAttack);
    connect(ui->btnUltimateSkill, &QPushButton::clicked, this, &MainWindow::onUltimateSkill);
    connect(ui->btnToggleStance, &QPushButton::toggled, this, &MainWindow::onToggleStance);

    // 技能连招下拉菜单
    auto *comboMenu = new QMenu(this);
    auto *actCombo1 = comboMenu->addAction(QStringLiteral("🔥 毁灭打击 (伤害 +150%)"));
    auto *actCombo2 = comboMenu->addAction(QStringLiteral("⚡ 旋风斩 (群体 AOE 8 段)"));
    auto *actCombo3 = comboMenu->addAction(QStringLiteral("☠️ 斩杀 (对低血量暴击)"));
    ui->btnComboMenu->setMenu(comboMenu);
    connect(comboMenu, &QMenu::triggered, this, &MainWindow::onComboActionTriggered);

    // 技能 CD 计时器
    m_cooldownTimer = new QTimer(this);
    connect(m_cooldownTimer, &QTimer::timeout, this, &MainWindow::onCooldownTick);

    // ========================================================================
    // 2. QToolButton 模式与箭头
    // ========================================================================
    // 副本选择菜单 (MenuButtonPopup)
    auto *dungeonMenu = new QMenu(this);
    dungeonMenu->addAction(QStringLiteral("🟢 普通模式 (Lv.20)"));
    dungeonMenu->addAction(QStringLiteral("🟡 英雄模式 (Lv.50)"));
    dungeonMenu->addAction(QStringLiteral("🔴 史诗深渊 (Lv.85)"));
    ui->toolBtnDungeon->setMenu(dungeonMenu);
    connect(dungeonMenu, &QMenu::triggered, this, &MainWindow::onDungeonActionTriggered);
    connect(ui->toolBtnDungeon, &QToolButton::clicked, this, [this]() {
        appendLog(QStringLiteral("副本"), QStringLiteral("点击了副本主按钮，默认进入已选模式。"));
    });

    // 全图雷达菜单 (InstantPopup)
    auto *radarMenu = new QMenu(this);
    radarMenu->addAction(QStringLiteral("👥 扫描友方公会成员"));
    radarMenu->addAction(QStringLiteral("👹 扫描世界 Boss 坐标"));
    radarMenu->addAction(QStringLiteral("💎 扫描隐藏远古宝箱"));
    ui->toolBtnRadar->setMenu(radarMenu);
    connect(radarMenu, &QMenu::triggered, this, &MainWindow::onRadarActionTriggered);

    // 左右箭头换页
    ui->toolBtnPrev->setArrowType(Qt::LeftArrow);
    ui->toolBtnNext->setArrowType(Qt::RightArrow);
    connect(ui->toolBtnPrev, &QToolButton::clicked, this, &MainWindow::onPrevPage);
    connect(ui->toolBtnNext, &QToolButton::clicked, this, &MainWindow::onNextPage);
    connect(ui->toolBtnAutoRaise, &QToolButton::clicked, this, &MainWindow::onAutoRaiseClicked);

    // ========================================================================
    // 3. QRadioButton & QButtonGroup
    // ========================================================================
    m_campGroup = new QButtonGroup(this);
    m_campGroup->addButton(ui->radioAlliance, 1);
    m_campGroup->addButton(ui->radioHorde, 2);
    m_campGroup->addButton(ui->radioNeutral, 3);
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    connect(m_campGroup, &QButtonGroup::idClicked, this, &MainWindow::onCampChanged);
#else
    connect(m_campGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &MainWindow::onCampChanged);
#endif

    m_qualityGroup = new QButtonGroup(this);
    m_qualityGroup->addButton(ui->radioQualityUltra, 10);
    m_qualityGroup->addButton(ui->radioQualityHigh, 20);
    m_qualityGroup->addButton(ui->radioQualityPower, 30);
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    connect(m_qualityGroup, &QButtonGroup::idClicked, this, &MainWindow::onQualityChanged);
#else
    connect(m_qualityGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &MainWindow::onQualityChanged);
#endif

    // ========================================================================
    // 4. QCheckBox 挂机与三态拾取
    // ========================================================================
    connect(ui->chkAutoBattle, &QCheckBox::toggled, this, &MainWindow::onAutoBattleToggled);
    connect(ui->chkAutoPotion, &QCheckBox::toggled, this, [this](bool c) {
        appendLog(QStringLiteral("挂机辅助"), QStringLiteral("低血量自动饮用生命药剂: %1").arg(c ? QStringLiteral("开启") : QStringLiteral("关闭")));
    });

    connect(ui->chkLootAll, &QCheckBox::stateChanged, this, &MainWindow::onLootAllStateChanged);
    connect(ui->chkLootGold, &QCheckBox::toggled, this, &MainWindow::onSubLootChanged);
    connect(ui->chkLootEquip, &QCheckBox::toggled, this, &MainWindow::onSubLootChanged);
    connect(ui->chkLootMaterial, &QCheckBox::toggled, this, &MainWindow::onSubLootChanged);
    updateLootAllState();

    // ========================================================================
    // 5. QMessageBox 弹窗系列
    // ========================================================================
    connect(ui->btnMsgInfo, &QPushButton::clicked, this, &MainWindow::onMsgInfo);
    connect(ui->btnMsgWarning, &QPushButton::clicked, this, &MainWindow::onMsgWarning);
    connect(ui->btnMsgCritical, &QPushButton::clicked, this, &MainWindow::onMsgCritical);
    connect(ui->btnMsgQuestion, &QPushButton::clicked, this, &MainWindow::onMsgQuestion);
    connect(ui->btnMsgCustom, &QPushButton::clicked, this, &MainWindow::onMsgCustom);
    connect(ui->btnMsgAbout, &QPushButton::clicked, this, &MainWindow::onMsgAbout);

    // 清空日志按钮
    connect(ui->btnClearLog, &QPushButton::clicked, ui->textLog, &QPlainTextEdit::clear);
}

void MainWindow::appendLog(const QString &category, const QString &message)
{
    QString timestamp = QDateTime::currentDateTime().toString(QStringLiteral("HH:mm:ss.zzz"));
    QString logLine = QStringLiteral("[%1] [%2] %3").arg(timestamp).arg(category.leftJustified(6, ' ')).arg(message);
    ui->textLog->appendPlainText(logLine);
}

// 1. 技能施放
void MainWindow::onNormalAttack()
{
    appendLog(QStringLiteral("战斗"), QStringLiteral("普攻命中目标！造成 1,280 点物理伤害。"));
}

void MainWindow::onUltimateSkill()
{
    m_remainingCd = 5;
    ui->btnUltimateSkill->setEnabled(false);
    ui->btnUltimateSkill->setText(QStringLiteral("🔥 冷却中 (%1s)").arg(m_remainingCd));
    m_cooldownTimer->start(1000);

    appendLog(QStringLiteral("战斗"), QStringLiteral("释放【终极奥义·天神下凡】！进入 5 秒技能冷却倒计时。"));
}

void MainWindow::onCooldownTick()
{
    --m_remainingCd;
    if (m_remainingCd <= 0) {
        m_cooldownTimer->stop();
        ui->btnUltimateSkill->setEnabled(true);
        ui->btnUltimateSkill->setText(QStringLiteral("🔥 终极奥义 (CD 5s)"));
        appendLog(QStringLiteral("战斗"), QStringLiteral("【终极奥义·天神下凡】冷却完毕，可再次施放！"));
    } else {
        ui->btnUltimateSkill->setText(QStringLiteral("🔥 冷却中 (%1s)").arg(m_remainingCd));
    }
}

void MainWindow::onToggleStance(bool checked)
{
    if (checked) {
        ui->btnToggleStance->setText(QStringLiteral("⚔️ 狂暴姿态 (攻击+30%, 防御-15%)"));
        appendLog(QStringLiteral("姿态"), QStringLiteral("切换为狂暴姿态，攻击大幅提升！"));
    } else {
        ui->btnToggleStance->setText(QStringLiteral("🛡️ 防御战姿 (防御+30%, 攻击-10%)"));
        appendLog(QStringLiteral("姿态"), QStringLiteral("切换为防御姿态，格挡与减伤大幅提升！"));
    }
}

void MainWindow::onComboActionTriggered(QAction *action)
{
    if (action) {
        appendLog(QStringLiteral("连招"), QStringLiteral("施展组合连击：%1").arg(action->text()));
    }
}

// 2. 工具栏
void MainWindow::onDungeonActionTriggered(QAction *action)
{
    if (action) {
        appendLog(QStringLiteral("副本"), QStringLiteral("已选择目标副本模式：%1").arg(action->text()));
    }
}

void MainWindow::onRadarActionTriggered(QAction *action)
{
    if (action) {
        appendLog(QStringLiteral("雷达"), QStringLiteral("执行全图雷达探测：%1").arg(action->text()));
    }
}

void MainWindow::onPrevPage()
{
    if (m_currentChapter > 1) {
        --m_currentChapter;
        appendLog(QStringLiteral("关卡"), QStringLiteral("切换到第 %1 幕大地图").arg(m_currentChapter));
    }
}

void MainWindow::onNextPage()
{
    ++m_currentChapter;
    appendLog(QStringLiteral("关卡"), QStringLiteral("切换到第 %1 幕大地图").arg(m_currentChapter));
}

void MainWindow::onAutoRaiseClicked()
{
    appendLog(QStringLiteral("操作"), QStringLiteral("点击了 AutoRaise 无边框悬浮凸起按钮。"));
}

// 3. 阵营与画质
void MainWindow::onCampChanged(int id)
{
    QString campName;
    if (id == 1) campName = QStringLiteral("🦁 荣耀联盟");
    else if (id == 2) campName = QStringLiteral("🐺 钢铁部落");
    else campName = QStringLiteral("🕊️ 中立游侠");

    appendLog(QStringLiteral("阵营"), QStringLiteral("玩家阵营切换为：%1 (ID: %2)").arg(campName).arg(id));
}

void MainWindow::onQualityChanged(int id)
{
    QString qName;
    if (id == 10) qName = QStringLiteral("4K 极致画质 (60FPS)");
    else if (id == 20) qName = QStringLiteral("高清均衡模式 (120FPS)");
    else qName = QStringLiteral("省电流畅模式 (30FPS)");

    appendLog(QStringLiteral("画质"), QStringLiteral("引擎渲染画质已应用：%1").arg(qName));
}

// 4. 勾选与三态拾取
void MainWindow::onAutoBattleToggled(bool checked)
{
    appendLog(QStringLiteral("挂机辅助"), QStringLiteral("自动寻路与战斗：%1").arg(checked ? QStringLiteral("已开启") : QStringLiteral("已关闭")));
}

void MainWindow::onLootAllStateChanged(int state)
{
    // 阻断子项信号避免递归触发
    ui->chkLootGold->blockSignals(true);
    ui->chkLootEquip->blockSignals(true);
    ui->chkLootMaterial->blockSignals(true);

    if (state == Qt::Checked) {
        ui->chkLootGold->setChecked(true);
        ui->chkLootEquip->setChecked(true);
        ui->chkLootMaterial->setChecked(true);
        appendLog(QStringLiteral("拾取"), QStringLiteral("全选拾取：金币、史诗装备、强化晶石全部自动拾取。"));
    } else if (state == Qt::Unchecked) {
        ui->chkLootGold->setChecked(false);
        ui->chkLootEquip->setChecked(false);
        ui->chkLootMaterial->setChecked(false);
        appendLog(QStringLiteral("拾取"), QStringLiteral("取消拾取：关闭所有自动拾取项。"));
    }

    ui->chkLootGold->blockSignals(false);
    ui->chkLootEquip->blockSignals(false);
    ui->chkLootMaterial->blockSignals(false);
}

void MainWindow::onSubLootChanged()
{
    updateLootAllState();
}

void MainWindow::updateLootAllState()
{
    int checkedCount = 0;
    if (ui->chkLootGold->isChecked()) ++checkedCount;
    if (ui->chkLootEquip->isChecked()) ++checkedCount;
    if (ui->chkLootMaterial->isChecked()) ++checkedCount;

    ui->chkLootAll->blockSignals(true);
    if (checkedCount == 3) {
        ui->chkLootAll->setCheckState(Qt::Checked);
    } else if (checkedCount == 0) {
        ui->chkLootAll->setCheckState(Qt::Unchecked);
    } else {
        ui->chkLootAll->setCheckState(Qt::PartiallyChecked);
    }
    ui->chkLootAll->blockSignals(false);

    appendLog(QStringLiteral("拾取"), QStringLiteral("子拾取项变更：当前拾取 (%1/3 项)").arg(checkedCount));
}

// 5. 消息弹窗全家桶
void MainWindow::onMsgInfo()
{
    QMessageBox::information(this, QStringLiteral("系统停服维护公告"),
                             QStringLiteral("尊敬的玩家：\n游戏将于明日凌晨 02:00 进行例行停服维护更新，预计维护 4 小时，请提前做好下线准备！"));
    appendLog(QStringLiteral("弹窗"), QStringLiteral("展示了 QMessageBox::information 信息公告。"));
}

void MainWindow::onMsgWarning()
{
    QMessageBox::warning(this, QStringLiteral("体力不足警告"),
                         QStringLiteral("当前角色体力值不足 10 点，无法进入史诗深渊地下城！请使用【强效体力药剂】或等待自然恢复。"));
    appendLog(QStringLiteral("弹窗"), QStringLiteral("展示了 QMessageBox::warning 警告弹窗。"));
}

void MainWindow::onMsgCritical()
{
    QMessageBox::critical(this, QStringLiteral("服务器网络异常"),
                          QStringLiteral("错误代码 [ERR_SOCKET_TIMEOUT_10060]：\n与艾泽拉斯游戏网关服务器连接断开，请检查本地网络连接后重试！"));
    appendLog(QStringLiteral("弹窗"), QStringLiteral("展示了 QMessageBox::critical 严重错误弹窗。"));
}

void MainWindow::onMsgQuestion()
{
    auto reply = QMessageBox::question(this, QStringLiteral("分解高阶神装确认"),
                                       QStringLiteral("您正准备分解【SSR·霜之哀伤 (+15)】！\n分解后神剑将永久损毁并转化为 5,000 强化结晶，此操作不可逆，是否继续？"),
                                       QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        appendLog(QStringLiteral("弹窗"), QStringLiteral("玩家确认分解神器！获得 5,000 强化结晶。"));
    } else {
        appendLog(QStringLiteral("弹窗"), QStringLiteral("玩家取消了分解神装操作。"));
    }
}

void MainWindow::onMsgCustom()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(QStringLiteral("角色战败复活选择"));
    msgBox.setText(QStringLiteral("勇者，您已被恶魔领主击败！请选择复活方式："));
    msgBox.setIcon(QMessageBox::Question);

    QPushButton *btnReviveCoin = msgBox.addButton(QStringLiteral("💎 原地满血复活 (消耗复活币 x1)"), QMessageBox::ActionRole);
    QPushButton *btnTown = msgBox.addButton(QStringLiteral("🏰 回城主干道复活 (虚弱 5 分钟)"), QMessageBox::ActionRole);
    QPushButton *btnExit = msgBox.addButton(QStringLiteral("🚪 放弃挑战并退出副本"), QMessageBox::RejectRole);

    msgBox.exec();

    if (msgBox.clickedButton() == btnReviveCoin) {
        appendLog(QStringLiteral("弹窗"), QStringLiteral("选择：消耗复活币原地满血复活！继续战斗！"));
    } else if (msgBox.clickedButton() == btnTown) {
        appendLog(QStringLiteral("弹窗"), QStringLiteral("选择：回城复活，附带虚弱状态。"));
    } else if (msgBox.clickedButton() == btnExit) {
        appendLog(QStringLiteral("弹窗"), QStringLiteral("选择：放弃副本挑战，传送出地下城。"));
    }
}

void MainWindow::onMsgAbout()
{
    QMessageBox::about(this, QStringLiteral("关于游戏客户端"),
                       QStringLiteral("<h3>艾泽拉斯传说 (Legend of Azeroth)</h3>"
                                      "<p>版本号：v2.5.0 Release</p>"
                                      "<p>引擎架构：Qt 5.15.2 / C++17 Desktop Client</p>"
                                      "<p>版权所有 © 2026 StudyQt Team. All Rights Reserved.</p>"));
    appendLog(QStringLiteral("弹窗"), QStringLiteral("展示了 QMessageBox::about 游戏客户端版本信息。"));
}
