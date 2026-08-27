#pragma once

#include <QWidget>

class QLabel;
class QPushButton;
class QVBoxLayout;

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget() override = default;

private slots:
    void onButtonClicked();

private:
    QLabel *m_label{nullptr};
    QPushButton *m_button{nullptr};
    int m_clickCount{0};
};
