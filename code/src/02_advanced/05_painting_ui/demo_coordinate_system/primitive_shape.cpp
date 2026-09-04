#include "primitive_shape.h"

#include <QLinearGradient>
#include <QPainter>
#include <QPolygonF>
#include <QtMath>

// -------------------------------------------------------------
// RectanglePrimitive
// -------------------------------------------------------------
void RectanglePrimitive::draw(QPainter &painter, const QSizeF &size) {
    QRectF rect(-size.width() / 2.0, -size.height() / 2.0, size.width(), size.height());

    // 渐变填充增加视觉质感
    QLinearGradient grad(rect.topLeft(), rect.bottomRight());
    grad.setColorAt(0.0, m_fillColor.lighter(130));
    grad.setColorAt(1.0, m_fillColor);

    painter.setPen(QPen(m_strokeColor, m_lineWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush(grad);
    painter.drawRoundedRect(rect, 8.0, 8.0);
}

QPainterPath RectanglePrimitive::shapePath(const QSizeF &size) const {
    QRectF rect(-size.width() / 2.0, -size.height() / 2.0, size.width(), size.height());
    QPainterPath path;
    path.addRoundedRect(rect, 8.0, 8.0);
    return path;
}

// -------------------------------------------------------------
// TrianglePrimitive
// -------------------------------------------------------------
void TrianglePrimitive::draw(QPainter &painter, const QSizeF &size) {
    qreal hw = size.width() / 2.0;
    qreal hh = size.height() / 2.0;

    QPolygonF triangle;
    triangle << QPointF(0.0, -hh)      // 顶点
             << QPointF(hw, hh)        // 右下
             << QPointF(-hw, hh);      // 左下

    QLinearGradient grad(0, -hh, 0, hh);
    grad.setColorAt(0.0, m_fillColor.lighter(140));
    grad.setColorAt(1.0, m_fillColor);

    painter.setPen(QPen(m_strokeColor, m_lineWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush(grad);
    painter.drawPolygon(triangle);
}

QPainterPath TrianglePrimitive::shapePath(const QSizeF &size) const {
    qreal hw = size.width() / 2.0;
    qreal hh = size.height() / 2.0;

    QPolygonF triangle;
    triangle << QPointF(0.0, -hh) << QPointF(hw, hh) << QPointF(-hw, hh);

    QPainterPath path;
    path.addPolygon(triangle);
    path.closeSubpath();
    return path;
}

// -------------------------------------------------------------
// HexagonPrimitive
// -------------------------------------------------------------
void HexagonPrimitive::draw(QPainter &painter, const QSizeF &size) {
    qreal rx = size.width() / 2.0;
    qreal ry = size.height() / 2.0;

    QPolygonF hexagon;
    for (int i = 0; i < 6; ++i) {
        qreal angle = i * (M_PI / 3.0); // 60度间隔
        hexagon << QPointF(rx * qCos(angle), ry * qSin(angle));
    }

    QLinearGradient grad(-rx, -ry, rx, ry);
    grad.setColorAt(0.0, m_fillColor.lighter(135));
    grad.setColorAt(1.0, m_fillColor);

    painter.setPen(QPen(m_strokeColor, m_lineWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush(grad);
    painter.drawPolygon(hexagon);
}

QPainterPath HexagonPrimitive::shapePath(const QSizeF &size) const {
    qreal rx = size.width() / 2.0;
    qreal ry = size.height() / 2.0;

    QPolygonF hexagon;
    for (int i = 0; i < 6; ++i) {
        qreal angle = i * (M_PI / 3.0);
        hexagon << QPointF(rx * qCos(angle), ry * qSin(angle));
    }

    QPainterPath path;
    path.addPolygon(hexagon);
    path.closeSubpath();
    return path;
}

// -------------------------------------------------------------
// ImagePrimitive
// -------------------------------------------------------------
ImagePrimitive::ImagePrimitive() {
    generateDefaultPixmap();
}

void ImagePrimitive::generateDefaultPixmap() {
    // 生成一张高颜值科技感矢量风格默认位图 (256x256)
    m_pixmap = QPixmap(300, 300);
    m_pixmap.fill(Qt::transparent);

    QPainter p(&m_pixmap);
    p.setRenderHint(QPainter::Antialiasing);

    // 圆角卡片背景
    QRectF bgRect(10, 10, 280, 280);
    QLinearGradient bgGrad(10, 10, 290, 290);
    bgGrad.setColorAt(0.0, QColor(24, 32, 54));
    bgGrad.setColorAt(0.5, QColor(41, 55, 90));
    bgGrad.setColorAt(1.0, QColor(15, 23, 42));

    p.setPen(QPen(QColor(59, 130, 246), 3));
    p.setBrush(bgGrad);
    p.drawRoundedRect(bgRect, 24, 24);

    // 内部科技感同心圆环与雷达线条
    p.setPen(QPen(QColor(96, 165, 250, 120), 1.5, Qt::DashLine));
    p.setBrush(Qt::NoBrush);
    p.drawEllipse(QPointF(150, 150), 90, 90);
    p.drawEllipse(QPointF(150, 150), 55, 55);

    // 中心微标
    QRadialGradient centerGrad(150, 150, 35);
    centerGrad.setColorAt(0.0, QColor(244, 63, 94));
    centerGrad.setColorAt(1.0, QColor(225, 29, 72, 80));
    p.setPen(QPen(QColor(255, 255, 255), 2));
    p.setBrush(centerGrad);
    p.drawEllipse(QPointF(150, 150), 28, 28);

    // 文字
    QFont font = p.font();
    font.setFamily("Segoe UI");
    font.setPointSize(12);
    font.setBold(true);
    p.setFont(font);
    p.setPen(QColor(241, 245, 249));
    p.drawText(bgRect.adjusted(0, 0, 0, -20), Qt::AlignBottom | Qt::AlignHCenter, QStringLiteral("Qt 2D Transform"));
}

void ImagePrimitive::draw(QPainter &painter, const QSizeF &size) {
    QRectF targetRect(-size.width() / 2.0, -size.height() / 2.0, size.width(), size.height());
    if (!m_pixmap.isNull()) {
        painter.drawPixmap(targetRect.toRect(), m_pixmap);
    }
}

QPainterPath ImagePrimitive::shapePath(const QSizeF &size) const {
    QRectF rect(-size.width() / 2.0, -size.height() / 2.0, size.width(), size.height());
    QPainterPath path;
    path.addRect(rect);
    return path;
}

// -------------------------------------------------------------
// Factory
// -------------------------------------------------------------
std::unique_ptr<PrimitiveShape> createPrimitive(PrimitiveType type) {
    switch (type) {
    case PrimitiveType::Rectangle:
        return std::make_unique<RectanglePrimitive>();
    case PrimitiveType::Triangle:
        return std::make_unique<TrianglePrimitive>();
    case PrimitiveType::Hexagon:
        return std::make_unique<HexagonPrimitive>();
    case PrimitiveType::Image:
        return std::make_unique<ImagePrimitive>();
    default:
        return std::make_unique<RectanglePrimitive>();
    }
}
