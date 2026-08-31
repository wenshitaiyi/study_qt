#pragma once

#include <QWidget>
#include <memory>

namespace Ui {
class PageGroupBox;
}

class PageGroupBox : public QWidget
{
    Q_OBJECT

public:
    explicit PageGroupBox(QWidget *parent = nullptr);
    ~PageGroupBox() override;

signals:
    void logMessage(const QString &category, const QString &message);

private slots:
    void onCheckableToggled(bool checked);

private:
    std::unique_ptr<Ui::PageGroupBox> ui;
};
