#pragma once

#include <QWidget>
#include <memory>

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget() override;

private slots:
    void onButtonClicked();

private:
    std::unique_ptr<Ui::MainWidget> m_ui;
    int m_clickCount{0};
};
