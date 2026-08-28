#pragma once

#include <QMainWindow>
#include <memory>

namespace Ui {
class MainWindow;
}

class QLabel;
class QListWidgetItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onActionNewTriggered();
    void onActionOpenTriggered();
    void onActionSaveTriggered();
    void onActionExitTriggered();
    void onActionAboutTriggered();
    void onActionAboutQtTriggered();
    void onTextEditorChanged();
    void onListItemDoubleClicked(QListWidgetItem *item);

private:
    void setupUiCustomizations();
    void updateStatusBarInfo();

private:
    std::unique_ptr<Ui::MainWindow> m_ui;
    QLabel *m_statusInfoLabel{nullptr};
    QLabel *m_statusEncodingLabel{nullptr};
    QString m_currentFilePath;
};
