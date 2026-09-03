#pragma once

#include <QWidget>
#include <QPointF>
#include <QColor>
#include <QImage>
#include <memory>
#include "grid_data_model.h"
#include "colourmap.h"
#include "minimap_widget.h"

class VisualizerWidget : public QWidget {
    Q_OBJECT

public:
    explicit VisualizerWidget(QWidget *parent = nullptr);
    ~VisualizerWidget() override = default;

    void setModel(GridDataModel *model);

    // 坐标与视口换算
    QPointF gridToScreen(double col, double row) const;
    QPointF screenToGrid(const QPointF &screenPos) const;

    double zoom() const { return m_zoom; }
    QPointF offset() const { return m_offset; }

public slots:
    // 色带与过滤设置
    void setColourMap(const std::shared_ptr<ColourMap> &map);
    void setFilterRange(double minNorm, double maxNorm, bool enabled);

    // 边框外观设置
    void setBorderEnabled(bool enabled);
    void setBorderColor(const QColor &color);
    void setBorderWidth(int width);

    // 动效与增强显示
    void setHoverEffectEnabled(bool enabled);
    void setHoverRadius(int radiusPx);
    void setElevationIntensity(double intensity);
    void setShowValues(bool show);
    void setShowMinimap(bool show);

    // 视图导航
    void resetView();
    void fitToWindow();
    void centerOnNormalized(double normX, double normY);

signals:
    void cellHovered(int row, int col, double rawValue, double normValue);
    void zoomChanged(double zoomFactor);
    void viewportChanged(const QRectF &visibleGridRect);
    void renderStatsUpdated(double frameTimeMs, int visibleCells);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    void updateFullImageCache();
    void updateMinimapGeometry();
    void syncMinimapViewport();

    GridDataModel *m_model{nullptr};
    std::shared_ptr<ColourMap> m_colourMap;
    std::vector<QRgb> m_lut256;
    QImage m_fullImageCache;

    // 视口与缩放平移参数
    double m_zoom{15.0}; // 每个格子的屏幕像素尺寸（Pixels per cell）
    QPointF m_offset{50.0, 50.0}; // 屏幕画布偏移
    bool m_isPanning{false};
    QPoint m_lastMousePos;

    // 颜色过滤
    bool m_filterEnabled{false};
    double m_filterMinNorm{0.0};
    double m_filterMaxNorm{1.0};

    // 边框控制
    bool m_showBorder{true};
    QColor m_borderColor{QColor(30, 30, 30, 180)};
    int m_borderWidth{1};

    // 鼠标圆域浮起动效
    bool m_hoverEffectEnabled{true};
    int m_hoverRadius{100}; // 像素半径
    double m_elevationIntensity{10.0}; // 浮起像素高度
    QPoint m_currentCursorPos{-1000, -1000};
    bool m_mouseInWidget{false};

    // 格点悬浮信息展示
    int m_hoveredRow{-1};
    int m_hoveredCol{-1};
    float m_hoveredRawVal{0.0f};
    float m_hoveredNormVal{0.0f};
    bool m_initialFitDone{false};

    // 格内数值显示
    bool m_showValues{true};

    // 鹰眼缩略图
    MinimapWidget *m_minimap{nullptr};
    bool m_showMinimap{true};

    void drawHoverInfoCard(QPainter &painter);
};
