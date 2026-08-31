#include "settings_dialog.h"
#include "ui_settings_dialog.h"
#include "app_settings.h"

#include <QMessageBox>
#include <QCloseEvent>
#include <QShowEvent>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(std::make_unique<Ui::SettingsDialog>())
{
    ui->setupUi(this);

    // 信号槽连接
    connect(ui->btnSave, &QPushButton::clicked, this, &SettingsDialog::onSaveClicked);
    connect(ui->btnReset, &QPushButton::clicked, this, &SettingsDialog::onResetClicked);
    connect(ui->btnReload, &QPushButton::clicked, this, &SettingsDialog::onReloadClicked);
    connect(ui->btnClose, &QPushButton::clicked, this, &SettingsDialog::close);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &SettingsDialog::onTabChanged);

    // 加载配置
    loadSettingsToUi();
    updatePreviewTab();
}

SettingsDialog::~SettingsDialog() = default;

void SettingsDialog::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);

    // 恢复窗口上次记忆的尺寸与位置 (Geometry)
    QByteArray geometry = AppSettings::instance().windowGeometry();
    if (!geometry.isEmpty()) {
        restoreGeometry(geometry);
    }
}

void SettingsDialog::closeEvent(QCloseEvent *event)
{
    // 关闭时自动保存窗口位置与大小
    AppSettings::instance().setWindowGeometry(saveGeometry());
    AppSettings::instance().sync();

    QDialog::closeEvent(event);
}

void SettingsDialog::loadSettingsToUi()
{
    auto &settings = AppSettings::instance();

    // 1. 常规
    ui->comboTheme->setCurrentText(settings.theme());
    ui->comboLanguage->setCurrentText(settings.language());
    ui->chkAutoStart->setChecked(settings.autoStart());

    // 2. 网络
    ui->editHost->setText(settings.serverHost());
    ui->spinPort->setValue(settings.serverPort());
    ui->spinTimeout->setValue(settings.timeoutMs());

    // 3. 用户
    ui->editUsername->setText(settings.lastUsername());
    ui->chkRememberPwd->setChecked(settings.rememberPassword());
}

void SettingsDialog::saveUiToSettings()
{
    auto &settings = AppSettings::instance();

    // 1. 常规
    settings.setTheme(ui->comboTheme->currentText());
    settings.setLanguage(ui->comboLanguage->currentText());
    settings.setAutoStart(ui->chkAutoStart->isChecked());

    // 2. 网络
    settings.setServerHost(ui->editHost->text().trimmed());
    settings.setServerPort(ui->spinPort->value());
    settings.setTimeoutMs(ui->spinTimeout->value());

    // 3. 用户
    settings.setLastUsername(ui->editUsername->text().trimmed());
    settings.setRememberPassword(ui->chkRememberPwd->isChecked());

    // 4. 强制刷盘
    settings.sync();
}

void SettingsDialog::updatePreviewTab()
{
    ui->labelFilePath->setText(QStringLiteral("<b>配置文件磁盘路径：</b><br>%1").arg(AppSettings::instance().iniFilePath()));
    ui->textIniPreview->setPlainText(AppSettings::instance().rawIniContent());
}

void SettingsDialog::onSaveClicked()
{
    saveUiToSettings();
    updatePreviewTab();
    QMessageBox::information(this, QStringLiteral("配置保存成功"),
                             QStringLiteral("所有参数已成功写入 INI 文件并实时同步！"));
}

void SettingsDialog::onResetClicked()
{
    auto reply = QMessageBox::question(this, QStringLiteral("重置确认"),
                                       QStringLiteral("确定要将所有配置参数恢复为系统初始默认值吗？"),
                                       QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        AppSettings::instance().resetToDefaults();
        loadSettingsToUi();
        updatePreviewTab();
    }
}

void SettingsDialog::onReloadClicked()
{
    loadSettingsToUi();
    updatePreviewTab();
    QMessageBox::information(this, QStringLiteral("重新加载完成"),
                             QStringLiteral("已从磁盘配置文件重新刷新读取全部设置项。"));
}

void SettingsDialog::onTabChanged(int index)
{
    if (index == 1) {
        updatePreviewTab();
    }
}
