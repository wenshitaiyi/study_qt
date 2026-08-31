#pragma once

#include <QWidget>
#include <memory>

namespace Ui {
class PageToolBox;
}

class PageToolBox : public QWidget
{
    Q_OBJECT

public:
    explicit PageToolBox(QWidget *parent = nullptr);
    ~PageToolBox() override;

signals:
    void logMessage(const QString &category, const QString &message);

private slots:
    void onToolboxCurrentChanged(int index);
    void onSwitchNextDrawerClicked();
    void onAddDrawerClicked();

private:
    std::unique_ptr<Ui::PageToolBox> ui;
    int m_drawerCounter{1};
};
