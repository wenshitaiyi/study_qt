#include "main_window.h"
#include "control_panel.h"
#include "visualizer_widget.h"
#include "colourmap_manager.h"

#include <QSplitter>
#include <QStatusBar>
#include <QLabel>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QSettings>
#include <QDir>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QStringLiteral("海量 2D 数组与连续过程噪声数据可视化工作台 - demo_array2d_visualizer"));
    resize(1280, 850);

    m_model = new GridDataModel(this);

    setupUi();
    setupConnections();
    m_controlPanel->syncToVisualizer(m_visualizer);
    initialDataGeneration();
    loadWindowState();
}

void MainWindow::setupUi()
{
    m_splitter = new QSplitter(Qt::Horizontal, this);

    m_controlPanel = new ControlPanel(m_splitter);
    m_visualizer = new VisualizerWidget(m_splitter);
    m_visualizer->setModel(m_model);

    m_splitter->addWidget(m_controlPanel);
    m_splitter->addWidget(m_visualizer);
    m_splitter->setStretchFactor(0, 0); // 左侧保持自然宽度
    m_splitter->setStretchFactor(1, 1); // 右侧画布优先拉伸
    m_splitter->setCollapsible(0, false);
    m_splitter->setCollapsible(1, false);

    setCentralWidget(m_splitter);

    // 状态栏
    auto *sb = statusBar();
    m_statusHoverLabel = new QLabel(QStringLiteral("光标: [-]"), this);
    m_statusValueLabel = new QLabel(QStringLiteral("数值: [-]"), this);
    m_statusZoomLabel = new QLabel(QStringLiteral("缩放: 15.0px"), this);
    m_statusDimLabel = new QLabel(QStringLiteral("规模: 150x150 (22,500 格)"), this);
    m_statusRenderLabel = new QLabel(QStringLiteral("帧耗时: - ms"), this);

    m_statusHoverLabel->setMinimumWidth(150);
    m_statusValueLabel->setMinimumWidth(180);
    m_statusZoomLabel->setMinimumWidth(130);
    m_statusDimLabel->setMinimumWidth(200);

    sb->addWidget(m_statusHoverLabel);
    sb->addWidget(m_statusValueLabel);
    sb->addWidget(m_statusZoomLabel);
    sb->addWidget(m_statusDimLabel);
    sb->addPermanentWidget(m_statusRenderLabel);

    // 菜单栏
    auto *helpMenu = menuBar()->addMenu(QStringLiteral("帮助 (&H)"));
    auto *aboutAction = helpMenu->addAction(QStringLiteral("关于本 Demo (&A)"));
    connect(aboutAction, &QAction::triggered, this, [this]() {
        QMessageBox::about(this, QStringLiteral("关于 demo_array2d_visualizer"),
            QStringLiteral("<h3>海量 2D 数组与连续过程噪声数据可视化系统</h3>"
                           "<p><b>技术亮点：</b></p>"
                           "<ul>"
                           "<li><b>连续过程噪声引擎</b>：支持 Perlin、fBm、Worley、调和波场与热源扩散。</li>"
                           "<li><b>专业科学色带体系</b>：基于 ScientificColourMaps8 构建，提供 62 款科学防伪色带。</li>"
                           "<li><b>LOD 分级自绘</b>：全图离屏贴图与视口裁剪双重加速，支持百万点 60 FPS 流畅渲染。</li>"
                           "<li><b>全套交互与动效</b>：Ctrl+滚轮锚点缩放、拖拽平移、鹰眼小地图、色彩过滤、光标圆域物理浮起微动效与格内反色数值显示。</li>"
                           "</ul>"));
    });
}

void MainWindow::setupConnections()
{
    // 控制面板 -> 模型
    connect(m_controlPanel, &ControlPanel::generateRequested, m_model, &GridDataModel::generateAsync);

    // 模型状态 -> 控制面板与状态栏
    connect(m_model, &GridDataModel::generationStarted, m_controlPanel, &ControlPanel::onGenerationStarted);
    connect(m_model, &GridDataModel::generationFinished, m_controlPanel, &ControlPanel::onGenerationFinished);
    connect(m_model, &GridDataModel::generationFinished, this, [this]() {
        m_controlPanel->updateDataStats(m_model->minValue(), m_model->maxValue(), m_model->totalCells());
        m_statusDimLabel->setText(QStringLiteral("规模: %1x%2 (%3 格)")
                                  .arg(m_model->rows())
                                  .arg(m_model->cols())
                                  .arg(m_model->totalCells()));
    });

    // 控制面板 -> 画布展示
    connect(m_controlPanel, &ControlPanel::colourMapChanged, m_visualizer, &VisualizerWidget::setColourMap);
    connect(m_controlPanel, &ControlPanel::filterChanged, m_visualizer, &VisualizerWidget::setFilterRange);
    connect(m_controlPanel, &ControlPanel::borderToggled, m_visualizer, &VisualizerWidget::setBorderEnabled);
    connect(m_controlPanel, &ControlPanel::borderColorChanged, m_visualizer, &VisualizerWidget::setBorderColor);
    connect(m_controlPanel, &ControlPanel::borderWidthChanged, m_visualizer, &VisualizerWidget::setBorderWidth);
    connect(m_controlPanel, &ControlPanel::hoverEffectToggled, m_visualizer, &VisualizerWidget::setHoverEffectEnabled);
    connect(m_controlPanel, &ControlPanel::hoverRadiusChanged, m_visualizer, &VisualizerWidget::setHoverRadius);
    connect(m_controlPanel, &ControlPanel::elevationIntensityChanged, m_visualizer, &VisualizerWidget::setElevationIntensity);
    connect(m_controlPanel, &ControlPanel::hoverHighlightToggled, m_visualizer, &VisualizerWidget::setHoverHighlightEnabled);
    connect(m_controlPanel, &ControlPanel::hoverHighlightColorChanged, m_visualizer, &VisualizerWidget::setHoverHighlightColor);
    connect(m_controlPanel, &ControlPanel::hoverHighlightWidthChanged, m_visualizer, &VisualizerWidget::setHoverHighlightWidth);
    connect(m_controlPanel, &ControlPanel::showHoverInfoCardToggled, m_visualizer, &VisualizerWidget::setShowHoverInfoCard);
    connect(m_controlPanel, &ControlPanel::showValuesToggled, m_visualizer, &VisualizerWidget::setShowValues);
    connect(m_controlPanel, &ControlPanel::showMinimapToggled, m_visualizer, &VisualizerWidget::setShowMinimap);
    connect(m_controlPanel, &ControlPanel::resetViewRequested, m_visualizer, &VisualizerWidget::resetView);
    connect(m_controlPanel, &ControlPanel::fitWindowRequested, m_visualizer, &VisualizerWidget::fitToWindow);

    // 画布状态 -> 状态栏
    connect(m_visualizer, &VisualizerWidget::cellHovered, this, [this](int r, int c, double rawVal, double normVal) {
        if (r >= 0 && c >= 0) {
            m_statusHoverLabel->setText(QStringLiteral("光标: 行 %1, 列 %2").arg(r).arg(c));
            m_statusValueLabel->setText(QStringLiteral("数值: %1 (归一: %2)")
                                        .arg(rawVal, 0, 'f', 3)
                                        .arg(normVal, 0, 'f', 2));
        } else {
            m_statusHoverLabel->setText(QStringLiteral("光标: [-]"));
            m_statusValueLabel->setText(QStringLiteral("数值: [-]"));
        }
    });

    connect(m_visualizer, &VisualizerWidget::zoomChanged, this, [this](double zoomFactor) {
        m_statusZoomLabel->setText(QStringLiteral("缩放: %1px / 格").arg(zoomFactor, 0, 'f', 1));
    });

    connect(m_visualizer, &VisualizerWidget::renderStatsUpdated, this, [this](double frameTimeMs, int visibleCells) {
        m_statusRenderLabel->setText(QStringLiteral("渲染: %1 ms | 可视: %2 格")
                                     .arg(frameTimeMs, 0, 'f', 2)
                                     .arg(visibleCells));
    });
}

void MainWindow::initialDataGeneration()
{
    // 初始化生成默认数据
    m_model->generateSync(m_controlPanel->currentParams());
    m_visualizer->fitToWindow();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    m_controlPanel->saveConfig();
    saveWindowState();
    QMainWindow::closeEvent(event);
}

void MainWindow::saveWindowState()
{
    QString configPath = QDir::tempPath() + QStringLiteral("/study_qt_demo_array2d_config.ini");
    QSettings s(configPath, QSettings::IniFormat);
    s.beginGroup(QStringLiteral("MainWindow"));
    s.setValue(QStringLiteral("geometry"), saveGeometry());
    s.setValue(QStringLiteral("splitter"), m_splitter->saveState());
    s.endGroup();
    s.sync();
}

void MainWindow::loadWindowState()
{
    QString configPath = QDir::tempPath() + QStringLiteral("/study_qt_demo_array2d_config.ini");
    if (!QFile::exists(configPath)) {
        return;
    }
    QSettings s(configPath, QSettings::IniFormat);
    s.beginGroup(QStringLiteral("MainWindow"));
    if (s.contains(QStringLiteral("geometry"))) {
        restoreGeometry(s.value(QStringLiteral("geometry")).toByteArray());
    }
    if (s.contains(QStringLiteral("splitter"))) {
        m_splitter->restoreState(s.value(QStringLiteral("splitter")).toByteArray());
    }
    s.endGroup();
}
