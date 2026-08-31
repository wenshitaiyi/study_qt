#pragma once

#include <QDialog>
#include <memory>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog() override;

protected:
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;

private slots:
    void onSaveClicked();
    void onResetClicked();
    void onReloadClicked();
    void onTabChanged(int index);

private:
    void loadSettingsToUi();
    void saveUiToSettings();
    void updatePreviewTab();

    std::unique_ptr<Ui::SettingsDialog> ui;
};
