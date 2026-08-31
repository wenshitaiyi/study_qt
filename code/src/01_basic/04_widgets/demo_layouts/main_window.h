#pragma once

#include <QMainWindow>
#include <memory>

namespace Ui {
class MainWindow;
}

/**
 * @brief Qt 五大核心布局管理器使用指南演示窗口
 * 
 * 本类通过 5 个独立的 Tab 页面，系统化罗列并演示 Qt 中最核心的 5 大布局方式：
 * 1. QHBoxLayout / QVBoxLayout (水平与垂直线性布局)
 * 2. QGridLayout (网格矩阵布局与跨行跨列)
 * 3. QFormLayout (两列键值表单布局)
 * 4. QStackedWidget / QStackedLayout (多步骤堆叠向导布局)
 * 5. 动态调参实验室 (运行时动态修改 Margins、Spacing、Stretch)
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    // Tab 1: 水平与垂直布局事件
    void onHBoxButtonClicked();
    void onVBoxButtonClicked();

    // Tab 2: 网格布局单元格事件
    void onGridCellClicked();

    // Tab 3: 表单提交事件
    void onFormSubmitClicked();

    // Tab 4: 向导步骤切换事件
    void onWizardPrevClicked();
    void onWizardNextClicked();
    void onWizardPageChanged(int index);

    // Tab 5: 动态布局调谐事件 (Margin, Spacing, Stretch)
    void onMarginSliderChanged(int value);
    void onSpacingSliderChanged(int value);
    void onStretch111Clicked();
    void onStretch121Clicked();
    void onStretch132Clicked();

private:
    void initLayoutsConfig();
    void updateWizardNavigation();
    void setStatusTip(const QString &tip);

    std::unique_ptr<Ui::MainWindow> ui;
};
