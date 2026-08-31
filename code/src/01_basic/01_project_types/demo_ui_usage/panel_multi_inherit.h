#pragma once

#include <QWidget>
#include "ui_panel_multi_inherit.h"

// 多重继承：同时继承 QWidget 与 Ui::PanelMultiInherit
class PanelMultiInherit : public QWidget, private Ui::PanelMultiInherit
{
    Q_OBJECT

public:
    explicit PanelMultiInherit(QWidget *parent = nullptr);
    ~PanelMultiInherit() override = default;

private slots:
    void onButtonClicked();

private:
    int m_counter{0};
};
