#include "custom_viewport.h"

#include <QPainterPath>
#include <QtMath>

CustomViewport::CustomViewport() = default;

void CustomViewport::setRect(const QRectF &r) {
    qreal x = r.x();
    qreal y = r.y();
    qreal w = qMax(60.0, r.width());
    qreal h = qMax(60.0, r.height());
    m_rect = QRectF(x, y, w, h);
}

QMap<ViewportHandle, QPointF> CustomViewport::handlePositions() const {
    qreal l = m_rect.left();
    qreal r = m_rect.right();
    qreal t = m_rect.top();
    qreal b = m_rect.bottom();
    qreal mx = m_rect.center().x();
    qreal my = m_rect.center().y();

    QMap<ViewportHandle, QPointF> map;
    map[ViewportHandle::TopLeft]     = QPointF(l, t);
    map[ViewportHandle::Top]         = QPointF(mx, t);
    map[ViewportHandle::TopRight]    = QPointF(r, t);
    map[ViewportHandle::Right]       = QPointF(r, my);
    map[ViewportHandle::BottomRight] = QPointF(r, b);
    map[ViewportHandle::Bottom]      = QPointF(mx, b);
    map[ViewportHandle::BottomLeft]  = QPointF(l, b);
    map[ViewportHandle::Left]        = QPointF(l, my);
    return map;
}

ViewportHandle CustomViewport::hitTestHandle(const QPointF &pt, qreal tolerance) const {
    if (!m_enabled || !m_borderVisible) return ViewportHandle::None;

    auto handles = handlePositions();
    for (auto it = handles.constBegin(); it != handles.constEnd(); ++it) {
        QPointF d = pt - it.value();
        if (QPointF::dotProduct(d, d) <= tolerance * tolerance) {
            return it.key();
        }
    }
    return ViewportHandle::None;
}

bool CustomViewport::hitTestBorder(const QPointF &pt, qreal tolerance) const {
    if (!m_enabled || !m_borderVisible) return false;

    // 点到四条外边距的距离检测
    QRectF outer = m_rect.adjusted(-tolerance, -tolerance, tolerance, tolerance);
    QRectF inner = m_rect.adjusted(tolerance, tolerance, -tolerance, -tolerance);
    return outer.contains(pt) && !inner.contains(pt);
}

void CustomViewport::translate(const QPointF &delta) {
    m_rect.translate(delta);
}

void CustomViewport::stretchHandle(ViewportHandle handle, const QPointF &mousePos) {
    qreal minW = 60.0;
    qreal minH = 60.0;

    qreal l = m_rect.left();
    qreal r = m_rect.right();
    qreal t = m_rect.top();
    qreal b = m_rect.bottom();

    switch (handle) {
    case ViewportHandle::Left:
        l = qMin(mousePos.x(), r - minW);
        break;
    case ViewportHandle::Right:
        r = qMax(mousePos.x(), l + minW);
        break;
    case ViewportHandle::Top:
        t = qMin(mousePos.y(), b - minH);
        break;
    case ViewportHandle::Bottom:
        b = qMax(mousePos.y(), t + minH);
        break;
    case ViewportHandle::TopLeft:
        l = qMin(mousePos.x(), r - minW);
        t = qMin(mousePos.y(), b - minH);
        break;
    case ViewportHandle::TopRight:
        r = qMax(mousePos.x(), l + minW);
        t = qMin(mousePos.y(), b - minH);
        break;
    case ViewportHandle::BottomLeft:
        l = qMin(mousePos.x(), r - minW);
        b = qMax(mousePos.y(), t + minH);
        break;
    case ViewportHandle::BottomRight:
        r = qMax(mousePos.x(), l + minW);
        b = qMax(mousePos.y(), t + minH);
        break;
    default:
        return;
    }

    m_rect = QRectF(QPointF(l, t), QPointF(r, b));
}

void CustomViewport::drawMask(QPainter &painter, const QRectF &widgetRect) const {
    if (!m_enabled) return;

    painter.save();
    // 构造视口以外的外部暗色半透明蒙版
    QPainterPath fullPath;
    fullPath.addRect(widgetRect);

    QPainterPath vpPath;
    vpPath.addRect(m_rect);

    QPainterPath maskPath = fullPath.subtracted(vpPath);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(15, 23, 42, 160)); // 科技暗灰色半透明
    painter.drawPath(maskPath);

    painter.restore();
}

void CustomViewport::drawBorder(QPainter &painter, ViewportHandle hoveredHandle) const {
    if (!m_enabled || !m_borderVisible) return;

    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);

    // 1. 视口高对比亮紫色/洋红色外边框虚线
    QPen borderPen(QColor(168, 85, 247), 2.0, Qt::DashDotLine);
    painter.setPen(borderPen);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(m_rect);

    // 2. 8 个调整手柄
    auto handles = handlePositions();
    qreal r = 5.0;

    for (auto it = handles.constBegin(); it != handles.constEnd(); ++it) {
        bool isHovered = (it.key() == hoveredHandle);
        QColor strokeCol = isHovered ? QColor(236, 72, 153) : QColor(168, 85, 247);
        QColor fillCol = isHovered ? QColor(255, 255, 255) : QColor(243, 232, 255);

        painter.setPen(QPen(strokeCol, 2.0));
        painter.setBrush(fillCol);
        painter.drawEllipse(it.value(), r, r);
    }

    // 3. 视口左上角尺寸铭牌标牌
    QRectF badgeRect(m_rect.left(), m_rect.top() - 22, 180, 20);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(168, 85, 247, 220));
    painter.drawRoundedRect(badgeRect, 4, 4);

    QFont font = painter.font();
    font.setPointSize(9);
    font.setBold(true);
    painter.setFont(font);
    painter.setPen(Qt::white);
    QString badgeText = QStringLiteral("外部视口: %1 x %2")
                            .arg(qRound(m_rect.width()))
                            .arg(qRound(m_rect.height()));
    painter.drawText(badgeRect, Qt::AlignCenter, badgeText);

    painter.restore();
}
