#pragma once

#include <QMainWindow>
#include <QCompleter>
#include <memory>

namespace Ui {
class MainWindow;
}

/**
 * @brief Qt 常用输入与编辑器组件使用指南演示窗口
 * 
 * 本类通过 4 个独立的 Tab 页面，系统化演示 Qt 中最核心的输入编辑控件：
 * 1. QLineEdit: 回显模式、掩码、整数/正则校验器、自动补全 (QCompleter)
 * 2. QSpinBox / QDoubleSpinBox: 整数/浮点微调、前后缀、步长、循环滚动、特殊文本
 * 3. QComboBox / QFontComboBox / QKeySequenceEdit / QDateTimeEdit: 下拉数据绑定、系统字体、按键捕获、日历
 * 4. QTextEdit / QPlainTextEdit: 富文本排版与 HTML 操作、高性能纯文本与最大行数控制
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    // Tab 1: QLineEdit 槽函数
    void onEchoNormalChanged(const QString &text);
    void onMaskIpChanged(const QString &text);
    void onMaskSerialChanged(const QString &text);
    void onValIntChanged(const QString &text);
    void onValRegexChanged(const QString &text);

    // Tab 2: SpinBox 槽函数
    void onSpinLevelChanged(int val);
    void onSpinStepChanged(int val);
    void onDoubleSpinPriceChanged(double val);
    void onDoubleSpinRateChanged(double val);

    // Tab 3: ComboBox, Font, Key, Date 槽函数
    void onComboDataIndexChanged(int index);
    void onFontChanged(const QFont &font);
    void onKeySequenceChanged(const QKeySequence &keySeq);
    void onDateTimeChanged(const QDateTime &dateTime);

    // Tab 4: QTextEdit 格式化工具槽函数
    void onFormatBold();
    void onFormatItalic();
    void onFormatUnderline();
    void onFormatColorRed();
    void onFormatColorBlue();

private:
    void initLineEdits();
    void initSpinBoxes();
    void initComboBoxesAndMisc();
    void initTextEdits();
    void appendLog(const QString &category, const QString &message);

    std::unique_ptr<Ui::MainWindow> ui;
    QCompleter *m_completer{nullptr};
};
