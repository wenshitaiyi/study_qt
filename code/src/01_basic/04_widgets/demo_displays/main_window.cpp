#include "main_window.h"
#include "ui_main_window.h"

#include <QPainter>
#include <QDateTime>
#include <QLinearGradient>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::MainWindow>())
{
    ui->setupUi(this);

    initHeroPortrait();

    // ========================================================================
    // Tab 1: QLabel 超链接与动态帧模拟
    // ========================================================================
    connect(ui->labelRichLore, &QLabel::linkActivated, this, &MainWindow::onLinkActivated);

    m_movieFrameTimer = new QTimer(this);
    connect(m_movieFrameTimer, &QTimer::timeout, this, &MainWindow::onMovieFrameTick);
    m_movieFrameTimer->start(300); // 300ms 一帧

    connect(ui->btnMoviePlay, &QPushButton::clicked, this, &MainWindow::onMoviePlay);
    connect(ui->btnMoviePause, &QPushButton::clicked, this, &MainWindow::onMoviePause);
    connect(ui->btnMovieSpeed, &QPushButton::clicked, this, &MainWindow::onMovieSpeed);

    // ========================================================================
    // Tab 2: QProgressBar 信号槽
    // ========================================================================
    m_downloadTimer = new QTimer(this);
    connect(m_downloadTimer, &QTimer::timeout, this, &MainWindow::onDownloadTick);

    connect(ui->btnStartDownload, &QPushButton::clicked, this, &MainWindow::onStartDownload);
    connect(ui->btnResetDownload, &QPushButton::clicked, this, &MainWindow::onResetDownload);
    connect(ui->btnBossDamage, &QPushButton::clicked, this, &MainWindow::onBossDamage);
    connect(ui->btnBossHeal, &QPushButton::clicked, this, &MainWindow::onBossHeal);

    // ========================================================================
    // Tab 3: QLCDNumber 信号槽
    // ========================================================================
    m_countdownTimer = new QTimer(this);
    connect(m_countdownTimer, &QTimer::timeout, this, &MainWindow::onTimerCountdownTick);
    updateLcdTimerDisplay();

    connect(ui->btnStartTimer, &QPushButton::clicked, this, &MainWindow::onStartTimer);
    connect(ui->btnPauseTimer, &QPushButton::clicked, this, &MainWindow::onPauseTimer);
    connect(ui->btnResetTimer, &QPushButton::clicked, this, &MainWindow::onResetTimer);

    connect(ui->btnModeDec, &QPushButton::clicked, this, &MainWindow::onModeDec);
    connect(ui->btnModeHex, &QPushButton::clicked, this, &MainWindow::onModeHex);
    connect(ui->btnModeBin, &QPushButton::clicked, this, &MainWindow::onModeBin);

    connect(ui->btnHitAdd1, &QPushButton::clicked, this, &MainWindow::onHitAdd1);
    connect(ui->btnHitAdd10, &QPushButton::clicked, this, &MainWindow::onHitAdd10);
    connect(ui->btnHitReset, &QPushButton::clicked, this, &MainWindow::onHitReset);

    ui->lcdCombo->display(128);
    ui->lcdScore->display(99850);

    // 清空日志按钮
    connect(ui->btnClearLog, &QPushButton::clicked, ui->textLog, &QPlainTextEdit::clear);

    appendLog(QStringLiteral("系统"), QStringLiteral("展示控件交互中心就绪，支持富文本超链接、动态进度与仿真液晶屏。"));
}

MainWindow::~MainWindow() = default;

void MainWindow::initHeroPortrait()
{
    // 使用 QPainter 动态绘制高质感英雄立绘徽章卡片
    QPixmap pix(220, 180);
    pix.fill(Qt::transparent);

    QPainter painter(&pix);
    painter.setRenderHint(QPainter::Antialiasing);

    // 背景渐变
    QLinearGradient bgGrad(0, 0, 220, 180);
    bgGrad.setColorAt(0.0, QColor(44, 62, 80));
    bgGrad.setColorAt(1.0, QColor(24, 44, 97));
    painter.setBrush(bgGrad);
    painter.setPen(QPen(QColor(241, 196, 15), 3));
    painter.drawRoundedRect(5, 5, 210, 170, 10, 10);

    // 绘制英雄头像文字与战力数值
    painter.setPen(QColor(255, 255, 255));
    QFont font = painter.font();
    font.setPointSize(12);
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(QRect(10, 25, 200, 30), Qt::AlignCenter, QStringLiteral("🛡️ 圣骑士·亚瑟 (SSR)"));

    font.setPointSize(10);
    font.setBold(false);
    painter.setFont(font);
    painter.setPen(QColor(243, 156, 18));
    painter.drawText(QRect(10, 65, 200, 25), Qt::AlignCenter, QStringLiteral("⚔️ 综合战力: 985,000"));

    painter.setPen(QColor(149, 165, 166));
    painter.drawText(QRect(10, 95, 200, 25), Qt::AlignCenter, QStringLiteral("👑 所属公会: 【星辰之翼】"));
    painter.drawText(QRect(10, 125, 200, 25), Qt::AlignCenter, QStringLiteral("🌟 天梯巅峰段位: 宗师 I"));

    ui->labelHeroImage->setPixmap(pix);
}

void MainWindow::appendLog(const QString &category, const QString &message)
{
    QString timeStr = QDateTime::currentDateTime().toString(QStringLiteral("HH:mm:ss.zzz"));
    QString line = QStringLiteral("[%1] [%2] %3").arg(timeStr).arg(category.leftJustified(6, ' ')).arg(message);
    ui->textLog->appendPlainText(line);
}

// ============================================================================
// Tab 1: QLabel
// ============================================================================
void MainWindow::onLinkActivated(const QString &link)
{
    if (link.startsWith(QStringLiteral("item://"))) {
        QString itemId = link.mid(7);
        appendLog(QStringLiteral("超链接"), QStringLiteral("触发游戏内部协议跳转 -> 装备图鉴 ID: %1").arg(itemId));
        QMessageBox::information(this, QStringLiteral("装备详情"),
                                 QStringLiteral("已调出【SSR·灰烬使者】神圣词条全属性面板！"));
    } else if (link.startsWith(QStringLiteral("quest://"))) {
        QString questId = link.mid(8);
        appendLog(QStringLiteral("超链接"), QStringLiteral("触发游戏内部协议跳转 -> 史诗任务 ID: %1").arg(questId));
        QMessageBox::information(this, QStringLiteral("接取任务"),
                                 QStringLiteral("已成功接取史诗任务【圣剑重铸】！"));
    } else if (link.startsWith(QStringLiteral("http"))) {
        appendLog(QStringLiteral("超链接"), QStringLiteral("调起系统默认浏览器打开外链: %1").arg(link));
        QDesktopServices::openUrl(QUrl(link));
    }
}

void MainWindow::onMovieFrameTick()
{
    const QVector<QString> frames = {
        QStringLiteral("🌟 [第 1 帧] 圣光惩戒！聚集圣洁能量..."),
        QStringLiteral("🔥 [第 2 帧] 灰烬裁决！巨剑撕裂虚空！"),
        QStringLiteral("⚡ [第 3 帧] 审判风暴！对周围造成 8 段暴击！"),
        QStringLiteral("👑 [第 4 帧] 天神下凡！进入无敌狂暴形态！")
    };
    const QVector<QString> colors = {
        QStringLiteral("#f1c40f"), QStringLiteral("#e74c3c"), QStringLiteral("#3498db"), QStringLiteral("#9b59b6")
    };

    m_movieFrameIndex = (m_movieFrameIndex + 1) % frames.size();
    ui->labelMovieAnim->setStyleSheet(
        QStringLiteral("background-color: #000000; border: 2px solid %1; border-radius: 6px; color: %1; font-size: 13px; font-weight: bold; qproperty-alignment: AlignCenter;")
        .arg(colors[m_movieFrameIndex])
    );
    ui->labelMovieAnim->setText(frames[m_movieFrameIndex]);
}

void MainWindow::onMoviePlay()
{
    m_movieFrameTimer->start();
    appendLog(QStringLiteral("动图控制"), QStringLiteral("播放技能动态特效帧动画。"));
}

void MainWindow::onMoviePause()
{
    m_movieFrameTimer->stop();
    appendLog(QStringLiteral("动图控制"), QStringLiteral("暂停技能动态特效帧动画。"));
}

void MainWindow::onMovieSpeed()
{
    if (m_movieSpeed == 100) {
        m_movieSpeed = 200;
        m_movieFrameTimer->setInterval(150);
        ui->btnMovieSpeed->setText(QStringLiteral("⚡ 1.0x 常速"));
        appendLog(QStringLiteral("动图控制"), QStringLiteral("特效帧率加速为 2.0x 倍速 (150ms/帧)"));
    } else {
        m_movieSpeed = 100;
        m_movieFrameTimer->setInterval(300);
        ui->btnMovieSpeed->setText(QStringLiteral("⚡ 2.0x 倍速"));
        appendLog(QStringLiteral("动图控制"), QStringLiteral("特效帧率恢复为 1.0x 常速 (300ms/帧)"));
    }
}

// ============================================================================
// Tab 2: QProgressBar
// ============================================================================
void MainWindow::onStartDownload()
{
    m_downloadProgress = 0;
    ui->progressBarDownload->setValue(0);
    m_downloadTimer->start(50); // 50ms 更新一次
    appendLog(QStringLiteral("进度条"), QStringLiteral("开始模拟热更资源分包下载 (总计 100 MB)..."));
}

void MainWindow::onResetDownload()
{
    m_downloadTimer->stop();
    m_downloadProgress = 0;
    ui->progressBarDownload->setValue(0);
    appendLog(QStringLiteral("进度条"), QStringLiteral("重置下载进度为 0%"));
}

void MainWindow::onDownloadTick()
{
    m_downloadProgress += 2;
    if (m_downloadProgress >= 100) {
        m_downloadProgress = 100;
        ui->progressBarDownload->setValue(100);
        m_downloadTimer->stop();
        appendLog(QStringLiteral("进度条"), QStringLiteral("🎉 游戏资源热更新包校验完毕，下载完成 100%！"));
        QMessageBox::information(this, QStringLiteral("热更新完成"), QStringLiteral("全部游戏资源分包已准备就绪！"));
    } else {
        ui->progressBarDownload->setValue(m_downloadProgress);
    }
}

void MainWindow::onBossDamage()
{
    int currentHp = ui->progressBossHp->value();
    currentHp = qMax(0, currentHp - 15);
    ui->progressBossHp->setValue(currentHp);

    int currentRage = ui->progressRage->value();
    currentRage = qMin(100, currentRage + 20);
    ui->progressRage->setValue(currentRage);

    appendLog(QStringLiteral("战斗血条"), QStringLiteral("击中首领！首领剩余 HP: %1%, 怒气值提升至: %2%").arg(currentHp).arg(currentRage));
    if (currentHp == 0) {
        QMessageBox::information(this, QStringLiteral("首领击杀"), QStringLiteral("首领已被成功击败！爆出金色传说神装！"));
    }
}

void MainWindow::onBossHeal()
{
    int currentHp = ui->progressBossHp->value();
    currentHp = qMin(100, currentHp + 25);
    ui->progressBossHp->setValue(currentHp);
    appendLog(QStringLiteral("战斗血条"), QStringLiteral("首领释放生命绽放！生命值恢复至: %1%").arg(currentHp));
}

// ============================================================================
// Tab 3: QLCDNumber
// ============================================================================
void MainWindow::updateLcdTimerDisplay()
{
    int minutes = m_remainingSeconds / 60;
    int seconds = m_remainingSeconds % 60;
    QString timeText = QStringLiteral("%1:%2")
                           .arg(minutes, 2, 10, QChar('0'))
                           .arg(seconds, 2, 10, QChar('0'));
    ui->lcdTimer->display(timeText);
}

void MainWindow::onStartTimer()
{
    m_countdownTimer->start(1000);
    appendLog(QStringLiteral("副本计时"), QStringLiteral("副本限时倒计时开始计时 (10:00)..."));
}

void MainWindow::onPauseTimer()
{
    m_countdownTimer->stop();
    appendLog(QStringLiteral("副本计时"), QStringLiteral("副本倒计时已暂停。"));
}

void MainWindow::onResetTimer()
{
    m_countdownTimer->stop();
    m_remainingSeconds = 600;
    updateLcdTimerDisplay();
    appendLog(QStringLiteral("副本计时"), QStringLiteral("重置副本倒计时为 10:00。"));
}

void MainWindow::onTimerCountdownTick()
{
    if (m_remainingSeconds > 0) {
        --m_remainingSeconds;
        updateLcdTimerDisplay();
    } else {
        m_countdownTimer->stop();
        appendLog(QStringLiteral("副本计时"), QStringLiteral("⚠️ 副本挑战时间耗尽！判定战败！"));
        QMessageBox::warning(this, QStringLiteral("时间耗尽"), QStringLiteral("很遗憾，未能于 10 分钟内通关地下城！"));
    }
}

void MainWindow::onModeDec()
{
    ui->lcdCombo->setDecMode();
    appendLog(QStringLiteral("LCD 进制"), QStringLiteral("连击液晶屏切换为十进制 (DEC) 模式"));
}

void MainWindow::onModeHex()
{
    ui->lcdCombo->setHexMode();
    appendLog(QStringLiteral("LCD 进制"), QStringLiteral("连击液晶屏切换为十六进制 (HEX) 模式"));
}

void MainWindow::onModeBin()
{
    ui->lcdCombo->setBinMode();
    appendLog(QStringLiteral("LCD 进制"), QStringLiteral("连击液晶屏切换为二进制 (BIN) 模式"));
}

void MainWindow::onHitAdd1()
{
    ++m_comboCount;
    ui->lcdCombo->display(m_comboCount);
    appendLog(QStringLiteral("连击数"), QStringLiteral("连击数 +1 -> 当前 Hits: %1").arg(m_comboCount));
}

void MainWindow::onHitAdd10()
{
    m_comboCount += 10;
    ui->lcdCombo->display(m_comboCount);
    appendLog(QStringLiteral("连击数"), QStringLiteral("狂暴连击 +10 -> 当前 Hits: %1").arg(m_comboCount));
}

void MainWindow::onHitReset()
{
    m_comboCount = 0;
    ui->lcdCombo->display(0);
    appendLog(QStringLiteral("连击数"), QStringLiteral("连击中断归零 (0 Hits)"));
}
