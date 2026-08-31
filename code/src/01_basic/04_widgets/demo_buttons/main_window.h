#pragma once

#include <QMainWindow>
#include <QButtonGroup>
#include <QMenu>
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
    // 1. QPushButton 槽函数
    void onNormalAttack();
    void onUltimateSkill();
    void onToggleStance(bool checked);
    void onComboActionTriggered(QAction *action);
    void onCooldownTick();

    // 2. QToolButton 槽函数
    void onDungeonActionTriggered(QAction *action);
    void onRadarActionTriggered(QAction *action);
    void onPrevPage();
    void onNextPage();
    void onAutoRaiseClicked();

    // 3. QRadioButton & QButtonGroup 槽函数
    void onCampChanged(int id);
    void onQualityChanged(int id);

    // 4. QCheckBox 槽函数
    void onAutoBattleToggled(bool checked);
    void onLootAllStateChanged(int state);
    void onSubLootChanged();

    // 5. QMessageBox 全家桶槽函数
    void onMsgInfo();
    void onMsgWarning();
    void onMsgCritical();
    void onMsgQuestion();
    void onMsgCustom();
    void onMsgAbout();

private:
    void initMenusAndButtons();
    void appendLog(const QString &category, const QString &message);
    void updateLootAllState();

    std::unique_ptr<Ui::MainWindow> ui;

    // 按钮分组与冷却计时器
    QButtonGroup *m_campGroup{nullptr};
    QButtonGroup *m_qualityGroup{nullptr};
    QTimer *m_cooldownTimer{nullptr};
    int m_remainingCd{0};
    int m_currentChapter{1};
};
