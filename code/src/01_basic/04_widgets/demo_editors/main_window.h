#pragma once

#include <QMainWindow>
#include <QCompleter>
#include <QRegularExpressionValidator>
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
    // Tab 1: QLineEdit
    void onAccountChanged(const QString &text);
    void onCdkChanged(const QString &text);
    void onIpChanged(const QString &text);
    void onNicknameChanged(const QString &text);

    // Tab 2: SpinBox & ComboBox & KeySequence
    void onLevelChanged(int val);
    void onGoldChanged(int val);
    void onCritRateChanged(double val);
    void onDiscountChanged(double val);
    void onRarityChanged(int index);
    void onFontChanged(const QFont &font);
    void onKeySequenceChanged(const QKeySequence &keySeq);
    void onDateTimeChanged(const QDateTime &dt);

    // Tab 3: QTextEdit & QPlainTextEdit
    void onBoldClicked();
    void onItalicClicked();
    void onUnderlineClicked();
    void onColorRedClicked();
    void onColorGoldClicked();
    void onExecuteGmScript();

    // Tab 4: QSlider & QDial
    void onMasterVolumeChanged(int val);
    void onBgmVolumeChanged(int val);
    void onDialCameraAngleChanged(int val);

private:
    void initValidatorsAndCompleter();
    void initComboBoxData();
    void initRichTextDefault();
    void appendLog(const QString &category, const QString &message);

    std::unique_ptr<Ui::MainWindow> ui;
    QCompleter *m_itemCompleter{nullptr};
};
