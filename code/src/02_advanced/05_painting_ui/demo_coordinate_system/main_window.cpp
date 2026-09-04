#include "main_window.h"
#include "ui_main_window.h"
#include "transform_canvas.h"

#include <QLabel>
#include <QtMath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupUiCustom();
    connectSignals();
    syncUiFromCanvas();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupUiCustom() {
    // 实例化自绘画布并嵌入中心右侧容器
    m_canvas = new TransformCanvas(this);
    ui->canvasVLayout->addWidget(m_canvas);

    // 配置主分割条：左侧面板可拉伸、最大宽度 700px、最小 280px
    ui->leftScrollArea->setMaximumWidth(700);
    ui->leftScrollArea->setMinimumWidth(280);

    ui->mainSplitter->setStretchFactor(0, 0);
    ui->mainSplitter->setStretchFactor(1, 1);
    ui->mainSplitter->setSizes({330, 910});
    ui->mainSplitter->setChildrenCollapsible(false);

    // 状态栏常驻标签
    m_statusLabel = new QLabel(this);
    m_statusLabel->setStyleSheet(QStringLiteral("color: #334155; font-family: Consolas, 'Courier New'; font-size: 12px; padding: 2px 8px;"));
    ui->statusBar->addPermanentWidget(m_statusLabel, 1);

    onCanvasStatusChanged(m_canvas->statusSummary());
}

void MainWindow::connectSignals() {
    // 图元与包围盒
    connect(ui->comboPrimitive, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onPrimitiveIndexChanged);
    connect(ui->checkBoundingBox, &QCheckBox::toggled,
            this, &MainWindow::onBoundingBoxToggled);

    // 旋转与尺寸微调
    connect(ui->spinRotation, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onRotationSpinChanged);
    connect(ui->sliderRotation, &QSlider::valueChanged,
            this, &MainWindow::onRotationSliderChanged);

    connect(ui->spinWidth, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onSizeChanged);
    connect(ui->spinHeight, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onSizeChanged);
    connect(ui->spinCenterX, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onCenterChanged);
    connect(ui->spinCenterY, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onCenterChanged);

    // 重置按钮
    connect(ui->btnFitInView, &QPushButton::clicked,
            this, &MainWindow::onFitInViewClicked);
    connect(ui->btnFitWidth, &QPushButton::clicked,
            this, &MainWindow::onFitWidthClicked);
    connect(ui->btnFitHeight, &QPushButton::clicked,
            this, &MainWindow::onFitHeightClicked);
    connect(ui->btnResetRotation, &QPushButton::clicked,
            this, &MainWindow::onResetRotationClicked);

    // 视口
    connect(ui->checkEnableViewport, &QCheckBox::toggled,
            this, &MainWindow::onViewportEnabledToggled);
    connect(ui->checkViewportBorder, &QCheckBox::toggled,
            this, &MainWindow::onViewportBorderToggled);

    // 画布联动
    connect(m_canvas, &TransformCanvas::statusChanged,
            this, &MainWindow::onCanvasStatusChanged);
    connect(m_canvas, &TransformCanvas::transformChanged,
            this, &MainWindow::onCanvasTransformChanged);
    connect(m_canvas, &TransformCanvas::viewportChanged,
            this, &MainWindow::onCanvasViewportChanged);
}

void MainWindow::syncUiFromCanvas() {
    if (!m_canvas) return;
    m_updatingFromCanvas = true;

    // 1. 同步旋转
    qreal rot = m_canvas->rotation();
    ui->spinRotation->setValue(rot);
    int sliderVal = static_cast<int>(std::round(rot)) % 360;
    if (sliderVal < 0) sliderVal += 360;
    ui->sliderRotation->setValue(sliderVal);

    // 2. 同步尺寸
    QSizeF sz = m_canvas->primitiveSize();
    ui->spinWidth->setValue(sz.width());
    ui->spinHeight->setValue(sz.height());

    // 3. 同步中心
    QPointF c = m_canvas->primitiveCenter();
    ui->spinCenterX->setValue(c.x());
    ui->spinCenterY->setValue(c.y());

    // 4. 同步矩阵展示
    QTransform t = m_canvas->transformBox().transform();
    QString matrixStr = QStringLiteral(
        "[ M11: %1  M12: %2 ]\n"
        "[ M21: %3  M22: %4 ]\n"
        "[ Dx : %5   Dy : %6  ]")
        .arg(t.m11(), 6, 'f', 2)
        .arg(t.m12(), 6, 'f', 2)
        .arg(t.m21(), 6, 'f', 2)
        .arg(t.m22(), 6, 'f', 2)
        .arg(t.dx(), 6, 'f', 1)
        .arg(t.dy(), 6, 'f', 1);
    ui->labelMatrixValues->setText(matrixStr);

    m_updatingFromCanvas = false;
}

void MainWindow::onPrimitiveIndexChanged(int index) {
    if (!m_canvas || m_updatingFromCanvas) return;
    m_canvas->setPrimitiveType(static_cast<PrimitiveType>(index));
}

void MainWindow::onBoundingBoxToggled(bool checked) {
    if (!m_canvas || m_updatingFromCanvas) return;
    m_canvas->setBoundingBoxVisible(checked);
}

void MainWindow::onRotationSpinChanged(double val) {
    if (!m_canvas || m_updatingFromCanvas) return;
    m_updatingFromCanvas = true;
    int sVal = static_cast<int>(std::round(val)) % 360;
    if (sVal < 0) sVal += 360;
    ui->sliderRotation->setValue(sVal);
    m_updatingFromCanvas = false;

    m_canvas->setRotation(val);
}

void MainWindow::onRotationSliderChanged(int val) {
    if (!m_canvas || m_updatingFromCanvas) return;
    m_updatingFromCanvas = true;
    ui->spinRotation->setValue(val);
    m_updatingFromCanvas = false;

    m_canvas->setRotation(val);
}

void MainWindow::onSizeChanged() {
    if (!m_canvas || m_updatingFromCanvas) return;
    m_canvas->setPrimitiveSize(QSizeF(ui->spinWidth->value(), ui->spinHeight->value()));
}

void MainWindow::onCenterChanged() {
    if (!m_canvas || m_updatingFromCanvas) return;
    m_canvas->setPrimitiveCenter(QPointF(ui->spinCenterX->value(), ui->spinCenterY->value()));
}

void MainWindow::onFitInViewClicked() {
    if (!m_canvas) return;
    m_canvas->resetFitInView();
}

void MainWindow::onFitWidthClicked() {
    if (!m_canvas) return;
    m_canvas->resetFitWidthCenter();
}

void MainWindow::onFitHeightClicked() {
    if (!m_canvas) return;
    m_canvas->resetFitHeightCenter();
}

void MainWindow::onResetRotationClicked() {
    if (!m_canvas) return;
    m_canvas->resetRotationOnly();
}

void MainWindow::onViewportEnabledToggled(bool checked) {
    if (!m_canvas) return;
    m_canvas->setViewportEnabled(checked);
}

void MainWindow::onViewportBorderToggled(bool checked) {
    if (!m_canvas) return;
    m_canvas->setViewportBorderVisible(checked);
}

void MainWindow::onCanvasStatusChanged(const QString &text) {
    if (m_statusLabel) {
        m_statusLabel->setText(text);
    }
}

void MainWindow::onCanvasTransformChanged() {
    syncUiFromCanvas();
}

void MainWindow::onCanvasViewportChanged() {
    // 更新视口提示
    if (!m_canvas) return;
    const auto &vp = m_canvas->customViewport();
    if (vp.isEnabled()) {
        ui->labelVpInfo->setText(QStringLiteral("外部视口开启中：位置 (%1, %2) 尺寸 %3 x %4。图元绘制与交互被严格限制在视口内。")
                                     .arg(qRound(vp.rect().x()))
                                     .arg(qRound(vp.rect().y()))
                                     .arg(qRound(vp.rect().width()))
                                     .arg(qRound(vp.rect().height())));
    } else {
        ui->labelVpInfo->setText(QStringLiteral("外部视口未开启，全画布绘制。勾选上方复选框可开启视口裁剪沙箱。"));
    }
}
