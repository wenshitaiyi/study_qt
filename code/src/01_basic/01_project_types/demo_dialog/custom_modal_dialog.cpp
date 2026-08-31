#include "custom_modal_dialog.h"
#include "ui_custom_modal_dialog.h"

CustomModalDialog::CustomModalDialog(QWidget *parent)
    : QDialog(parent)
    , m_ui(std::make_unique<Ui::CustomModalDialog>())
{
    m_ui->setupUi(this);

    connect(m_ui->buttonBox, &QDialogButtonBox::accepted, this, &CustomModalDialog::onAcceptClicked);
}

CustomModalDialog::~CustomModalDialog() = default;

void CustomModalDialog::setConfig(const DialogConfig &config)
{
    m_ui->editUsername->setText(config.username);
    m_ui->spinPort->setValue(config.port);
    int index = m_ui->comboMode->findText(config.mode);
    if (index >= 0) {
        m_ui->comboMode->setCurrentIndex(index);
    }
    m_ui->checkAutoReconnect->setChecked(config.autoReconnect);
}

DialogConfig CustomModalDialog::getConfig() const
{
    DialogConfig cfg;
    cfg.username = m_ui->editUsername->text().trimmed();
    cfg.port = m_ui->spinPort->value();
    cfg.mode = m_ui->comboMode->currentText();
    cfg.autoReconnect = m_ui->checkAutoReconnect->isChecked();
    return cfg;
}

void CustomModalDialog::onAcceptClicked()
{
    emit configApplied(getConfig());
}
