#pragma once

#include <QMainWindow>
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
    // Tab 1: 技能动作栏
    void onSkillSlotClicked();

    // Tab 2: 背包物品点击
    void onInventoryItemClicked();

    // Tab 3: 创建英雄表单
    void onCreateCharClicked();

    // Tab 4: 觉醒向导切换
    void onWizardPrev();
    void onWizardNext();

    // Tab 5: 动态布局调谐
    void onMarginChanged(int value);
    void onSpacingChanged(int value);
    void onStretch111();
    void onStretch121();
    void onStretch132();

private:
    void initInventoryGrid();
    void updateWizardUI();
    void setStatus(const QString &msg);

    std::unique_ptr<Ui::MainWindow> ui;
};
