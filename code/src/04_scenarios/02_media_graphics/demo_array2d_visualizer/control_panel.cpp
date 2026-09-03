#include "control_panel.h"
#include "colourmap_manager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QColorDialog>
#include <QScrollArea>
#include <QIcon>
#include <QPixmap>

ControlPanel::ControlPanel(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    populateColourMaps();
}

void ControlPanel::setupUi()
{
    auto *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);

    auto *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    auto *contentWidget = new QWidget(scrollArea);
    auto *layout = new QVBoxLayout(contentWidget);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(10);

    // 1. 数据与连续噪声引擎
    createNoiseControls(layout);
    // 2. 色带与数值过滤
    createColourMapControls(layout);
    // 3. 网格外观与边框
    createAppearanceControls(layout);
    // 4. 动效与缩略图
    createAnimationControls(layout);
    // 5. 视角导航
    createNavigationControls(layout);

    layout->addStretch();
    scrollArea->setWidget(contentWidget);
    outerLayout->addWidget(scrollArea);

    setMinimumWidth(320);
    setMaximumWidth(700);
}

void ControlPanel::createNoiseControls(QVBoxLayout *layout)
{
    auto *group = new QGroupBox(QStringLiteral("1. 数据与连续过程噪声生成"), this);
    auto *form = new QFormLayout(group);
    form->setLabelAlignment(Qt::AlignRight);

    m_noiseTypeCombo = new QComboBox(group);
    m_noiseTypeCombo->addItem(QStringLiteral("Perlin 连续梯度噪声"), static_cast<int>(NoiseType::Perlin));
    m_noiseTypeCombo->addItem(QStringLiteral("fBm 多阶分形布朗运动"), static_cast<int>(NoiseType::Fbm));
    m_noiseTypeCombo->addItem(QStringLiteral("Worley 细胞晶格距离噪声"), static_cast<int>(NoiseType::WorleyCellular));
    m_noiseTypeCombo->addItem(QStringLiteral("调和正余弦波干涉场"), static_cast<int>(NoiseType::HarmonicWaves));
    m_noiseTypeCombo->addItem(QStringLiteral("高斯热源羽流扩散"), static_cast<int>(NoiseType::ThermalPlume));
    form->addRow(QStringLiteral("噪声函数:"), m_noiseTypeCombo);

    auto *dimLayout = new QHBoxLayout();
    m_rowsSpin = new QSpinBox(group);
    m_rowsSpin->setRange(10, 2000);
    m_rowsSpin->setValue(150);
    m_rowsSpin->setSingleStep(20);
    m_colsSpin = new QSpinBox(group);
    m_colsSpin->setRange(10, 2000);
    m_colsSpin->setValue(150);
    m_colsSpin->setSingleStep(20);
    dimLayout->addWidget(new QLabel(QStringLiteral("行:"), group));
    dimLayout->addWidget(m_rowsSpin);
    dimLayout->addWidget(new QLabel(QStringLiteral("列:"), group));
    dimLayout->addWidget(m_colsSpin);
    form->addRow(QStringLiteral("矩阵规模:"), dimLayout);

    auto *xRangeLayout = new QHBoxLayout();
    m_xMinSpin = new QDoubleSpinBox(group);
    m_xMinSpin->setRange(-1000.0, 1000.0);
    m_xMinSpin->setValue(-5.0);
    m_xMaxSpin = new QDoubleSpinBox(group);
    m_xMaxSpin->setRange(-1000.0, 1000.0);
    m_xMaxSpin->setValue(5.0);
    xRangeLayout->addWidget(m_xMinSpin);
    xRangeLayout->addWidget(new QLabel(QStringLiteral("~"), group));
    xRangeLayout->addWidget(m_xMaxSpin);
    form->addRow(QStringLiteral("X 坐标区间:"), xRangeLayout);

    auto *yRangeLayout = new QHBoxLayout();
    m_yMinSpin = new QDoubleSpinBox(group);
    m_yMinSpin->setRange(-1000.0, 1000.0);
    m_yMinSpin->setValue(-5.0);
    m_yMaxSpin = new QDoubleSpinBox(group);
    m_yMaxSpin->setRange(-1000.0, 1000.0);
    m_yMaxSpin->setValue(5.0);
    yRangeLayout->addWidget(m_yMinSpin);
    yRangeLayout->addWidget(new QLabel(QStringLiteral("~"), group));
    yRangeLayout->addWidget(m_yMaxSpin);
    form->addRow(QStringLiteral("Y 坐标区间:"), yRangeLayout);

    m_seedSpin = new QSpinBox(group);
    m_seedSpin->setRange(0, 999999);
    m_seedSpin->setValue(1337);
    form->addRow(QStringLiteral("随机种子:"), m_seedSpin);

    m_freqSpin = new QDoubleSpinBox(group);
    m_freqSpin->setRange(0.01, 20.0);
    m_freqSpin->setValue(1.0);
    m_freqSpin->setSingleStep(0.2);
    form->addRow(QStringLiteral("空间频率:"), m_freqSpin);

    m_octavesSpin = new QSpinBox(group);
    m_octavesSpin->setRange(1, 8);
    m_octavesSpin->setValue(4);
    form->addRow(QStringLiteral("分形阶数 (fBm):"), m_octavesSpin);

    m_generateBtn = new QPushButton(QStringLiteral("🚀 生成 / 重新计算数据"), group);
    m_generateBtn->setStyleSheet(QStringLiteral("QPushButton { background-color: #2b78e4; color: white; font-weight: bold; padding: 6px; border-radius: 4px; } QPushButton:hover { background-color: #3b88f4; }"));
    form->addRow(m_generateBtn);

    m_statusLabel = new QLabel(QStringLiteral("状态: 待生成"), group);
    m_statusLabel->setStyleSheet(QStringLiteral("color: #888888;"));
    form->addRow(m_statusLabel);

    connect(m_generateBtn, &QPushButton::clicked, this, [this]() {
        emit generateRequested(currentParams());
    });

    layout->addWidget(group);
}

void ControlPanel::createColourMapControls(QVBoxLayout *layout)
{
    auto *group = new QGroupBox(QStringLiteral("2. 科学色带与数值过滤"), this);
    auto *form = new QFormLayout(group);
    form->setLabelAlignment(Qt::AlignRight);

    m_colourMapCombo = new QComboBox(group);
    m_colourMapCombo->setIconSize(QSize(64, 16));
    form->addRow(QStringLiteral("选择色带:"), m_colourMapCombo);

    connect(m_colourMapCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int /*index*/) {
        QString mapName = m_colourMapCombo->currentData().toString();
        auto map = ColourMapManager::instance().getMap(mapName);
        if (map) {
            emit colourMapChanged(map);
        }
    });

    m_filterCheck = new QCheckBox(QStringLiteral("启用数值/色彩过滤"), group);
    m_filterCheck->setChecked(false);
    form->addRow(m_filterCheck);

    auto *filterRangeLayout = new QHBoxLayout();
    m_filterMinSpin = new QDoubleSpinBox(group);
    m_filterMinSpin->setRange(0.0, 1.0);
    m_filterMinSpin->setValue(0.2);
    m_filterMinSpin->setSingleStep(0.05);

    m_filterMaxSpin = new QDoubleSpinBox(group);
    m_filterMaxSpin->setRange(0.0, 1.0);
    m_filterMaxSpin->setValue(0.8);
    m_filterMaxSpin->setSingleStep(0.05);

    filterRangeLayout->addWidget(m_filterMinSpin);
    filterRangeLayout->addWidget(new QLabel(QStringLiteral("~"), group));
    filterRangeLayout->addWidget(m_filterMaxSpin);
    form->addRow(QStringLiteral("归一化区间:"), filterRangeLayout);

    auto onFilterChanged = [this]() {
        emit filterChanged(m_filterMinSpin->value(), m_filterMaxSpin->value(), m_filterCheck->isChecked());
    };
    connect(m_filterCheck, &QCheckBox::toggled, this, onFilterChanged);
    connect(m_filterMinSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, onFilterChanged);
    connect(m_filterMaxSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, onFilterChanged);

    layout->addWidget(group);
}

void ControlPanel::createAppearanceControls(QVBoxLayout *layout)
{
    auto *group = new QGroupBox(QStringLiteral("3. 网格外观与边框"), this);
    auto *form = new QFormLayout(group);
    form->setLabelAlignment(Qt::AlignRight);

    m_borderCheck = new QCheckBox(QStringLiteral("显示单元格边框"), group);
    m_borderCheck->setChecked(true);
    form->addRow(m_borderCheck);
    connect(m_borderCheck, &QCheckBox::toggled, this, &ControlPanel::borderToggled);

    auto *colorLayout = new QHBoxLayout();
    m_borderColorBtn = new QPushButton(QStringLiteral("选择颜色"), group);
    m_borderColorBtn->setStyleSheet(QStringLiteral("background-color: rgb(30,30,30); color: white;"));
    colorLayout->addWidget(m_borderColorBtn);

    m_borderWidthSpin = new QSpinBox(group);
    m_borderWidthSpin->setRange(1, 4);
    m_borderWidthSpin->setValue(1);
    colorLayout->addWidget(new QLabel(QStringLiteral("宽:"), group));
    colorLayout->addWidget(m_borderWidthSpin);
    form->addRow(QStringLiteral("边框样式:"), colorLayout);

    connect(m_borderColorBtn, &QPushButton::clicked, this, [this]() {
        QColor color = QColorDialog::getColor(m_currentBorderColor, this, QStringLiteral("选择单元格边框颜色"));
        if (color.isValid()) {
            m_currentBorderColor = color;
            m_borderColorBtn->setStyleSheet(QStringLiteral("background-color: %1; color: %2;")
                .arg(color.name())
                .arg(color.lightness() > 128 ? "black" : "white"));
            emit borderColorChanged(color);
        }
    });
    connect(m_borderWidthSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &ControlPanel::borderWidthChanged);

    m_showValuesCheck = new QCheckBox(QStringLiteral("在格内显示数值 (需放大 >= 36px)"), group);
    m_showValuesCheck->setChecked(true);
    form->addRow(m_showValuesCheck);
    connect(m_showValuesCheck, &QCheckBox::toggled, this, &ControlPanel::showValuesToggled);

    layout->addWidget(group);
}

void ControlPanel::createAnimationControls(QVBoxLayout *layout)
{
    auto *group = new QGroupBox(QStringLiteral("4. 交互微动效与鹰眼地图"), this);
    auto *form = new QFormLayout(group);
    form->setLabelAlignment(Qt::AlignRight);

    m_hoverEffectCheck = new QCheckBox(QStringLiteral("鼠标圆域浮起动效 (3D立体凸出)"), group);
    m_hoverEffectCheck->setChecked(true);
    form->addRow(m_hoverEffectCheck);
    connect(m_hoverEffectCheck, &QCheckBox::toggled, this, &ControlPanel::hoverEffectToggled);

    auto *radiusLayout = new QHBoxLayout();
    m_hoverRadiusSlider = new QSlider(Qt::Horizontal, group);
    m_hoverRadiusSlider->setRange(30, 250);
    m_hoverRadiusSlider->setValue(100);
    m_hoverRadiusLabel = new QLabel(QStringLiteral("100px"), group);
    radiusLayout->addWidget(m_hoverRadiusSlider);
    radiusLayout->addWidget(m_hoverRadiusLabel);
    form->addRow(QStringLiteral("感应半径:"), radiusLayout);

    connect(m_hoverRadiusSlider, &QSlider::valueChanged, this, [this](int val) {
        m_hoverRadiusLabel->setText(QStringLiteral("%1px").arg(val));
        emit hoverRadiusChanged(val);
    });

    auto *elevationLayout = new QHBoxLayout();
    m_elevationSlider = new QSlider(Qt::Horizontal, group);
    m_elevationSlider->setRange(1, 30);
    m_elevationSlider->setValue(12);
    m_elevationLabel = new QLabel(QStringLiteral("12px"), group);
    elevationLayout->addWidget(m_elevationSlider);
    elevationLayout->addWidget(m_elevationLabel);
    form->addRow(QStringLiteral("浮起高度:"), elevationLayout);

    connect(m_elevationSlider, &QSlider::valueChanged, this, [this](int val) {
        m_elevationLabel->setText(QStringLiteral("%1px").arg(val));
        emit elevationIntensityChanged(static_cast<double>(val));
    });

    m_minimapCheck = new QCheckBox(QStringLiteral("显示鹰眼缩略图小地图"), group);
    m_minimapCheck->setChecked(true);
    form->addRow(m_minimapCheck);
    connect(m_minimapCheck, &QCheckBox::toggled, this, &ControlPanel::showMinimapToggled);

    layout->addWidget(group);
}

void ControlPanel::createNavigationControls(QVBoxLayout *layout)
{
    auto *group = new QGroupBox(QStringLiteral("5. 视口操作与定位"), this);
    auto *btnLayout = new QHBoxLayout(group);

    m_fitBtn = new QPushButton(QStringLiteral("📐 全局适应"), group);
    m_resetBtn = new QPushButton(QStringLiteral("🔄 重置 100%"), group);

    btnLayout->addWidget(m_fitBtn);
    btnLayout->addWidget(m_resetBtn);

    connect(m_fitBtn, &QPushButton::clicked, this, &ControlPanel::fitWindowRequested);
    connect(m_resetBtn, &QPushButton::clicked, this, &ControlPanel::resetViewRequested);

    layout->addWidget(group);
}

GridParams ControlPanel::currentParams() const
{
    GridParams p;
    p.noiseType = static_cast<NoiseType>(m_noiseTypeCombo->currentData().toInt());
    p.rows = m_rowsSpin->value();
    p.cols = m_colsSpin->value();
    p.xMin = m_xMinSpin->value();
    p.xMax = m_xMaxSpin->value();
    p.yMin = m_yMinSpin->value();
    p.yMax = m_yMaxSpin->value();
    p.seed = m_seedSpin->value();
    p.frequency = static_cast<float>(m_freqSpin->value());
    p.octaves = m_octavesSpin->value();
    return p;
}

void ControlPanel::populateColourMaps()
{
    m_colourMapCombo->clear();
    QStringList names = ColourMapManager::instance().availableMapNames();

    for (const QString &name : names) {
        auto map = ColourMapManager::instance().getMap(name);
        if (map) {
            QImage bar = map->generatePreviewBar(80, 16, true);
            QIcon icon(QPixmap::fromImage(bar));
            m_colourMapCombo->addItem(icon, name, name);
        }
    }

    int batlowIdx = m_colourMapCombo->findData(QStringLiteral("batlow"));
    if (batlowIdx >= 0) {
        m_colourMapCombo->setCurrentIndex(batlowIdx);
    }
}

void ControlPanel::onGenerationStarted()
{
    m_generateBtn->setEnabled(false);
    m_statusLabel->setText(QStringLiteral("状态: 计算中..."));
}

void ControlPanel::onGenerationFinished()
{
    m_generateBtn->setEnabled(true);
}

void ControlPanel::updateDataStats(float minVal, float maxVal, int totalCells)
{
    m_statusLabel->setText(QStringLiteral("总格数: %1 | 范围: [%2, %3]")
                           .arg(totalCells)
                           .arg(minVal, 0, 'f', 2)
                           .arg(maxVal, 0, 'f', 2));
}
