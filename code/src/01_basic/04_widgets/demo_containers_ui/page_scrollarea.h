#pragma once

#include <QWidget>
#include <memory>

namespace Ui {
class PageScrollArea;
}

class PageScrollArea : public QWidget
{
    Q_OBJECT

public:
    explicit PageScrollArea(QWidget *parent = nullptr);
    ~PageScrollArea() override;

signals:
    void logMessage(const QString &category, const QString &message);

private slots:
    void onWidgetResizableToggled(bool checked);
    void onHBarPolicyChanged(int index);
    void onScrollToBottomClicked();

private:
    void populateScrollCards();

    std::unique_ptr<Ui::PageScrollArea> ui;
};
