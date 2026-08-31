#pragma once

#include <QDialog>
#include <memory>
#include "custom_modal_dialog.h"

namespace Ui {
class MainDialog;
}

class CustomModalDialog;

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = nullptr);
    ~MainDialog() override;

private slots:
    void onBtnModalClicked();
    void onBtnModelessClicked();
    void onBtnMessageBoxClicked();
    void onBtnFileDialogClicked();
    void onBtnColorDialogClicked();
    void onBtnFontDialogClicked();
    void onBtnInputDialogClicked();
    void onBtnClearLogClicked();
    void onModelessConfigApplied(const DialogConfig &config);

private:
    void appendLog(const QString &category, const QString &message);

private:
    std::unique_ptr<Ui::MainDialog> m_ui;
    DialogConfig m_currentConfig{QStringLiteral("Developer"), 9000, QStringLiteral("调试模式 (Debug)"), true};
    CustomModalDialog *m_modelessDialog{nullptr};
};
