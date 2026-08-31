#include "main_window.h"
#include "ui_main_window.h"

#include <QDateTime>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QStringListModel>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::MainWindow>())
{
    ui->setupUi(this);

    initValidatorsAndCompleter();
    initComboBoxData();
    initRichTextDefault();

    // ========================================================================
    // Tab 1: QLineEdit 信号槽
    // ========================================================================
    connect(ui->editAccount, &QLineEdit::textChanged, this, &MainWindow::onAccountChanged);
    connect(ui->editCdkMask, &QLineEdit::textChanged, this, &MainWindow::onCdkChanged);
    connect(ui->editIpMask, &QLineEdit::textChanged, this, &MainWindow::onIpChanged);
    connect(ui->editNickname, &QLineEdit::textChanged, this, &MainWindow::onNicknameChanged);
    connect(ui->editSearchItem, &QLineEdit::textChanged, this, [this](const QString &t) {
        appendLog(QStringLiteral("搜索补全"), QStringLiteral("图鉴搜索词输入：%1").arg(t));
    });

    // ========================================================================
    // Tab 2: SpinBox & ComboBox 信号槽
    // ========================================================================
    connect(ui->spinLevel, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onLevelChanged);
    connect(ui->spinGold, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onGoldChanged);
    connect(ui->spinCritRate, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::onCritRateChanged);
    connect(ui->spinDiscount, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::onDiscountChanged);
    connect(ui->comboRarity, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onRarityChanged);
    connect(ui->fontComboDialogue, &QFontComboBox::currentFontChanged, this, &MainWindow::onFontChanged);
    connect(ui->keySeqSkill, &QKeySequenceEdit::keySequenceChanged, this, &MainWindow::onKeySequenceChanged);
    connect(ui->dateTimeEvent, &QDateTimeEdit::dateTimeChanged, this, &MainWindow::onDateTimeChanged);

    // ========================================================================
    // Tab 3: QTextEdit & QPlainTextEdit 信号槽
    // ========================================================================
    connect(ui->btnBold, &QPushButton::clicked, this, &MainWindow::onBoldClicked);
    connect(ui->btnItalic, &QPushButton::clicked, this, &MainWindow::onItalicClicked);
    connect(ui->btnUnderline, &QPushButton::clicked, this, &MainWindow::onUnderlineClicked);
    connect(ui->btnColorRed, &QPushButton::clicked, this, &MainWindow::onColorRedClicked);
    connect(ui->btnColorGold, &QPushButton::clicked, this, &MainWindow::onColorGoldClicked);
    connect(ui->btnExecuteGm, &QPushButton::clicked, this, &MainWindow::onExecuteGmScript);

    // ========================================================================
    // Tab 4: QSlider & QDial 信号槽
    // ========================================================================
    connect(ui->sliderMaster, &QSlider::valueChanged, this, &MainWindow::onMasterVolumeChanged);
    connect(ui->sliderBgm, &QSlider::valueChanged, this, &MainWindow::onBgmVolumeChanged);
    connect(ui->dialCamera, &QDial::valueChanged, this, &MainWindow::onDialCameraAngleChanged);

    // 清空日志按钮
    connect(ui->btnClearLog, &QPushButton::clicked, ui->textLogRight, &QPlainTextEdit::clear);

    appendLog(QStringLiteral("系统"), QStringLiteral("游戏编辑器控制中心就绪，所有输入组件已初始化。"));
}

MainWindow::~MainWindow() = default;

void MainWindow::initValidatorsAndCompleter()
{
    // 1. 角色昵称正则校验器 (允许 2~8 位中文字符、英文字母或数字)
    QRegularExpression rx(QStringLiteral("^[a-zA-Z0-9\\x{4e00}-\\x{9fa5}]{2,8}$"));
    auto *nicknameValidator = new QRegularExpressionValidator(rx, this);
    ui->editNickname->setValidator(nicknameValidator);

    // 2. 神装图鉴搜索动态自动补全 (QCompleter)
    const QStringList weaponDb = {
        QStringLiteral("灰烬使者 (Ashbringer)"),
        QStringLiteral("霜之哀伤 (Frostmourne)"),
        QStringLiteral("誓约胜利之剑 (Excalibur)"),
        QStringLiteral("雷霆之怒·逐风者的祝福之剑"),
        QStringLiteral("埃辛诺斯战刃 (双刃)"),
        QStringLiteral("巨龙之怒·泰蕾苟萨的寄魂杖"),
        QStringLiteral("炽热龙晶祝福指环"),
        QStringLiteral("永恒王权守护胸甲"),
        QStringLiteral("远古泰坦之握护手"),
        QStringLiteral("虚空之翼飞行披风")
    };

    m_itemCompleter = new QCompleter(weaponDb, this);
    m_itemCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    m_itemCompleter->setFilterMode(Qt::MatchContains); // 包含匹配
    ui->editSearchItem->setCompleter(m_itemCompleter);
}

void MainWindow::initComboBoxData()
{
    // 下拉框绑定数据 (带品质颜色与自定义 UserData)
    ui->comboRarity->addItem(QStringLiteral("🟢 优秀 (Common)"), QStringLiteral("GREEN"));
    ui->comboRarity->addItem(QStringLiteral("🔵 精良 (Rare)"), QStringLiteral("BLUE"));
    ui->comboRarity->addItem(QStringLiteral("🟣 史诗 (Epic)"), QStringLiteral("PURPLE"));
    ui->comboRarity->addItem(QStringLiteral("🟡 传说 (Legendary)"), QStringLiteral("GOLD"));
    ui->comboRarity->addItem(QStringLiteral("🔴 不朽 (Immortal)"), QStringLiteral("RED"));
    ui->comboRarity->setCurrentIndex(3); // 默认选传说

    // 快捷键初始绑定
    ui->keySeqSkill->setKeySequence(QKeySequence(Qt::Key_R | Qt::AltModifier));

    // 活动日期时间初始值
    ui->dateTimeEvent->setDateTime(QDateTime::currentDateTime().addDays(7));
}

void MainWindow::initRichTextDefault()
{
    ui->textEditAnnouncement->setHtml(
        QStringLiteral(
            "<h2 style='color:#e67e22;'>🔥 全服限时 Boss【灭世巨龙】狂欢降临！</h2>"
            "<p>活动期间，全服玩家挑战副本掉落概率 <span style='color:#e74c3c; font-weight:bold;'>提升 300%</span>！</p>"
            "<p>累计击杀首领可直接获取 <span style='color:#f1c40f; font-weight:bold;'>【传说·灰烬使者】</span> 武器图纸！</p>"
        )
    );
}

void MainWindow::appendLog(const QString &category, const QString &message)
{
    QString timeStr = QDateTime::currentDateTime().toString(QStringLiteral("HH:mm:ss.zzz"));
    QString line = QStringLiteral("[%1] [%2] %3").arg(timeStr).arg(category.leftJustified(6, ' ')).arg(message);
    ui->textLogRight->appendPlainText(line);
}

// ============================================================================
// Tab 1: QLineEdit
// ============================================================================
void MainWindow::onAccountChanged(const QString &text)
{
    appendLog(QStringLiteral("账号"), QStringLiteral("玩家账号变更：%1").arg(text));
}

void MainWindow::onCdkChanged(const QString &text)
{
    appendLog(QStringLiteral("CDK"), QStringLiteral("掩码格式 CDK：%1").arg(text));
}

void MainWindow::onIpChanged(const QString &text)
{
    appendLog(QStringLiteral("网关IP"), QStringLiteral("服务器 IP：%1").arg(text));
}

void MainWindow::onNicknameChanged(const QString &text)
{
    bool isValid = ui->editNickname->hasAcceptableInput();
    appendLog(QStringLiteral("昵称校验"), QStringLiteral("角色名 [%1] -> 正则合法性: %2").arg(text).arg(isValid ? QStringLiteral("通过") : QStringLiteral("未通过(2~8位)")));
}

// ============================================================================
// Tab 2: SpinBox & ComboBox
// ============================================================================
void MainWindow::onLevelChanged(int val)
{
    appendLog(QStringLiteral("等级微调"), QStringLiteral("英雄等级调整为: Lv.%1").arg(val));
}

void MainWindow::onGoldChanged(int val)
{
    appendLog(QStringLiteral("金币微调"), QStringLiteral("充值金币数量: %1 金币").arg(val));
}

void MainWindow::onCritRateChanged(double val)
{
    appendLog(QStringLiteral("暴击倍率"), QStringLiteral("暴击伤害倍率: %1x").arg(val, 0, 'f', 2));
}

void MainWindow::onDiscountChanged(double val)
{
    appendLog(QStringLiteral("特惠折扣"), QStringLiteral("商城特惠折扣: %1 折").arg(val, 0, 'f', 2));
}

void MainWindow::onRarityChanged(int index)
{
    QString text = ui->comboRarity->itemText(index);
    QVariant data = ui->comboRarity->itemData(index);
    appendLog(QStringLiteral("品质下拉"), QStringLiteral("选择品质: %1 (代码: %2)").arg(text).arg(data.toString()));
}

void MainWindow::onFontChanged(const QFont &font)
{
    appendLog(QStringLiteral("对话字体"), QStringLiteral("字体切换为: %1 (字号: %2)").arg(font.family()).arg(font.pointSize()));
}

void MainWindow::onKeySequenceChanged(const QKeySequence &keySeq)
{
    appendLog(QStringLiteral("按键映射"), QStringLiteral("技能快捷键重映射为: %1").arg(keySeq.toString()));
}

void MainWindow::onDateTimeChanged(const QDateTime &dt)
{
    appendLog(QStringLiteral("活动时间"), QStringLiteral("活动截止时间调整为: %1").arg(dt.toString(QStringLiteral("yyyy-MM-dd HH:mm:ss"))));
}

// ============================================================================
// Tab 3: QTextEdit & QPlainTextEdit
// ============================================================================
void MainWindow::onBoldClicked()
{
    QTextCursor cursor = ui->textEditAnnouncement->textCursor();
    QTextCharFormat fmt = cursor.charFormat();
    fmt.setFontWeight(fmt.fontWeight() == QFont::Bold ? QFont::Normal : QFont::Bold);
    cursor.mergeCharFormat(fmt);
    appendLog(QStringLiteral("富文本"), QStringLiteral("切换选中文字加粗状态"));
}

void MainWindow::onItalicClicked()
{
    QTextCursor cursor = ui->textEditAnnouncement->textCursor();
    QTextCharFormat fmt = cursor.charFormat();
    fmt.setFontItalic(!fmt.fontItalic());
    cursor.mergeCharFormat(fmt);
    appendLog(QStringLiteral("富文本"), QStringLiteral("切换选中文字斜体状态"));
}

void MainWindow::onUnderlineClicked()
{
    QTextCursor cursor = ui->textEditAnnouncement->textCursor();
    QTextCharFormat fmt = cursor.charFormat();
    fmt.setFontUnderline(!fmt.fontUnderline());
    cursor.mergeCharFormat(fmt);
    appendLog(QStringLiteral("富文本"), QStringLiteral("切换选中文字下划线状态"));
}

void MainWindow::onColorRedClicked()
{
    QTextCursor cursor = ui->textEditAnnouncement->textCursor();
    QTextCharFormat fmt;
    fmt.setForeground(QColor(QStringLiteral("#e74c3c")));
    cursor.mergeCharFormat(fmt);
    appendLog(QStringLiteral("富文本"), QStringLiteral("应用红色警告高亮"));
}

void MainWindow::onColorGoldClicked()
{
    QTextCursor cursor = ui->textEditAnnouncement->textCursor();
    QTextCharFormat fmt;
    fmt.setForeground(QColor(QStringLiteral("#f1c40f")));
    cursor.mergeCharFormat(fmt);
    appendLog(QStringLiteral("富文本"), QStringLiteral("应用金色传说高亮"));
}

void MainWindow::onExecuteGmScript()
{
    QString script = ui->plainTextGmScript->toPlainText();
    QStringList lines = script.split('\n', QString::SkipEmptyParts);

    int executed = 0;
    for (const QString &line : lines) {
        QString trimmed = line.trimmed();
        if (trimmed.isEmpty() || trimmed.startsWith('#')) {
            continue;
        }
        appendLog(QStringLiteral("GM指令"), QStringLiteral("执行 -> %1 (状态: SUCCESS)").arg(trimmed));
        ++executed;
    }

    QMessageBox::information(this, QStringLiteral("GM 脚本批处理完成"),
                             QStringLiteral("成功执行 %1 条开发者调试指令！").arg(executed));
}

// ============================================================================
// Tab 4: QSlider & QDial
// ============================================================================
void MainWindow::onMasterVolumeChanged(int val)
{
    ui->labelMasterVal->setText(QStringLiteral("%1 %").arg(val));
    appendLog(QStringLiteral("音频"), QStringLiteral("主音量 (Master) 调整为: %1%").arg(val));
}

void MainWindow::onBgmVolumeChanged(int val)
{
    ui->labelBgmVal->setText(QStringLiteral("%1 %").arg(val));
    appendLog(QStringLiteral("音频"), QStringLiteral("背景音乐 (BGM) 调整为: %1%").arg(val));
}

void MainWindow::onDialCameraAngleChanged(int val)
{
    QString direction;
    if (val >= 338 || val < 23) direction = QStringLiteral("正北 (N)");
    else if (val >= 23 && val < 68) direction = QStringLiteral("东北 (NE)");
    else if (val >= 68 && val < 113) direction = QStringLiteral("正东 (E)");
    else if (val >= 113 && val < 158) direction = QStringLiteral("东南 (SE)");
    else if (val >= 158 && val < 203) direction = QStringLiteral("正南 (S)");
    else if (val >= 203 && val < 248) direction = QStringLiteral("西南 (SW)");
    else if (val >= 248 && val < 293) direction = QStringLiteral("正西 (W)");
    else direction = QStringLiteral("西北 (NW)");

    ui->labelCameraAngle->setText(QStringLiteral("当前主视角朝向：%1° (%2)").arg(val).arg(direction));
    appendLog(QStringLiteral("视角罗盘"), QStringLiteral("3D 视角旋转: %1° (%2)").arg(val).arg(direction));
}
