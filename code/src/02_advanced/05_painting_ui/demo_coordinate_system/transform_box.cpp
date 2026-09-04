#include "transform_box.h"

#include <QtMath>
#include <cmath>
#include <algorithm>

TransformBox::TransformBox() {
    m_aspectRatio = m_size.width() / m_size.height();
}

void TransformBox::setSize(const QSizeF &s) {
    qreal w = qMax(10.0, s.width());
    qreal h = qMax(10.0, s.height());
    m_size = QSizeF(w, h);
    m_aspectRatio = w / h;
}

void TransformBox::setRotation(qreal deg) {
    m_rotation = std::fmod(deg, 360.0);
    if (m_rotation < 0.0) {
        m_rotation += 360.0;
    }
}

QTransform TransformBox::transform() const {
    QTransform t;
    t.translate(m_center.x(), m_center.y());
    t.rotate(m_rotation);
    return t;
}

QTransform TransformBox::invertedTransform() const {
    return transform().inverted();
}

QRectF TransformBox::localRect() const {
    return QRectF(-m_size.width() / 2.0, -m_size.height() / 2.0, m_size.width(), m_size.height());
}

QPointF TransformBox::mapToWorld(const QPointF &localPt) const {
    return transform().map(localPt);
}

QPointF TransformBox::mapFromWorld(const QPointF &worldPt) const {
    return invertedTransform().map(worldPt);
}

QPolygonF TransformBox::worldPolygon() const {
    QRectF lr = localRect();
    QPolygonF poly;
    poly << mapToWorld(lr.topLeft())
         << mapToWorld(lr.topRight())
         << mapToWorld(lr.bottomRight())
         << mapToWorld(lr.bottomLeft());
    return poly;
}

bool TransformBox::containsPoint(const QPointF &worldPt) const {
    QPointF lp = mapFromWorld(worldPt);
    return localRect().contains(lp);
}

QMap<BoxHandle, QPointF> TransformBox::handleWorldPositions() const {
    QRectF lr = localRect();
    qreal left = lr.left();
    qreal right = lr.right();
    qreal top = lr.top();
    qreal bottom = lr.bottom();
    qreal midX = 0.0;
    qreal midY = 0.0;

    QMap<BoxHandle, QPointF> map;
    map[BoxHandle::TopLeft]     = mapToWorld(QPointF(left, top));
    map[BoxHandle::Top]         = mapToWorld(QPointF(midX, top));
    map[BoxHandle::TopRight]    = mapToWorld(QPointF(right, top));
    map[BoxHandle::Right]       = mapToWorld(QPointF(right, midY));
    map[BoxHandle::BottomRight] = mapToWorld(QPointF(right, bottom));
    map[BoxHandle::Bottom]      = mapToWorld(QPointF(midX, bottom));
    map[BoxHandle::BottomLeft]  = mapToWorld(QPointF(left, bottom));
    map[BoxHandle::Left]        = mapToWorld(QPointF(left, midY));
    return map;
}

BoxHandle TransformBox::oppositeHandle(BoxHandle handle) {
    switch (handle) {
    case BoxHandle::TopLeft:     return BoxHandle::BottomRight;
    case BoxHandle::Top:         return BoxHandle::Bottom;
    case BoxHandle::TopRight:    return BoxHandle::BottomLeft;
    case BoxHandle::Right:       return BoxHandle::Left;
    case BoxHandle::BottomRight: return BoxHandle::TopLeft;
    case BoxHandle::Bottom:      return BoxHandle::Top;
    case BoxHandle::BottomLeft:  return BoxHandle::TopRight;
    case BoxHandle::Left:        return BoxHandle::Right;
    default:                     return BoxHandle::None;
    }
}

BoxHandle TransformBox::hitTestHandle(const QPointF &worldPt, qreal tolerance) const {
    if (!m_visible) return BoxHandle::None;

    auto handles = handleWorldPositions();
    for (auto it = handles.constBegin(); it != handles.constEnd(); ++it) {
        QPointF delta = worldPt - it.value();
        if (QPointF::dotProduct(delta, delta) <= tolerance * tolerance) {
            return it.key();
        }
    }
    return BoxHandle::None;
}

void TransformBox::translate(const QPointF &delta) {
    m_center += delta;
}

void TransformBox::rotateAround(const QPointF &pivot, qreal deltaAngle) {
    setRotation(m_rotation + deltaAngle);

    // 围绕 pivot 旋转中心点
    qreal rad = qDegreesToRadians(deltaAngle);
    qreal cosA = qCos(rad);
    qreal sinA = qSin(rad);

    QPointF rel = m_center - pivot;
    qreal nx = rel.x() * cosA - rel.y() * sinA;
    qreal ny = rel.x() * sinA + rel.y() * cosA;

    m_center = pivot + QPointF(nx, ny);
}

void TransformBox::scaleAround(const QPointF &pivot, qreal scaleFactor) {
    if (scaleFactor <= 0.001) return;

    qreal newW = qMax(10.0, m_size.width() * scaleFactor);
    qreal newH = qMax(10.0, m_size.height() * scaleFactor);

    // 重新计算真实的实际倍率（受 min 约束）
    qreal realFactor = newW / m_size.width();
    m_size = QSizeF(newW, newH);

    // 光标不动点中心位移公式：C_new = Pivot + (C_old - Pivot) * k
    m_center = pivot + (m_center - pivot) * realFactor;
}

void TransformBox::stretchHandle(BoxHandle handle, const QPointF &worldMousePos) {
    BoxHandle opp = oppositeHandle(handle);
    if (opp == BoxHandle::None) return;

    // 对角锚点的世界坐标（保持绝对不动）
    auto handleMap = handleWorldPositions();
    QPointF anchorPt = handleMap.value(opp);

    qreal rad = qDegreesToRadians(m_rotation);
    // 局部轴单位向量（世界坐标系下）
    QPointF u(qCos(rad), qSin(rad));   // 局部 +X 轴
    QPointF v(-qSin(rad), qCos(rad));  // 局部 +Y 轴

    // 锚点到当前鼠标位置的向量
    QPointF d = worldMousePos - anchorPt;
    qreal projU = QPointF::dotProduct(d, u);
    qreal projV = QPointF::dotProduct(d, v);

    qreal curW = m_size.width();
    qreal curH = m_size.height();

    qreal newW = curW;
    qreal newH = curH;

    // 符号判定
    switch (handle) {
    case BoxHandle::Right:
        newW = qMax(10.0, projU);
        m_center = anchorPt + u * (newW / 2.0);
        break;
    case BoxHandle::Left:
        newW = qMax(10.0, -projU);
        m_center = anchorPt - u * (newW / 2.0);
        break;
    case BoxHandle::Bottom:
        newH = qMax(10.0, projV);
        m_center = anchorPt + v * (newH / 2.0);
        break;
    case BoxHandle::Top:
        newH = qMax(10.0, -projV);
        m_center = anchorPt - v * (newH / 2.0);
        break;
    case BoxHandle::BottomRight:
        newW = qMax(10.0, projU);
        newH = qMax(10.0, projV);
        m_center = anchorPt + u * (newW / 2.0) + v * (newH / 2.0);
        break;
    case BoxHandle::TopLeft:
        newW = qMax(10.0, -projU);
        newH = qMax(10.0, -projV);
        m_center = anchorPt - u * (newW / 2.0) - v * (newH / 2.0);
        break;
    case BoxHandle::TopRight:
        newW = qMax(10.0, projU);
        newH = qMax(10.0, -projV);
        m_center = anchorPt + u * (newW / 2.0) - v * (newH / 2.0);
        break;
    case BoxHandle::BottomLeft:
        newW = qMax(10.0, -projU);
        newH = qMax(10.0, projV);
        m_center = anchorPt - u * (newW / 2.0) + v * (newH / 2.0);
        break;
    default:
        return;
    }

    m_size = QSizeF(newW, newH);
}

void TransformBox::resetRotation() {
    setRotation(0.0);
}

void TransformBox::fitInRect(const QRectF &targetRect, qreal marginRatio) {
    resetRotation();
    qreal availW = targetRect.width() * marginRatio;
    qreal availH = targetRect.height() * marginRatio;
    if (availW <= 0 || availH <= 0) return;

    qreal scale = qMin(availW / (m_aspectRatio * 100.0), availH / 100.0);
    qreal newW = (m_aspectRatio * 100.0) * scale;
    qreal newH = 100.0 * scale;

    m_size = QSizeF(newW, newH);
    m_center = targetRect.center();
}

void TransformBox::fitWidthCenter(const QRectF &targetRect, qreal marginRatio) {
    resetRotation();
    qreal availW = targetRect.width() * marginRatio;
    if (availW <= 0) return;

    qreal newW = availW;
    qreal newH = newW / m_aspectRatio;

    m_size = QSizeF(newW, newH);
    m_center = targetRect.center();
}

void TransformBox::fitHeightCenter(const QRectF &targetRect, qreal marginRatio) {
    resetRotation();
    qreal availH = targetRect.height() * marginRatio;
    if (availH <= 0) return;

    qreal newH = availH;
    qreal newW = newH * m_aspectRatio;

    m_size = QSizeF(newW, newH);
    m_center = targetRect.center();
}

void TransformBox::draw(QPainter &painter, BoxHandle hoveredHandle) const {
    if (!m_visible) return;

    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);

    // 1. 绘制包围盒虚线框
    QPolygonF poly = worldPolygon();
    QPen boxPen(QColor(16, 185, 129), 1.5, Qt::DashLine);
    painter.setPen(boxPen);
    painter.setBrush(Qt::NoBrush);
    painter.drawPolygon(poly);

    // 2. 绘制 8 个控制点手柄
    auto handles = handleWorldPositions();
    qreal handleRadius = 4.5;

    for (auto it = handles.constBegin(); it != handles.constEnd(); ++it) {
        bool isHovered = (it.key() == hoveredHandle);

        QColor fillCol = isHovered ? QColor(245, 158, 11) : QColor(255, 255, 255);
        QColor strokeCol = isHovered ? QColor(217, 119, 6) : QColor(16, 185, 129);

        painter.setPen(QPen(strokeCol, 1.8));
        painter.setBrush(fillCol);

        QRectF hRect(it.value().x() - handleRadius, it.value().y() - handleRadius,
                     handleRadius * 2.0, handleRadius * 2.0);
        painter.drawRect(hRect);
    }

    // 3. 绘制中心微十字与旋转指示
    painter.setPen(QPen(QColor(16, 185, 129, 180), 1.2));
    qreal crossSize = 5.0;
    painter.drawLine(QPointF(m_center.x() - crossSize, m_center.y()),
                     QPointF(m_center.x() + crossSize, m_center.y()));
    painter.drawLine(QPointF(m_center.x(), m_center.y() - crossSize),
                     QPointF(m_center.x(), m_center.y() + crossSize));

    painter.restore();
}
