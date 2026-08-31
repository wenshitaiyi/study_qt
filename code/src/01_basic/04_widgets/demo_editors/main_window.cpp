#include "main_window.h"
#include "ui_main_window.h"

#include <QDateTime>
#include <QIntValidator>
#include <QRegularExpressionValidator>
#include <QStringListModel>
#include <QTextCursor>
#include <QTextCharFormat>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::MainWindow>())
{
    ui->setupUi(this);

    initLineEdits();
    initSpinBoxes();
    initComboBoxesAndMisc();
    initTextEdits();

    connect(ui->btnClearLog, &QPushButton::clicked, ui->textLog, &QPlainTextEdit::clear);
    appendLog(QStringLiteral("系统"), QStringLiteral("输入与编辑器演示中心已就绪。"));
}

MainWindow::~MainWindow() = default;

void MainWindow::appendLog(const QString &category, const QString &message)
{
    QString timeStr = QDateTime::currentDateTime().toString(QStringLiteral("HH:mm:ss.zzz"));
    QString line = QStringLiteral("[%1] [%2] %3").arg(timeStr).arg(category.leftJustified(8, ' ')).arg(message);
    ui->textLog->appendPlainText(line);
}

// ============================================================================
// 1. QLineEdit (单行文本输入框)
// 
// 【核心知识点与 API 说明】：
//  - setEchoMode(EchoMode): 设置回显模式：
//    * Normal: 常规明文（默认）
//    * Password: 密码模式（显示掩码圆点）
//    * PasswordEchoOnEdit: 键入字符时短暂显示明文，光标离开或一段时间后变掩码
//    * NoEcho: 完全不显示任何字符（类似 Linux 终端输入密码）
//  - setClearButtonEnabled(bool): 开启右侧内嵌一键清除“×”小图标
//  - setInputMask(mask): 设置固定格式掩码（如 IP、MAC、序列号，字符占位严格匹配）
//  - setValidator(QValidator*): 设置输入校验器，拦截非法输入字符
//  - setCompleter(QCompleter*): 关联自动补全器，提供下拉候选项建议
// 
// 【注意事项】：
//  1. InputMask 会强制修改输入长度与格式，若用户未完整输入掩码要求位数，text() 可能会包含占位符。
//  2. 推荐使用 QRegularExpressionValidator 代替已弃用的 QRegExpValidator。
// ============================================================================
void MainWindow::initLineEdits()
{
    // 1.1 校验器：限制端口号 1 ~ 65535
    auto *portValidator = new QIntValidator(1, 65535, this);
    ui->editValInt->setValidator(portValidator);

    // 1.2 正则校验器：3~16位字母、数字或下划线
    QRegularExpression rx(QStringLiteral("^[a-zA-Z0-9_]{3,16}$"));
    auto *regexValidator = new QRegularExpressionValidator(rx, this);
    ui->editValRegex->setValidator(regexValidator);

    // 1.3 QCompleter 自动补全
    const QStringList wordList = {
        QStringLiteral("QLabel"), QStringLiteral("QLineEdit"), QStringLiteral("QPushButton"),
        QStringLiteral("QProgressBar"), QStringLiteral("QPlainTextEdit"), QStringLiteral("QTextEdit"),
        QStringLiteral("QVBoxLayout"), QStringLiteral("QHBoxLayout"), QStringLiteral("QGridLayout"),
        QStringLiteral("QFormLayout"), QStringLiteral("QStackedWidget"), QStringLiteral("QComboBox")
    };
    m_completer = new QCompleter(wordList, this);
    m_completer->setCaseSensitivity(Qt::CaseInsensitive); // 忽略大小写
    m_completer->setFilterMode(Qt::MatchContains);        // 包含匹配模式
    ui->editCompleter->setCompleter(m_completer);

    // 信号槽连接
    connect(ui->editEchoNormal, &QLineEdit::textChanged, this, &MainWindow::onEchoNormalChanged);
    connect(ui->editMaskIp, &QLineEdit::textChanged, this, &MainWindow::onMaskIpChanged);
    connect(ui->editMaskSerial, &QLineEdit::textChanged, this, &MainWindow::onMaskSerialChanged);
    connect(ui->editValInt, &QLineEdit::textChanged, this, &MainWindow::onValIntChanged);
    connect(ui->editValRegex, &QLineEdit::textChanged, this, &MainWindow::onValRegexChanged);
}

void MainWindow::onEchoNormalChanged(const QString &text)
{
    appendLog(QStringLiteral("QLineEdit"), QStringLiteral("Normal 文本变更：%1").arg(text));
}

void MainWindow::onMaskIpChanged(const QString &text)
{
    appendLog(QStringLiteral("InputMask"), QStringLiteral("IP 掩码输入：%1").arg(text));
}

void MainWindow::onMaskSerialChanged(const QString &text)
{
    appendLog(QStringLiteral("InputMask"), QStringLiteral("序列号掩码输入：%1").arg(text));
}

void MainWindow::onValIntChanged(const QString &text)
{
    bool acceptable = ui->editValInt->hasAcceptableInput();
    appendLog(QStringLiteral("Validator"), QStringLiteral("端口输入: %1 (合法状态: %2)").arg(text).arg(acceptable ? QStringLiteral("合法") : QStringLiteral("中间/非法")));
}

void MainWindow::onValRegexChanged(const QString &text)
{
    bool acceptable = ui->editValRegex->hasAcceptableInput();
    appendLog(QStringLiteral("Validator"), QStringLiteral("正则输入: %1 (合法状态: %2)").arg(text).arg(acceptable ? QStringLiteral("合法") : QStringLiteral("中间/非法")));
}

// ============================================================================
// 2. QSpinBox & QDoubleSpinBox (数值微调框)
// 
// 【核心知识点与 API 说明】：
//  - setRange(min, max): 快速设定允许输入的上下限范围。
//  - setSingleStep(step): 设定点击上下微调箭头或键盘方向键时的单步递增/递减值。
//  - setPrefix(QString) / setSuffix(QString): 设定数值显示的前缀或后缀（不影响实际 value() 数值）。
//  - setWrapping(bool): 开启循环滚动模式（到达最大值后继续增加会循环回最小值）。
//  - setSpecialValueText(QString): 当数值等于最小值 (minimum) 时展示的特殊占位文本（如 "0 (自动)"）。
//  - setDecimals(int): QDoubleSpinBox 专用，控制浮点数显示的小数位数精度。
// ============================================================================
void MainWindow::initSpinBoxes()
{
    connect(ui->spinLevel, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onSpinLevelChanged);
    connect(ui->spinStep, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onSpinStepChanged);
    connect(ui->doubleSpinPrice, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::onDoubleSpinPriceChanged);
    connect(ui->doubleSpinRate, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::onDoubleSpinRateChanged);
}

void MainWindow::onSpinLevelChanged(int val)
{
    appendLog(QStringLiteral("QSpinBox"), QStringLiteral("等级数值变更: %1 (实际值: %2)").arg(ui->spinLevel->text()).arg(val));
}

void MainWindow::onSpinStepChanged(int val)
{
    appendLog(QStringLiteral("QSpinBox"), QStringLiteral("步长数值变更: %1 ms").arg(val));
}

void MainWindow::onDoubleSpinPriceChanged(double val)
{
    appendLog(QStringLiteral("QDoubleSpinBox"), QStringLiteral("货币金额变更: ¥ %1").arg(val, 0, 'f', 2));
}

void MainWindow::onDoubleSpinRateChanged(double val)
{
    appendLog(QStringLiteral("QDoubleSpinBox"), QStringLiteral("缩放比率变更: %1x").arg(val, 0, 'f', 3));
}

// ============================================================================
// 3. QComboBox, QFontComboBox, QKeySequenceEdit, QDateTimeEdit
// 
// 【核心知识点与 API 说明】：
//  - addItem(text, userData): 添加下拉选项并绑定任意类型的自定义数据（QVariant）。
//  - itemData(index): 获取指定项绑定的自定义数据。
//  - QFontComboBox::currentFontChanged: 监听系统字体变更信号。
//  - QKeySequenceEdit::keySequenceChanged: 捕获用户在键盘上按下的快捷键组合（如 Ctrl+Shift+S）。
//  - setCalendarPopup(true): 允许点击下拉小箭头弹出日历选择视图。
// ============================================================================
void MainWindow::initComboBoxesAndMisc()
{
    // 下拉框填充数据并绑定内部枚举/标识代码
    ui->comboData->addItem(QStringLiteral("开发环境 (Development)"), QStringLiteral("DEV"));
    ui->comboData->addItem(QStringLiteral("测试环境 (Testing)"), QStringLiteral("TEST"));
    ui->comboData->addItem(QStringLiteral("灰度预发 (Staging)"), QStringLiteral("STAGE"));
    ui->comboData->addItem(QStringLiteral("生产环境 (Production)"), QStringLiteral("PROD"));

    // 默认快捷键映射
    ui->keySequenceEdit->setKeySequence(QKeySequence(Qt::CTRL | Qt::Key_S));

    // 默认时间设为当前时间加 3 天
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime().addDays(3));

    connect(ui->comboData, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onComboDataIndexChanged);
    connect(ui->fontCombo, &QFontComboBox::currentFontChanged, this, &MainWindow::onFontChanged);
    connect(ui->keySequenceEdit, &QKeySequenceEdit::keySequenceChanged, this, &MainWindow::onKeySequenceChanged);
    connect(ui->dateTimeEdit, &QDateTimeEdit::dateTimeChanged, this, &MainWindow::onDateTimeChanged);
}

void MainWindow::onComboDataIndexChanged(int index)
{
    QString text = ui->comboData->itemText(index);
    QVariant data = ui->comboData->itemData(index);
    appendLog(QStringLiteral("QComboBox"), QStringLiteral("下拉项选中: %1 (绑定 UserData: %2)").arg(text).arg(data.toString()));
}

void MainWindow::onFontChanged(const QFont &font)
{
    appendLog(QStringLiteral("QFontComboBox"), QStringLiteral("选中字体: %1").arg(font.family()));
}

void MainWindow::onKeySequenceChanged(const QKeySequence &keySeq)
{
    appendLog(QStringLiteral("QKeySequence"), QStringLiteral("捕获快捷键: %1").arg(keySeq.toString()));
}

void MainWindow::onDateTimeChanged(const QDateTime &dateTime)
{
    appendLog(QStringLiteral("QDateTimeEdit"), QStringLiteral("日期时间变更: %1").arg(dateTime.toString(QStringLiteral("yyyy-MM-dd HH:mm:ss"))));
}

// ============================================================================
// 4. QTextEdit & QPlainTextEdit (多行文本与富文本编辑)
// 
// 【核心知识点与 API 说明】：
//  - QTextEdit:
//    * setHtml(html) / toHtml(): 支持富文本排版、超链接、颜色与表格。
//    * textCursor().mergeCharFormat(fmt): 对当前选区或光标所在单词应用样式格式。
//  - QPlainTextEdit:
//    * 针对大容量纯文本（日志控制台、源码编辑）进行了专门的高性能优化。
//    * setMaximumBlockCount(int): 限制最大行数（防止海量日志写入导致内存溢出）。
//    * setReadOnly(true): 设置为只读模式。
// ============================================================================
void MainWindow::initTextEdits()
{
    ui->textEditRich->setHtml(
        QStringLiteral(
            "<p><b>QTextEdit</b> 是一个强大的 <span style='color:#e74c3c;'>富文本</span> 编辑组件。</p>"
            "<p>请使用鼠标选中这段文字，然后点击上方的格式化按钮体验 <i>斜体</i>、<u>下划线</u> 或变色效果！</p>"
        )
    );

    connect(ui->btnBold, &QPushButton::clicked, this, &MainWindow::onFormatBold);
    connect(ui->btnItalic, &QPushButton::clicked, this, &MainWindow::onFormatItalic);
    connect(ui->btnUnderline, &QPushButton::clicked, this, &MainWindow::onFormatUnderline);
    connect(ui->btnColorRed, &QPushButton::clicked, this, &MainWindow::onFormatColorRed);
    connect(ui->btnColorBlue, &QPushButton::clicked, this, &MainWindow::onFormatColorBlue);
}

void MainWindow::onFormatBold()
{
    QTextCursor cursor = ui->textEditRich->textCursor();
    QTextCharFormat fmt = cursor.charFormat();
    fmt.setFontWeight(fmt.fontWeight() == QFont::Bold ? QFont::Normal : QFont::Bold);
    cursor.mergeCharFormat(fmt);
    appendLog(QStringLiteral("QTextEdit"), QStringLiteral("切换选中文字加粗状态"));
}

void MainWindow::onFormatItalic()
{
    QTextCursor cursor = ui->textEditRich->textCursor();
    QTextCharFormat fmt = cursor.charFormat();
    fmt.setFontItalic(!fmt.fontItalic());
    cursor.mergeCharFormat(fmt);
    appendLog(QStringLiteral("QTextEdit"), QStringLiteral("切换选中文字斜体状态"));
}

void MainWindow::onFormatUnderline()
{
    QTextCursor cursor = ui->textEditRich->textCursor();
    QTextCharFormat fmt = cursor.charFormat();
    fmt.setFontUnderline(!fmt.fontUnderline());
    cursor.mergeCharFormat(fmt);
    appendLog(QStringLiteral("QTextEdit"), QStringLiteral("切换选中文字下划线状态"));
}

void MainWindow::onFormatColorRed()
{
    QTextCursor cursor = ui->textEditRich->textCursor();
    QTextCharFormat fmt;
    fmt.setForeground(QColor(QStringLiteral("#e74c3c")));
    cursor.mergeCharFormat(fmt);
    appendLog(QStringLiteral("QTextEdit"), QStringLiteral("设置文字颜色为红色"));
}

void MainWindow::onFormatColorBlue()
{
    QTextCursor cursor = ui->textEditRich->textCursor();
    QTextCharFormat fmt;
    fmt.setForeground(QColor(QStringLiteral("#3498db")));
    cursor.mergeCharFormat(fmt);
    appendLog(QStringLiteral("QTextEdit"), QStringLiteral("设置文字颜色为蓝色"));
}
