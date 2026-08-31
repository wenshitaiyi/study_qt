#pragma once

#include <QWidget>
#include <memory>

namespace Ui {
class PageSplitter;
}

class PageSplitter : public QWidget
{
    Q_OBJECT

public:
    explicit PageSplitter(QWidget *parent = nullptr);
    ~PageSplitter() override;

signals:
    void logMessage(const QString &category, const QString &message);

private slots:
    void onResetRatioClicked();
    void onCollapsibleToggled(bool checked);
    void onSplitterMoved(int pos, int index);

private:
    std::unique_ptr<Ui::PageSplitter> ui;
};
