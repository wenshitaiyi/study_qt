#pragma once

#include <QDialog>
#include <memory>

namespace Ui {
class CustomModalDialog;
}

struct DialogConfig
{
    QString username;
    int port{8080};
    QString mode;
    bool autoReconnect{true};
};

class CustomModalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CustomModalDialog(QWidget *parent = nullptr);
    ~CustomModalDialog() override;

    void setConfig(const DialogConfig &config);
    DialogConfig getConfig() const;

signals:
    void configApplied(const DialogConfig &config);

private slots:
    void onAcceptClicked();

private:
    std::unique_ptr<Ui::CustomModalDialog> m_ui;
};
