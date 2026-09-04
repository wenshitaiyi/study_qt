#include "visualizer_widget.h"
#include "colourmap_manager.h"

#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QElapsedTimer>
#include <cmath>
#include <algorithm>

VisualizerWidget::VisualizerWidget(QWidget *parent)
    : QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    setAttribute(Qt::WA_OpaquePaintEvent, true);

    // 默认回退色带
    m_colourMap = ColourMapManager::instance().defaultMap();
    if (m_colourMap) {
        m_lut256 = m_colourMap->generateLut256();
    }

    // 创建右下角小地图
    m_minimap = new MinimapWidget(this);
    connect(m_minimap, &MinimapWidget::requestCenterOn, this, &VisualizerWidget::centerOnNormalized);
    m_minimap->show();
}

void VisualizerWidget::setModel(GridDataModel *model)
{
    if (m_model) {
        disconnect(m_model, nullptr, this, nullptr);
    }
    m_model = model;
    if (m_model) {
        connect(m_model, &GridDataModel::dataChanged, this, [this]() {
            updateFullImageCache();
            update();
        });
        updateFullImageCache();
    }
    update();
}

void VisualizerWidget::setColourMap(const std::shared_ptr<ColourMap> &map)
{
    if (map && map->isValid()) {
        m_colourMap = map;
        m_lut256 = m_colourMap->generateLut256();
        updateFullImageCache();
        update();
    }
}

void VisualizerWidget::setFilterRange(double minNorm, double maxNorm, bool enabled)
{
    m_filterMinNorm = std::clamp(minNorm, 0.0, 1.0);
    m_filterMaxNorm = std::clamp(maxNorm, 0.0, 1.0);
    m_filterEnabled = enabled;
    updateFullImageCache();
    update();
}

void VisualizerWidget::setBorderEnabled(bool enabled)
{
    m_showBorder = enabled;
    update();
}

void VisualizerWidget::setBorderColor(const QColor &color)
{
    m_borderColor = color;
    update();
}

void VisualizerWidget::setBorderWidth(int width)
{
    m_borderWidth = std::max(1, width);
    update();
}

void VisualizerWidget::setHoverEffectEnabled(bool enabled)
{
    m_hoverEffectEnabled = enabled;
    update();
}

void VisualizerWidget::setHoverRadius(int radiusPx)
{
    m_hoverRadius = std::max(10, radiusPx);
    update();
}

void VisualizerWidget::setElevationIntensity(double intensity)
{
    m_elevationIntensity = std::clamp(intensity, 0.0, 40.0);
    update();
}

void VisualizerWidget::setShowValues(bool show)
{
    m_showValues = show;
    update();
}

void VisualizerWidget::setShowMinimap(bool show)
{
    m_showMinimap = show;
    m_minimap->setVisible(show);
    update();
}

void VisualizerWidget::setHoverHighlightEnabled(bool enabled)
{
    m_hoverHighlightEnabled = enabled;
    update();
}

void VisualizerWidget::setHoverHighlightColor(const QColor &color)
{
    m_hoverHighlightColor = color;
    update();
}

void VisualizerWidget::setHoverHighlightWidth(int width)
{
    m_hoverHighlightWidth = std::clamp(width, 1, 6);
    update();
}

void VisualizerWidget::setShowHoverInfoCard(bool show)
{
    m_showHoverInfoCard = show;
    update();
}

QPointF VisualizerWidget::gridToScreen(double col, double row) const
{
    return QPointF(m_offset.x() + col * m_zoom, m_offset.y() + row * m_zoom);
}

QPointF VisualizerWidget::screenToGrid(const QPointF &screenPos) const
{
    if (std::abs(m_zoom) < 1e-6) {
        return QPointF(0, 0);
    }
    return QPointF((screenPos.x() - m_offset.x()) / m_zoom,
                   (screenPos.y() - m_offset.y()) / m_zoom);
}

void VisualizerWidget::resetView()
{
    m_zoom = 15.0;
    m_offset = QPointF(50.0, 50.0);
    emit zoomChanged(m_zoom);
    syncMinimapViewport();
    update();
}

void VisualizerWidget::fitToWindow()
{
    if (!m_model || m_model->rows() <= 0 || m_model->cols() <= 0) {
        return;
    }

    double margin = 30.0;
    double availW = std::max(10.0, width() - margin * 2.0);
    double availH = std::max(10.0, height() - margin * 2.0);

    double zoomX = availW / m_model->cols();
    double zoomY = availH / m_model->rows();
    m_zoom = std::min(zoomX, zoomY);

    double totalW = m_model->cols() * m_zoom;
    double totalH = m_model->rows() * m_zoom;

    m_offset = QPointF((width() - totalW) * 0.5, (height() - totalH) * 0.5);

    emit zoomChanged(m_zoom);
    syncMinimapViewport();
    update();
}

void VisualizerWidget::centerOnNormalized(double normX, double normY)
{
    if (!m_model || m_model->rows() <= 0 || m_model->cols() <= 0) {
        return;
    }

    double targetCol = normX * m_model->cols();
    double targetRow = normY * m_model->rows();

    m_offset.setX(width() * 0.5 - targetCol * m_zoom);
    m_offset.setY(height() * 0.5 - targetRow * m_zoom);

    syncMinimapViewport();
    update();
}

void VisualizerWidget::updateFullImageCache()
{
    if (!m_model || m_model->rows() <= 0 || m_model->cols() <= 0) {
        m_fullImageCache = QImage();
        m_minimap->setThumbnail(QImage());
        return;
    }

    int rows = m_model->rows();
    int cols = m_model->cols();

    if (m_lut256.empty()) {
        m_lut256.resize(256, qRgba(128, 128, 128, 255));
    }

    QImage img(cols, rows, QImage::Format_ARGB32_Premultiplied);

    for (int r = 0; r < rows; ++r) {
        QRgb *line = reinterpret_cast<QRgb *>(img.scanLine(r));
        for (int c = 0; c < cols; ++c) {
            float normVal = m_model->normalizedValueAt(r, c);

            if (m_filterEnabled && (normVal < m_filterMinNorm || normVal > m_filterMaxNorm)) {
                // 过滤区间外淡化
                line[c] = qRgba(35, 35, 35, 80);
            } else {
                int lutIdx = std::clamp(static_cast<int>(normVal * 255.0f), 0, 255);
                line[c] = m_lut256[lutIdx];
            }
        }
    }

    m_fullImageCache = img;
    m_minimap->setThumbnail(m_fullImageCache);
}

void VisualizerWidget::updateMinimapGeometry()
{
    if (m_minimap) {
        int margin = 16;
        int mw = m_minimap->width();
        int mh = m_minimap->height();
        m_minimap->move(width() - mw - margin, height() - mh - margin);
    }
}

void VisualizerWidget::syncMinimapViewport()
{
    if (!m_model || m_model->rows() <= 0 || m_model->cols() <= 0 || m_zoom <= 0) {
        m_minimap->setViewportRect(QRectF());
        return;
    }

    QPointF topLeftGrid = screenToGrid(QPointF(0, 0));
    QPointF bottomRightGrid = screenToGrid(QPointF(width(), height()));

    double normLeft = topLeftGrid.x() / m_model->cols();
    double normTop = topLeftGrid.y() / m_model->rows();
    double normW = (bottomRightGrid.x() - topLeftGrid.x()) / m_model->cols();
    double normH = (bottomRightGrid.y() - topLeftGrid.y()) / m_model->rows();

    QRectF normRect(normLeft, normTop, normW, normH);
    m_minimap->setViewportRect(normRect);
    emit viewportChanged(normRect);
}

void VisualizerWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updateMinimapGeometry();
    if (!m_initialFitDone && m_model && m_model->rows() > 0 && width() > 100 && height() > 100) {
        m_initialFitDone = true;
        fitToWindow();
    } else {
        syncMinimapViewport();
    }
}

void VisualizerWidget::paintEvent(QPaintEvent * /*event*/)
{
    QElapsedTimer timer;
    timer.start();

    QPainter painter(this);

    // 深色网格背景画布
    painter.fillRect(rect(), QColor(26, 28, 32));

    if (!m_model || m_model->rows() <= 0 || m_model->cols() <= 0) {
        painter.setPen(QColor(180, 180, 180));
        painter.drawText(rect(), Qt::AlignCenter, QStringLiteral("暂无数据，请在左侧面板生成数据"));
        return;
    }

    int rows = m_model->rows();
    int cols = m_model->cols();
    int visibleCellsCount = 0;

    // 分级绘制策略（LOD）
    if (m_zoom < 3.0) {
        // 【远距离视距】：直接贴图，百万点纳秒级完成
        QRectF targetRect(m_offset.x(), m_offset.y(), cols * m_zoom, rows * m_zoom);
        painter.setRenderHint(QPainter::SmoothPixmapTransform, false);
        painter.drawImage(targetRect, m_fullImageCache);
        visibleCellsCount = rows * cols;
    } else {
        // 【中近距离视距】：启用视口裁剪（Viewport Culling）
        QPointF tl = screenToGrid(QPointF(0, 0));
        QPointF br = screenToGrid(QPointF(width(), height()));

        int cStart = std::clamp(static_cast<int>(std::floor(tl.x())), 0, cols - 1);
        int rStart = std::clamp(static_cast<int>(std::floor(tl.y())), 0, rows - 1);
        int cEnd = std::clamp(static_cast<int>(std::ceil(br.x())), 0, cols - 1);
        int rEnd = std::clamp(static_cast<int>(std::ceil(br.y())), 0, rows - 1);

        visibleCellsCount = (cEnd - cStart + 1) * (rEnd - rStart + 1);

        QPen borderPen(m_borderColor, m_borderWidth);
        bool drawBorderNow = m_showBorder && (m_zoom >= 6.0);
        bool drawValuesNow = m_showValues && (m_zoom >= 36.0);

        if (drawValuesNow) {
            QFont font = painter.font();
            int fontSize = std::clamp(static_cast<int>(m_zoom * 0.26), 8, 14);
            font.setPointSize(fontSize);
            font.setBold(true);
            painter.setFont(font);
        }

        // 第一阶段：绘制常规平铺单元格
        for (int r = rStart; r <= rEnd; ++r) {
            double sy = m_offset.y() + r * m_zoom;
            for (int c = cStart; c <= cEnd; ++c) {
                double sx = m_offset.x() + c * m_zoom;
                QRectF cellRect(sx, sy, m_zoom, m_zoom);

                float normVal = m_model->normalizedValueAt(r, c);
                QRgb rgb;
                bool isFiltered = m_filterEnabled && (normVal < m_filterMinNorm || normVal > m_filterMaxNorm);
                if (isFiltered) {
                    rgb = qRgba(35, 35, 35, 70);
                } else {
                    int lutIdx = std::clamp(static_cast<int>(normVal * 255.0f), 0, 255);
                    rgb = m_lut256[lutIdx];
                }

                painter.fillRect(cellRect, QColor(rgb));

                if (drawBorderNow) {
                    painter.setPen(borderPen);
                    painter.setBrush(Qt::NoBrush);
                    painter.drawRect(cellRect);
                }

                if (drawValuesNow && !isFiltered) {
                    float rawVal = m_model->valueAt(r, c);
                    QString text = QString::number(rawVal, 'f', 2);
                    // 自适应反色，确保清晰辨识
                    int gray = qGray(rgb);
                    painter.setPen(gray > 135 ? QColor(10, 10, 10) : QColor(245, 245, 245));
                    painter.drawText(cellRect, Qt::AlignCenter, text);
                }
            }
        }

        // 第二阶段：光标邻近圆域浮起动效（Elevation Pass）
        if (m_hoverEffectEnabled && m_mouseInWidget && m_hoverRadius > 0) {
            double mx = m_currentCursorPos.x();
            double my = m_currentCursorPos.y();
            double rPx = m_hoverRadius;

            // 计算圆域影响的格子范围
            QPointF circleTL = screenToGrid(QPointF(mx - rPx, my - rPx));
            QPointF circleBR = screenToGrid(QPointF(mx + rPx, my + rPx));

            int hcStart = std::clamp(static_cast<int>(std::floor(circleTL.x())), 0, cols - 1);
            int hrStart = std::clamp(static_cast<int>(std::floor(circleTL.y())), 0, rows - 1);
            int hcEnd = std::clamp(static_cast<int>(std::ceil(circleBR.x())), 0, cols - 1);
            int hrEnd = std::clamp(static_cast<int>(std::ceil(circleBR.y())), 0, rows - 1);

            for (int r = hrStart; r <= hrEnd; ++r) {
                double sy = m_offset.y() + r * m_zoom;
                for (int c = hcStart; c <= hcEnd; ++c) {
                    double sx = m_offset.x() + c * m_zoom;
                    double cx = sx + m_zoom * 0.5;
                    double cy = sy + m_zoom * 0.5;

                    double dist = std::sqrt((cx - mx) * (cx - mx) + (cy - my) * (cy - my));
                    if (dist < rPx) {
                        double weight = std::cos((dist / rPx) * (3.1415926535 * 0.5));
                        double elevY = -m_elevationIntensity * weight;
                        double scale = 1.0 + 0.22 * weight;
                        double cellW = m_zoom * scale;
                        double cellH = m_zoom * scale;

                        // 绘制浮起格子的阴影
                        QRectF shadowRect(cx - cellW * 0.5 + 2.0 * weight,
                                          cy - cellH * 0.5 + 4.0 * weight,
                                          cellW, cellH);
                        painter.fillRect(shadowRect, QColor(0, 0, 0, static_cast<int>(130 * weight)));

                        // 绘制浮起格身
                        QRectF elevatedRect(cx - cellW * 0.5,
                                            cy - cellH * 0.5 + elevY,
                                            cellW, cellH);

                        float normVal = m_model->normalizedValueAt(r, c);
                        int lutIdx = std::clamp(static_cast<int>(normVal * 255.0f), 0, 255);
                        QRgb rgb = m_lut256[lutIdx];

                        painter.fillRect(elevatedRect, QColor(rgb));

                        // 浮起格子的发光边框
                        QColor elevBorderColor(255, 255, 255, static_cast<int>(160 * weight));
                        painter.setPen(QPen(elevBorderColor, std::max(1.5, m_borderWidth * 1.5)));
                        painter.setBrush(Qt::NoBrush);
                        painter.drawRect(elevatedRect);

                        // 浮起格子内的数值展示
                        if (drawValuesNow) {
                            float rawVal = m_model->valueAt(r, c);
                            QString text = QString::number(rawVal, 'f', 2);
                            int gray = qGray(rgb);
                            painter.setPen(gray > 135 ? QColor(10, 10, 10) : QColor(245, 245, 245));
                            painter.drawText(elevatedRect, Qt::AlignCenter, text);
                        }
                    }
                }
            }

            // 绘制微弱的光标感知圆圈
            painter.setPen(QPen(QColor(255, 255, 255, 45), 1.2, Qt::DashLine));
            painter.setBrush(Qt::NoBrush);
            painter.drawEllipse(m_currentCursorPos, m_hoverRadius, m_hoverRadius);
        }
    }

    // 绘制当前悬停选中的格点高亮外框（与浮起动效完全对齐）
    if (m_hoverHighlightEnabled && m_mouseInWidget && m_hoveredRow >= 0 && m_hoveredCol >= 0 && m_zoom >= 2.5) {
        double sx = m_offset.x() + m_hoveredCol * m_zoom;
        double sy = m_offset.y() + m_hoveredRow * m_zoom;
        double cx = sx + m_zoom * 0.5;
        double cy = sy + m_zoom * 0.5;

        double cellW = m_zoom;
        double cellH = m_zoom;
        double elevY = 0.0;

        // 如果开启了浮起动效且在感应圆域内，实时同步浮起高度与尺寸缩放
        if (m_hoverEffectEnabled && m_hoverRadius > 0) {
            double mx = m_currentCursorPos.x();
            double my = m_currentCursorPos.y();
            double dist = std::sqrt((cx - mx) * (cx - mx) + (cy - my) * (cy - my));
            if (dist < m_hoverRadius) {
                double weight = std::cos((dist / m_hoverRadius) * (3.1415926535 * 0.5));
                elevY = -m_elevationIntensity * weight;
                double scale = 1.0 + 0.22 * weight;
                cellW = m_zoom * scale;
                cellH = m_zoom * scale;
            }
        }

        QRectF highlightRect(cx - cellW * 0.5,
                            cy - cellH * 0.5 + elevY,
                            cellW, cellH);
        painter.setPen(QPen(m_hoverHighlightColor, m_hoverHighlightWidth));
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(highlightRect);
    }

    // 绘制悬浮格点详细信息卡片 (HUD 信息框)
    if (m_showHoverInfoCard) {
        drawHoverInfoCard(painter);
    }

    double frameTime = timer.nsecsElapsed() / 1e6;
    emit renderStatsUpdated(frameTime, visibleCellsCount);
}

void VisualizerWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isPanning = true;
        m_lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }
}

void VisualizerWidget::mouseMoveEvent(QMouseEvent *event)
{
    m_mouseInWidget = true;
    m_currentCursorPos = event->pos();

    if (m_isPanning) {
        QPoint delta = event->pos() - m_lastMousePos;
        m_offset += delta;
        m_lastMousePos = event->pos();
        syncMinimapViewport();
    }

    // 计算当前光标悬浮所在的网格坐标
    QPointF gridPos = screenToGrid(event->pos());
    int col = static_cast<int>(std::floor(gridPos.x()));
    int row = static_cast<int>(std::floor(gridPos.y()));

    if (m_model && row >= 0 && row < m_model->rows() && col >= 0 && col < m_model->cols()) {
        m_hoveredRow = row;
        m_hoveredCol = col;
        m_hoveredRawVal = m_model->valueAt(row, col);
        m_hoveredNormVal = m_model->normalizedValueAt(row, col);
        emit cellHovered(row, col, m_hoveredRawVal, m_hoveredNormVal);
    } else {
        m_hoveredRow = -1;
        m_hoveredCol = -1;
        emit cellHovered(-1, -1, 0.0, 0.0);
    }

    update();
}

void VisualizerWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isPanning = false;
        setCursor(Qt::ArrowCursor);
    }
}

void VisualizerWidget::wheelEvent(QWheelEvent *event)
{
    // 支持 ctrl+滚轮顶点锚点缩放，或常规滚轮缩放
    QPoint mouseScreenPos = event->pos();
    QPointF gridPosBefore = screenToGrid(mouseScreenPos);

    double degrees = event->angleDelta().y() / 8.0;
    double steps = degrees / 15.0;
    double factor = std::pow(1.18, steps);

    double newZoom = std::clamp(m_zoom * factor, 0.05, 300.0);
    if (std::abs(newZoom - m_zoom) < 1e-6) {
        return;
    }

    m_zoom = newZoom;

    // 顶点锚点核心换算公式：保持光标下方对应的 grid 坐标像素位置不变
    m_offset.setX(mouseScreenPos.x() - gridPosBefore.x() * m_zoom);
    m_offset.setY(mouseScreenPos.y() - gridPosBefore.y() * m_zoom);

    emit zoomChanged(m_zoom);
    syncMinimapViewport();
    update();
}

void VisualizerWidget::leaveEvent(QEvent * /*event*/)
{
    m_mouseInWidget = false;
    m_hoveredRow = -1;
    m_hoveredCol = -1;
    emit cellHovered(-1, -1, 0.0, 0.0);
    update();
}

void VisualizerWidget::drawHoverInfoCard(QPainter &painter)
{
    if (!m_mouseInWidget || m_hoveredRow < 0 || m_hoveredCol < 0 || !m_model) {
        return;
    }

    int rows = m_model->rows();
    int cols = m_model->cols();
    if (m_hoveredRow >= rows || m_hoveredCol >= cols) {
        return;
    }

    // 计算空间连续坐标
    double spatialX = m_model->xMin();
    double spatialY = m_model->yMin();
    if (cols > 1) {
        spatialX += m_hoveredCol * (m_model->xMax() - m_model->xMin()) / (cols - 1);
    }
    if (rows > 1) {
        spatialY += m_hoveredRow * (m_model->yMax() - m_model->yMin()) / (rows - 1);
    }

    QString title = QStringLiteral("📍 当前格点信息");
    QString lineCoord = QStringLiteral("网格位置: 第 %1 行, 第 %2 列 (格号 #%3)")
                            .arg(m_hoveredRow)
                            .arg(m_hoveredCol)
                            .arg(m_hoveredRow * cols + m_hoveredCol);
    QString lineSpatial = QStringLiteral("空间范围: X = %1,  Y = %2")
                              .arg(spatialX, 0, 'f', 3)
                              .arg(spatialY, 0, 'f', 3);
    QString lineValue = QStringLiteral("采样数值: %1  (归一化: %2%)")
                            .arg(m_hoveredRawVal, 0, 'f', 3)
                            .arg(m_hoveredNormVal * 100.0f, 0, 'f', 1);

    QFont cardFont = painter.font();
    cardFont.setPointSize(9);
    cardFont.setBold(false);
    painter.setFont(cardFont);

    int cardW = 285;
    int cardH = 90;
    int margin = 14;

    // 默认在鼠标光标右下方偏移，若触碰边界则自适应翻转
    int cardX = m_currentCursorPos.x() + 18;
    int cardY = m_currentCursorPos.y() + 18;

    if (cardX + cardW > width() - margin) {
        cardX = m_currentCursorPos.x() - cardW - 14;
    }
    if (cardY + cardH > height() - margin) {
        cardY = m_currentCursorPos.y() - cardH - 14;
    }
    cardX = std::max(margin, cardX);
    cardY = std::max(margin, cardY);

    QRect cardRect(cardX, cardY, cardW, cardH);

    // 绘制半透明发光背景与圆角边框
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QColor(18, 22, 28, 230));
    painter.setPen(QPen(QColor(0, 210, 255, 180), 1.3));
    painter.drawRoundedRect(cardRect, 6, 6);

    // 标题
    QFont titleFont = cardFont;
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.setPen(QColor(0, 220, 255));
    painter.drawText(cardX + 12, cardY + 22, title);

    // 内容行
    painter.setFont(cardFont);
    painter.setPen(QColor(230, 230, 230));
    painter.drawText(cardX + 12, cardY + 43, lineCoord);
    painter.setPen(QColor(170, 200, 230));
    painter.drawText(cardX + 12, cardY + 62, lineSpatial);
    painter.setPen(QColor(255, 215, 80));
    painter.drawText(cardX + 12, cardY + 81, lineValue);
}
