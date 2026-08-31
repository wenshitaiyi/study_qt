#pragma once

#include <QWidget>

class QLabel;
class QPushButton;
class QSlider;

// 纯代码构建方式：不依赖任何 .ui 文件或 uic 预编译器
class PanelPureCode : public QWidget
{
    Q_OBJECT

public:
    explicit PanelPureCode(QWidget *parent = nullptr);
    ~PanelPureCode() override = default;

private slots:
    void onSliderValueChanged(int value);
    void onResetClicked();

private:
    void initUiLayout();

private:
    QLabel *m_labelResult{nullptr};
    QSlider *m_slider{nullptr};
    QPushButton *m_btnReset{nullptr};
};
