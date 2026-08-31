#include "main_window.h"
#include "ui_main_window.h"

#include <QPainter>
#include <QDateTime>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::MainWindow>())
{
    ui->setupUi(this);

    initLabelsAndMovie();
    initProgressBars();
    initLcdNumbers();

    connect(ui->btnClearLog, &QPushButton::clicked, ui->textLog, &QPlainTextEdit::clear);
    appendLog(QStringLiteral("系统"), QStringLiteral("展示控件演示中心已就绪。"));
}

MainWindow::~MainWindow() = default;

void MainWindow::appendLog(const QString &category, const QString &message)
{
    QString timeStr = QDateTime::currentDateTime().toString(QStringLiteral("HH:mm:ss.zzz"));
    QString line = QStringLiteral("[%1] [%2] %3").arg(timeStr).arg(category.leftJustified(8, ' ')).arg(message);
    ui->textLog->appendPlainText(line);
}

// ============================================================================
// 1. QLabel & QMovie (标签、图文与动图)
// 
// 【核心知识点与 API 说明】：
//  - setTextFormat(Qt::RichText): 开启富文本解析模式。
//  - setOpenExternalLinks(bool):
//    * true: 点击超链接直接调用系统默认浏览器打开 URL。
//    * false: 触发 linkActivated(QString) 信号，由开发者自行拦截处理（如自定义协议 app://）。
//  - setPixmap(QPixmap): 设置静态位图展示。
//  - setScaledContents(bool): 是否自动拉伸图片填满 QLabel 几何区域。
//  - QMovie: 专门用于加载并循环播放 GIF 动图，提供 start()、stop()、setPaused()、setSpeed() 等控制。
// 
// 【注意事项】：
//  1. 若需要高质量缩放位图，推荐在代码中使用 pixmap.scaled(..., Qt::KeepAspectRatio, Qt::SmoothTransformation)
//     主动缩放后再 setPixmap，避免 setScaledContents(true) 引起的图像模糊失真。
// ============================================================================
void MainWindow::initLabelsAndMovie()
{
    // 1.1 使用 QPainter 动态绘制一张示例图片位图
    QPixmap pix(220, 160);
    pix.fill(Qt::transparent);
    QPainter painter(&pix);
    painter.setRenderHint(QPainter::Antialiasing);

    QLinearGradient grad(0, 0, 220, 160);
    grad.setColorAt(0.0, QColor(52, 73, 94));
    grad.setColorAt(1.0, QColor(41, 128, 185));
    painter.setBrush(grad);
    painter.setPen(QPen(QColor(255, 255, 255), 2));
    painter.drawRoundedRect(5, 5, 210, 150, 8, 8);

    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(11);
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(QRect(0, 45, 220, 30), Qt::AlignCenter, QStringLiteral("QPixmap 绘图渲染"));
    font.setPointSize(9);
    font.setBold(false);
    painter.setFont(font);
    painter.drawText(QRect(0, 80, 220, 30), Qt::AlignCenter, QStringLiteral("220 x 160 矢量渐变矩形"));

    ui->labelPixmap->setPixmap(pix);

    // 1.2 超链接信号拦截
    connect(ui->labelRichText, &QLabel::linkActivated, this, &MainWindow::onRichTextLinkActivated);

    // 1.3 QMovie 帧动画模拟计时器
    m_movieTimer = new QTimer(this);
    connect(m_movieTimer, &QTimer::timeout, this, &MainWindow::onMovieFrameTick);
    m_movieTimer->start(350); // 350ms 切换一帧

    connect(ui->btnMoviePlay, &QPushButton::clicked, this, &MainWindow::onMoviePlay);
    connect(ui->btnMoviePause, &QPushButton::clicked, this, &MainWindow::onMoviePause);
    connect(ui->btnMovieSpeed, &QPushButton::clicked, this, &MainWindow::onMovieSpeedToggle);
}

void MainWindow::onRichTextLinkActivated(const QString &link)
{
    if (link.startsWith(QStringLiteral("app://"))) {
        appendLog(QStringLiteral("QLabel"), QStringLiteral("拦截到内部自定义协议链接: %1").arg(link));
        QMessageBox::information(this, QStringLiteral("内部协议响应"),
                                 QStringLiteral("成功触发并拦截内部协议 [%1]！\n可在此处调起对应的业务窗口。").arg(link));
    } else if (link.startsWith(QStringLiteral("http"))) {
        appendLog(QStringLiteral("QLabel"), QStringLiteral("唤醒系统默认浏览器打开: %1").arg(link));
        QDesktopServices::openUrl(QUrl(link));
    }
}

void MainWindow::onMovieFrameTick()
{
    const QVector<QString> frames = {
        QStringLiteral("● 帧 1: [ 正在初始化引擎核心... ]"),
        QStringLiteral("● 帧 2: [ 正在加载纹理与着色器... ]"),
        QStringLiteral("● 帧 3: [ 正在同步网络数据报文... ]"),
        QStringLiteral("● 帧 4: [ 准备就绪，进入主循环! ]")
    };
    const QVector<QString> colors = {
        QStringLiteral("#1abc9c"), QStringLiteral("#3498db"), QStringLiteral("#e67e22"), QStringLiteral("#2ecc71")
    };

    m_movieFrame = (m_movieFrame + 1) % frames.size();
    ui->labelMovieDisplay->setStyleSheet(
        QStringLiteral("background-color: #2c3e50; border: 1px solid %1; border-radius: 4px; color: %1; font-size: 13px; font-weight: bold; qproperty-alignment: AlignCenter;")
        .arg(colors[m_movieFrame])
    );
    ui->labelMovieDisplay->setText(frames[m_movieFrame]);
}

void MainWindow::onMoviePlay()
{
    m_movieTimer->start();
    appendLog(QStringLiteral("QMovie"), QStringLiteral("调用 start() 开始播放动画"));
}

void MainWindow::onMoviePause()
{
    m_movieTimer->stop();
    appendLog(QStringLiteral("QMovie"), QStringLiteral("调用 stop() 暂停播放动画"));
}

void MainWindow::onMovieSpeedToggle()
{
    m_movieSpeed2x = !m_movieSpeed2x;
    if (m_movieSpeed2x) {
        m_movieTimer->setInterval(175);
        ui->btnMovieSpeed->setText(QStringLiteral("⚡ 1.0x 常速"));
        appendLog(QStringLiteral("QMovie"), QStringLiteral("调用 setSpeed(200) 加速为 2.0x 倍速"));
    } else {
        m_movieTimer->setInterval(350);
        ui->btnMovieSpeed->setText(QStringLiteral("⚡ 2.0x 倍速"));
        appendLog(QStringLiteral("QMovie"), QStringLiteral("调用 setSpeed(100) 恢复为 1.0x 常速"));
    }
}

// ============================================================================
// 2. QProgressBar (进度条)
// 
// 【核心知识点与 API 说明】：
//  - setRange(min, max): 设置进度范围（常用 0 ~ 100）。
//  - setValue(int): 设置当前进度数值。
//  - setFormat(QString): 自定义文本显示格式：
//    * %p: 百分比（Percentage）
//    * %v: 当前值（Value）
//    * %m: 最大值（Total / Maximum）
//  - setRange(0, 0): 进入“不确定跑马灯模式（Indeterminate / Busy）”，用于耗时未知任务。
//  - setOrientation(Qt::Vertical): 设置为垂直方向进度条。
//  - setInvertedAppearance(bool): 设置反向填充（如从上往下增长）。
// ============================================================================
void MainWindow::initProgressBars()
{
    m_progressTimer = new QTimer(this);
    connect(m_progressTimer, &QTimer::timeout, this, &MainWindow::onProgressTimerTick);

    connect(ui->btnStartProgress, &QPushButton::clicked, this, &MainWindow::onStartProgress);
    connect(ui->btnResetProgress, &QPushButton::clicked, this, &MainWindow::onResetProgress);
    connect(ui->btnVerticalPlus, &QPushButton::clicked, this, &MainWindow::onVerticalPlus);
    connect(ui->btnVerticalMinus, &QPushButton::clicked, this, &MainWindow::onVerticalMinus);
}

void MainWindow::onStartProgress()
{
    m_progressValue = 0;
    ui->progressDefinite->setValue(0);
    m_progressTimer->start(60); // 60ms 递增一次
    appendLog(QStringLiteral("QProgressBar"), QStringLiteral("启动确定模式进度递增 (0 -> 100)"));
}

void MainWindow::onResetProgress()
{
    m_progressTimer->stop();
    m_progressValue = 0;
    ui->progressDefinite->setValue(0);
    appendLog(QStringLiteral("QProgressBar"), QStringLiteral("重置确定模式进度为 0"));
}

void MainWindow::onProgressTimerTick()
{
    m_progressValue += 2;
    if (m_progressValue >= 100) {
        m_progressValue = 100;
        ui->progressDefinite->setValue(100);
        m_progressTimer->stop();
        appendLog(QStringLiteral("QProgressBar"), QStringLiteral("进度达到 100%，任务完成！"));
    } else {
        ui->progressDefinite->setValue(m_progressValue);
    }
}

void MainWindow::onVerticalPlus()
{
    int val1 = qMin(100, ui->progressVerticalNormal->value() + 10);
    int val2 = qMin(100, ui->progressVerticalInverted->value() + 10);
    ui->progressVerticalNormal->setValue(val1);
    ui->progressVerticalInverted->setValue(val2);
    appendLog(QStringLiteral("QProgressBar"), QStringLiteral("垂直进度条数值 +10% -> 当前: %1%").arg(val1));
}

void MainWindow::onVerticalMinus()
{
    int val1 = qMax(0, ui->progressVerticalNormal->value() - 10);
    int val2 = qMax(0, ui->progressVerticalInverted->value() - 10);
    ui->progressVerticalNormal->setValue(val1);
    ui->progressVerticalInverted->setValue(val2);
    appendLog(QStringLiteral("QProgressBar"), QStringLiteral("垂直进度条数值 -10% -> 当前: %1%").arg(val1));
}

// ============================================================================
// 3. QLCDNumber (数字液晶显示屏)
// 
// 【核心知识点与 API 说明】：
//  - display(int / double / QString): 输出数字或时间字符串（如 "12:34:56"）。
//  - setDigitCount(int): 设置最多可显示的字符/数字位数。
//  - setSegmentStyle(SegmentStyle):
//    * Outline: 浅色底色与细线描边轮廓
//    * Filled: 实心液晶线段
//    * Flat: 扁平无边框风格
//  - setDecMode() / setHexMode() / setBinMode() / setOctMode(): 快速切换十/十六/二/八进制模式。
// ============================================================================
void MainWindow::initLcdNumbers()
{
    m_clockTimer = new QTimer(this);
    connect(m_clockTimer, &QTimer::timeout, this, &MainWindow::onClockTimerTick);
    m_clockTimer->start(1000);
    onClockTimerTick();

    ui->lcdCounter->display(m_counterValue);

    connect(ui->btnStyleFilled, &QPushButton::clicked, this, &MainWindow::onStyleFilled);
    connect(ui->btnStyleOutline, &QPushButton::clicked, this, &MainWindow::onStyleOutline);
    connect(ui->btnStyleFlat, &QPushButton::clicked, this, &MainWindow::onStyleFlat);

    connect(ui->btnModeDec, &QPushButton::clicked, this, &MainWindow::onModeDec);
    connect(ui->btnModeHex, &QPushButton::clicked, this, &MainWindow::onModeHex);
    connect(ui->btnModeBin, &QPushButton::clicked, this, &MainWindow::onModeBin);

    connect(ui->btnCountAdd1, &QPushButton::clicked, this, &MainWindow::onCountAdd1);
    connect(ui->btnCountAdd10, &QPushButton::clicked, this, &MainWindow::onCountAdd10);
    connect(ui->btnCountReset, &QPushButton::clicked, this, &MainWindow::onCountReset);
}

void MainWindow::onClockTimerTick()
{
    QString timeText = QDateTime::currentDateTime().toString(QStringLiteral("HH:mm:ss"));
    ui->lcdClock->display(timeText);
}

void MainWindow::onStyleFilled()
{
    ui->lcdClock->setSegmentStyle(QLCDNumber::Filled);
    appendLog(QStringLiteral("QLCDNumber"), QStringLiteral("切换风格为 QLCDNumber::Filled (实心)"));
}

void MainWindow::onStyleOutline()
{
    ui->lcdClock->setSegmentStyle(QLCDNumber::Outline);
    appendLog(QStringLiteral("QLCDNumber"), QStringLiteral("切换风格为 QLCDNumber::Outline (空心)"));
}

void MainWindow::onStyleFlat()
{
    ui->lcdClock->setSegmentStyle(QLCDNumber::Flat);
    appendLog(QStringLiteral("QLCDNumber"), QStringLiteral("切换风格为 QLCDNumber::Flat (扁平)"));
}

void MainWindow::onModeDec()
{
    ui->lcdCounter->setDecMode();
    appendLog(QStringLiteral("QLCDNumber"), QStringLiteral("切换为十进制 (DEC) 显示模式"));
}

void MainWindow::onModeHex()
{
    ui->lcdCounter->setHexMode();
    appendLog(QStringLiteral("QLCDNumber"), QStringLiteral("切换为十六进制 (HEX) 显示模式"));
}

void MainWindow::onModeBin()
{
    ui->lcdCounter->setBinMode();
    appendLog(QStringLiteral("QLCDNumber"), QStringLiteral("切换为二进制 (BIN) 显示模式"));
}

void MainWindow::onCountAdd1()
{
    ++m_counterValue;
    ui->lcdCounter->display(m_counterValue);
    appendLog(QStringLiteral("QLCDNumber"), QStringLiteral("计数 +1 -> %1").arg(m_counterValue));
}

void MainWindow::onCountAdd10()
{
    m_counterValue += 10;
    ui->lcdCounter->display(m_counterValue);
    appendLog(QStringLiteral("QLCDNumber"), QStringLiteral("计数 +10 -> %1").arg(m_counterValue));
}

void MainWindow::onCountReset()
{
    m_counterValue = 0;
    ui->lcdCounter->display(0);
    appendLog(QStringLiteral("QLCDNumber"), QStringLiteral("计数归零 (0)"));
}
