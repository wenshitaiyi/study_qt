#pragma once

#include <QMainWindow>
#include <QTimer>
#include <memory>

namespace Ui {
class MainWindow;
}

/**
 * @brief Qt 常用展示控件使用指南演示窗口
 * 
 * 本类通过 4 个独立的 Tab 页面，系统化演示 Qt 中最核心的展示与输出控件：
 * 1. QLabel: 富文本 HTML 排版、超链接信号拦截、QPixmap 图像绘制、QMovie 动态帧控制
 * 2. QProgressBar: 确定进度、格式化输出 (setFormat)、跑马灯不确定模式 (0, 0)、垂直与反向显示
 * 3. QLCDNumber: 仿真时钟显示 (display)、液晶风格切换 (Outline/Filled/Flat)、进制切换 (DEC/HEX/BIN)
 * 4. 辅助展示: QToolTip 气泡提示、QWhatsThis 上下文帮助说明
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    // Tab 1: QLabel 与超链接
    void onRichTextLinkActivated(const QString &link);
    void onMoviePlay();
    void onMoviePause();
    void onMovieSpeedToggle();
    void onMovieFrameTick();

    // Tab 2: QProgressBar
    void onStartProgress();
    void onResetProgress();
    void onProgressTimerTick();
    void onVerticalPlus();
    void onVerticalMinus();

    // Tab 3: QLCDNumber
    void onClockTimerTick();
    void onStyleFilled();
    void onStyleOutline();
    void onStyleFlat();
    void onModeDec();
    void onModeHex();
    void onModeBin();
    void onCountAdd1();
    void onCountAdd10();
    void onCountReset();

private:
    void initLabelsAndMovie();
    void initProgressBars();
    void initLcdNumbers();
    void appendLog(const QString &category, const QString &message);

    std::unique_ptr<Ui::MainWindow> ui;

    // 动态模拟计时器
    QTimer *m_clockTimer{nullptr};
    QTimer *m_progressTimer{nullptr};
    int m_progressValue{0};

    QTimer *m_movieTimer{nullptr};
    int m_movieFrame{0};
    bool m_movieSpeed2x{false};

    int m_counterValue{42};
};
