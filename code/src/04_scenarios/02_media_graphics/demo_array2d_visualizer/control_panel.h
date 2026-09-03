#pragma once

#include <QWidget>
#include <QColor>
#include <memory>
#include "grid_data_model.h"
#include "colourmap.h"

class QComboBox;
class QSpinBox;
class QDoubleSpinBox;
class QCheckBox;
class QPushButton;
class QSlider;
class QLabel;
class QVBoxLayout;

class ControlPanel : public QWidget {
    Q_OBJECT

public:
    explicit ControlPanel(QWidget *parent = nullptr);

    // 获取当前配置的网格与噪声参数
    GridParams currentParams() const;

    // 填充色带列表
    void populateColourMaps();

signals:
    void generateRequested(const GridParams &params);
    void colourMapChanged(const std::shared_ptr<ColourMap> &map);
    void filterChanged(double minNorm, double maxNorm, bool enabled);
    void borderToggled(bool enabled);
    void borderColorChanged(const QColor &color);
    void borderWidthChanged(int width);
    void hoverEffectToggled(bool enabled);
    void hoverRadiusChanged(int radius);
    void elevationIntensityChanged(double intensity);
    void showValuesToggled(bool show);
    void showMinimapToggled(bool show);
    void resetViewRequested();
    void fitWindowRequested();

public slots:
    void onGenerationStarted();
    void onGenerationFinished();
    void updateDataStats(float minVal, float maxVal, int totalCells);

private:
    void setupUi();
    void createNoiseControls(QVBoxLayout *layout);
    void createColourMapControls(QVBoxLayout *layout);
    void createAppearanceControls(QVBoxLayout *layout);
    void createAnimationControls(QVBoxLayout *layout);
    void createNavigationControls(QVBoxLayout *layout);

    // 数据与噪声控件
    QComboBox *m_noiseTypeCombo{nullptr};
    QSpinBox *m_rowsSpin{nullptr};
    QSpinBox *m_colsSpin{nullptr};
    QDoubleSpinBox *m_xMinSpin{nullptr};
    QDoubleSpinBox *m_xMaxSpin{nullptr};
    QDoubleSpinBox *m_yMinSpin{nullptr};
    QDoubleSpinBox *m_yMaxSpin{nullptr};
    QSpinBox *m_seedSpin{nullptr};
    QDoubleSpinBox *m_freqSpin{nullptr};
    QSpinBox *m_octavesSpin{nullptr};
    QPushButton *m_generateBtn{nullptr};
    QLabel *m_statusLabel{nullptr};

    // 色带与过滤控件
    QComboBox *m_colourMapCombo{nullptr};
    QCheckBox *m_filterCheck{nullptr};
    QDoubleSpinBox *m_filterMinSpin{nullptr};
    QDoubleSpinBox *m_filterMaxSpin{nullptr};

    // 外观与边框控件
    QCheckBox *m_borderCheck{nullptr};
    QPushButton *m_borderColorBtn{nullptr};
    QSpinBox *m_borderWidthSpin{nullptr};
    QColor m_currentBorderColor{QColor(30, 30, 30, 180)};
    QCheckBox *m_showValuesCheck{nullptr};

    // 动效与缩略图控件
    QCheckBox *m_hoverEffectCheck{nullptr};
    QSlider *m_hoverRadiusSlider{nullptr};
    QLabel *m_hoverRadiusLabel{nullptr};
    QSlider *m_elevationSlider{nullptr};
    QLabel *m_elevationLabel{nullptr};
    QCheckBox *m_minimapCheck{nullptr};

    // 视角控件
    QPushButton *m_fitBtn{nullptr};
    QPushButton *m_resetBtn{nullptr};
};
