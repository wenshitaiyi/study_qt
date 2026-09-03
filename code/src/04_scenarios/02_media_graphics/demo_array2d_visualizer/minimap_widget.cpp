#include "minimap_widget.h"

#include <QPainter>
#include <QMouseEvent>
#include <algorithm>

MinimapWidget::MinimapWidget(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(180, 180);
    setAttribute(Qt::WA_OpaquePaintEvent, false);
    setCursor(Qt::CrossCursor);
}

void MinimapWidget::setThumbnail(const QImage &image)
{
    m_thumbnail = image;
    update();
}

void MinimapWidget::setViewportRect(const QRectF &normalizedRect)
{
    m_viewportRect = normalizedRect;
    update();
}

void MinimapWidget::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // 半透明背景与圆角深色边框
    QRect r = rect().adjusted(1, 1, -1, -1);
    painter.setBrush(QColor(25, 25, 25, 210));
    painter.setPen(QPen(QColor(80, 80, 80, 200), 1.5));
    painter.drawRoundedRect(r, 6, 6);

    // 绘制矩阵缩略图
    QRect innerRect = r.adjusted(6, 6, -6, -6);
    if (!m_thumbnail.isNull()) {
        painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
        painter.drawImage(innerRect, m_thumbnail);
    } else {
        painter.setPen(QColor(150, 150, 150));
        painter.drawText(innerRect, Qt::AlignCenter, QStringLiteral("缩略图就绪中..."));
    }

    // 绘制视口指示框
    if (!m_viewportRect.isEmpty()) {
        double vx = innerRect.left() + m_viewportRect.left() * innerRect.width();
        double vy = innerRect.top() + m_viewportRect.top() * innerRect.height();
        double vw = m_viewportRect.width() * innerRect.width();
        double vh = m_viewportRect.height() * innerRect.height();

        QRectF viewScreenRect(vx, vy, vw, vh);
        // 限制在 innerRect 内相交显示
        QRectF clippedRect = viewScreenRect.intersected(QRectF(innerRect));

        painter.setBrush(QColor(255, 70, 70, 45));
        painter.setPen(QPen(QColor(255, 70, 70, 230), 1.8));
        painter.drawRect(clippedRect);
    }

    // 绘制外框微光
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(QColor(255, 255, 255, 30), 1));
    painter.drawRoundedRect(innerRect, 2, 2);
}

void MinimapWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDragging = true;
        handleMouseInteraction(event->pos());
    }
}

void MinimapWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isDragging) {
        handleMouseInteraction(event->pos());
    }
}

void MinimapWidget::handleMouseInteraction(const QPoint &pos)
{
    QRect innerRect = rect().adjusted(7, 7, -7, -7);
    if (innerRect.width() <= 0 || innerRect.height() <= 0) {
        return;
    }

    double normX = std::clamp(static_cast<double>(pos.x() - innerRect.left()) / innerRect.width(), 0.0, 1.0);
    double normY = std::clamp(static_cast<double>(pos.y() - innerRect.top()) / innerRect.height(), 0.0, 1.0);

    emit requestCenterOn(normX, normY);
}
