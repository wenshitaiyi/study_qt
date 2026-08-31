#pragma once

#include <QWidget>
#include <memory>

// 前置声明：头文件无需包含 ui_panel_pointer_member.h
namespace Ui {
class PanelPointerMember;
}

class PanelPointerMember : public QWidget
{
    Q_OBJECT

public:
    explicit PanelPointerMember(QWidget *parent = nullptr);
    ~PanelPointerMember() override;

private slots:
    void onButtonClicked();

private:
    std::unique_ptr<Ui::PanelPointerMember> m_ui;
    int m_counter{0};
};
