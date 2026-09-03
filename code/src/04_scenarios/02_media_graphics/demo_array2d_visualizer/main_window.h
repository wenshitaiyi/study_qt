#pragma once

#include <QMainWindow>
#include "grid_data_model.h"

class VisualizerWidget;
class ControlPanel;
class QLabel;
class QSplitter;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

private:
    void setupUi();
    void setupConnections();
    void initialDataGeneration();

    GridDataModel *m_model{nullptr};
    ControlPanel *m_controlPanel{nullptr};
    VisualizerWidget *m_visualizer{nullptr};
    QSplitter *m_splitter{nullptr};

    // 状态栏组件
    QLabel *m_statusHoverLabel{nullptr};
    QLabel *m_statusValueLabel{nullptr};
    QLabel *m_statusZoomLabel{nullptr};
    QLabel *m_statusDimLabel{nullptr};
    QLabel *m_statusRenderLabel{nullptr};
};
