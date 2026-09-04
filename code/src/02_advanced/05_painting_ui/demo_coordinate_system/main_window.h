#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class TransformCanvas;
class QLabel;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    // 图元与包围盒
    void onPrimitiveIndexChanged(int index);
    void onBoundingBoxToggled(bool checked);

    // 变换微调输入
    void onRotationSpinChanged(double val);
    void onRotationSliderChanged(int val);
    void onSizeChanged();
    void onCenterChanged();

    // 重置操作
    void onFitInViewClicked();
    void onFitWidthClicked();
    void onFitHeightClicked();
    void onResetRotationClicked();

    // 视口
    void onViewportEnabledToggled(bool checked);
    void onViewportBorderToggled(bool checked);

    // 画布状态与同步
    void onCanvasStatusChanged(const QString &text);
    void onCanvasTransformChanged();
    void onCanvasViewportChanged();

private:
    void setupUiCustom();
    void connectSignals();
    void syncUiFromCanvas();

private:
    Ui::MainWindow *ui{nullptr};
    TransformCanvas *m_canvas{nullptr};
    QLabel *m_statusLabel{nullptr};
    bool m_updatingFromCanvas{false};
};

#endif // MAIN_WINDOW_H
