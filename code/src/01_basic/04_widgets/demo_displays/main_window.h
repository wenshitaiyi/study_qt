#pragma once

#include <QMainWindow>
#include <QTimer>
#include <memory>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    // Tab 1: QLabel
    void onLinkActivated(const QString &link);
    void onMoviePlay();
    void onMoviePause();
    void onMovieSpeed();
    void onMovieFrameTick();

    // Tab 2: QProgressBar
    void onStartDownload();
    void onResetDownload();
    void onDownloadTick();
    void onBossDamage();
    void onBossHeal();

    // Tab 3: QLCDNumber
    void onStartTimer();
    void onPauseTimer();
    void onResetTimer();
    void onTimerCountdownTick();
    void onModeDec();
    void onModeHex();
    void onModeBin();
    void onHitAdd1();
    void onHitAdd10();
    void onHitReset();

private:
    void initHeroPortrait();
    void updateLcdTimerDisplay();
    void appendLog(const QString &category, const QString &message);

    std::unique_ptr<Ui::MainWindow> ui;

    // 动效与计时器
    QTimer *m_downloadTimer{nullptr};
    int m_downloadProgress{0};

    QTimer *m_countdownTimer{nullptr};
    int m_remainingSeconds{600}; // 10 分钟 = 600 秒

    QTimer *m_movieFrameTimer{nullptr};
    int m_movieFrameIndex{0};
    int m_movieSpeed{100}; // 100%

    int m_comboCount{128};
};
