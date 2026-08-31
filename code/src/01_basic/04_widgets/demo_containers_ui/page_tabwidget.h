#pragma once

#include <QWidget>
#include <memory>

namespace Ui {
class PageTabWidget;
}

class PageTabWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PageTabWidget(QWidget *parent = nullptr);
    ~PageTabWidget() override;

signals:
    void logMessage(const QString &category, const QString &message);

private slots:
    void onTabPositionChanged(int index);
    void onClosableToggled(bool checked);
    void onMovableToggled(bool checked);
    void onAddNewTabClicked();
    void onTabCloseRequested(int index);
    void onCurrentTabChanged(int index);

private:
    void initCornerWidget();

    std::unique_ptr<Ui::PageTabWidget> ui;
    int m_newTabCounter{1};
};
