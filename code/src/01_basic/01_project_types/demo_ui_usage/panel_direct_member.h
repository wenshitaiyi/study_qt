#pragma once

#include <QWidget>
// 直接引入生成的 ui 头文件
#include "ui_panel_direct_member.h"

class PanelDirectMember : public QWidget
{
    Q_OBJECT

public:
    explicit PanelDirectMember(QWidget *parent = nullptr);
    ~PanelDirectMember() override = default;

private slots:
    void onButtonClicked();

private:
    // 直接作为值对象成员
    Ui::PanelDirectMember m_ui;
    int m_counter{0};
};
